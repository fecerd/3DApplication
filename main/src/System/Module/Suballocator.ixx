export module Suballocator;
import CSTDINT;
import Memory;

//ForwardNode
export namespace System {
	template<class T>
	struct ForwardNode {
		T value;
		ForwardNode<T>* next = nullptr;
	};
}

namespace System {
	class Subresource;
	struct SubresourceRange {
		Subresource* resource = nullptr;
		size_t offset;
		size_t range;
		bool used;
	};
}

export namespace System {
	class Suballocator;
	class Subresource {
		friend class Suballocator;
		Suballocator* m_allocator = nullptr;
		ForwardNode<SubresourceRange>* m_node = nullptr;
	private:
		Subresource(Suballocator* allocator = nullptr, ForwardNode<SubresourceRange>* node = nullptr) noexcept
			: m_allocator(allocator), m_node(node)
		{
			m_node->value.resource = this;
		}
	public:
		Subresource(const Subresource&) noexcept = delete;
		Subresource(Subresource&& arg) noexcept
			: m_allocator(arg.m_allocator), m_node(arg.m_node)
		{
			arg.m_allocator = nullptr;
			arg.m_node = nullptr;
			m_node->value.resource = this;
		}
		~Subresource() noexcept;
	public:
		void* GetBegin() noexcept;
		const void* GetBegin() const noexcept;
		void* GetEnd() noexcept { return static_cast<uint8_t*>(GetBegin()) + GetSize(); }
		const void* GetEnd() const noexcept { return static_cast<const uint8_t*>(GetBegin()) + GetSize(); }
		size_t GetOffset() const noexcept { return m_node ? m_node->value.offset : 0; }
		size_t GetSize() const noexcept { return m_node ? m_node->value.range : 0; }
		bool Enabled() const noexcept;
	public:
		Subresource& operator=(const Subresource&) noexcept = delete;
		Subresource& operator=(Subresource&& rhs) noexcept {
			if (this == &rhs) return *this;
			m_allocator = rhs.m_allocator;
			m_node = rhs.m_node;
			m_node->value.resource = this;
			rhs.m_allocator = nullptr;
			rhs.m_node = nullptr;
			return *this;
		}
	};

	class Suballocator {
		friend class Subresource;
		using node_t = ForwardNode<SubresourceRange>;
		uint8_t* m_heap = nullptr;
		size_t m_size = 0;
		node_t* m_first = nullptr;
		bool m_delete = false;
	public:
		Suballocator(uint8_t* heap = nullptr, size_t size = 0, bool deleteInDestructor = false) noexcept
			: m_heap(heap), m_size(size), m_delete(deleteInDestructor)
		{
			m_first = new node_t();
			m_first->value.offset = 0;
			m_first->value.range = m_size;
			m_first->value.used = false;
		}
		Suballocator(const Suballocator&) noexcept = delete;
		Suballocator(Suballocator&& arg) noexcept
			: m_heap(arg.m_heap), m_size(arg.m_size), m_first(arg.m_first), m_delete(arg.m_delete)
		{
			arg.m_heap = nullptr;
			arg.m_size = 0;
			arg.m_first = nullptr;
			arg.m_delete = false;
			node_t* node = m_first;
			while (node) {
				Subresource* res = node->value.resource;
				if (res) res->m_allocator = this;
				node = node->next;
			}
		}
		~Suballocator() noexcept { Reset(); }
	private:/* Subresource用 */
		void Free(node_t*& node) noexcept {
			if (!node) return;
			node->value.used = false;
			node_t* next = node->next;
			if (next && !next->value.used) {
				next->value.range += next->value.offset - node->value.offset;
				next->value.offset = node->value.offset;
				node->value.range = 0;
			}
			if (node == m_first) {
				if (!node->value.range) {
					m_first = node->next;
					delete node;
					node = nullptr;
				}
			}
			else {
				node_t* prev = m_first;
				while (prev->next != node) prev = prev->next;
				//prevが見つからないとき、nodeがこのインスタンスのものでないということだが、
				//Subresourceクラスから呼び出す以上ありえない
				if (!prev) return;
				if (!prev->value.used) {
					prev->value.range += node->value.range;
					node->value.offset = prev->value.offset + prev->value.range;
					node->value.range = 0;
				}
				if (!node->value.range) {
					prev->next = node->next;
					delete node;
					node = nullptr;
				}
			}
		}
	private:
		void Reset() noexcept {
			node_t* node = m_first;
			m_first = nullptr;
			while (node) {
				node_t* next = node->next;
				Subresource* subres = node->value.resource;
				if (subres) {
					subres->m_allocator = nullptr;
					subres->m_node = nullptr;
				}
				delete node;
				node = next;
			}
			m_size = 0;
			if (m_delete) delete[] m_heap;
			m_heap = nullptr;
		}
	public:
		Subresource Allocate(size_t byteSize, size_t alignment) noexcept {
			if (!m_heap || !m_size || !m_first) return Subresource();
			node_t* prev = nullptr;
			node_t** prevNodeNext = &m_first;
			node_t* node = m_first;
			do {
				if (node->value.used) continue;
				const size_t alignedOffset = Memory::GetAlignedSize(node->value.offset, alignment);
				const size_t endOffset = alignedOffset + byteSize;
				const size_t nodeEndOffset = node->value.offset + node->value.range;
				if (endOffset > nodeEndOffset) continue;
				const size_t offsetDelta = alignedOffset - node->value.offset;
				//整列によって余った領域がある場合
				if (offsetDelta) {
					//前方に未使用Nodeがあるなら結合する
					if (prev && !prev->value.used) {
						prev->value.range += offsetDelta;
					}
					//前方のNodeが存在しない、もしくは使用中の場合、Nodeを分割する
					else {
						node_t* tmp = new node_t();
						tmp->value.offset = node->value.offset;
						tmp->value.range = offsetDelta;
						tmp->value.used = false;
						tmp->next = node;
						*prevNodeNext = tmp;
						prevNodeNext = &tmp->next;
					}
					node->value.offset = alignedOffset;
					node->value.range -= offsetDelta;
				}
				const size_t endOffsetDelta = nodeEndOffset - endOffset;
				//必要なサイズよりもNodeのサイズが大きい場合
				if (endOffsetDelta) {
					//後方に未使用Nodeがあるなら結合する
					if (node->next && !node->next->value.used) {
						node->next->value.offset -= endOffsetDelta;
						node->next->value.range += endOffsetDelta;
					}
					//後方のNodeが存在しない、もしくは使用中の場合、Nodeを分割する
					else {
						node_t* tmp = new node_t();
						tmp->value.offset = endOffset;
						tmp->value.range = endOffsetDelta;
						tmp->value.used = false;
						tmp->next = node->next;
						node->next = tmp;
					}
					node->value.range -= endOffsetDelta;
				}
				//ここまででnodeはalignedOffsetからbyteSize分の領域を表すようになる
				node->value.used = true;
				return Subresource(this, node);

			} while ((prev = node) && (prevNodeNext = &node->next) && (node = node->next));
			return Subresource();
		}
		bool Enabled() const noexcept {
			return m_heap && m_size && m_first;
		}
	public:
		Suballocator& operator=(const Suballocator&) noexcept = delete;
		Suballocator& operator=(Suballocator&& rhs) noexcept {
			if (this == &rhs) return *this;
			Reset();
			m_heap = rhs.m_heap;
			m_size = rhs.m_size;
			m_first = rhs.m_first;
			m_delete = rhs.m_delete;
			rhs.m_heap = nullptr;
			rhs.m_size = 0;
			rhs.m_first = nullptr;
			rhs.m_delete = false;
			node_t* node = m_first;
			while (node) {
				Subresource* res = node->value.resource;
				if (res) res->m_allocator = this;
				node = node->next;
			}
			return *this;
		}
	};
}

namespace System {
	Subresource::~Subresource() noexcept {
		if (!Enabled()) return;
		m_allocator->Free(m_node);
		m_allocator = nullptr;
	}
	void* Subresource::GetBegin() noexcept {
		if (!m_allocator || !m_node) return nullptr;
		return m_allocator->m_heap + m_node->value.offset;
	}
	const void* Subresource::GetBegin() const noexcept {
		if (!m_allocator || !m_node) return nullptr;
		return m_allocator->m_heap + m_node->value.offset;
	}
	bool Subresource::Enabled() const noexcept { return m_allocator && m_allocator->Enabled() && m_node; }
}
