export module VectorBase;
import CSTDINT;
import Traits;
import Iterators;
import Sorts;
import <memory>;//std::allocator

export namespace System {
	template<class T>
	class VectorBase {
	public:
		static constexpr size_t ErrorValue = MAX_VALUE<size_t>;
	protected:
		T* m_data = nullptr;
		size_t m_count = 0;
		size_t m_capacity = 0;
	public:
		constexpr VectorBase() noexcept {}
		constexpr VectorBase(const VectorBase<T>& arg) noexcept {
			InternalReset(arg.m_capacity);
			m_count = arg.m_count;
			for (size_t i = 0; i < m_count; ++i) m_data[i] = arg.m_data[i];
		}
		constexpr VectorBase(VectorBase<T>&& arg) noexcept : m_data(arg.m_data), m_count(arg.m_count), m_capacity(arg.m_capacity) {
			arg.m_data = nullptr;
			arg.m_count = 0;
			arg.m_capacity = 0;
		}
		constexpr ~VectorBase() noexcept { InternalReset(); }
	public:
		/// <summary>
		/// 初期化子リストから動的配列を構築する。
		/// 各要素はT(const T&)によってコピーされる
		/// </summary>
		constexpr VectorBase(initializer_list<T> list) noexcept {
			InternalReset(list.size());
			m_count = m_capacity;
			size_t i = 0;
			for (T const& x : list) m_data[i++] = x;
		}
		/// <summary>
		/// 個数と値を指定して、同じ値が並ぶ動的配列を構築する。
		/// 各要素はT(const T&)でコピーされる
		/// </summary>
		constexpr VectorBase(size_t count, const T& value) noexcept {
			InternalReset(count);
			m_count = m_capacity;
			for (size_t i = 0; i < m_count; ++i) m_data[i] = value;
		}
		template<Traits::Concepts::CInputIterator InputIter>
		constexpr VectorBase(InputIter first, InputIter last) noexcept {
			auto tmp = last - first;
			if (tmp <= 0) return;
			InternalReset(static_cast<size_t>(tmp));
			m_count = m_capacity;
			for (size_t i = 0; i < m_count; ++i) m_data[i] = *first++;
		}
	private:
		constexpr void InternalReset(size_t newSize = 0) noexcept {
			std::allocator<T> al;
			if (m_data) {
				al.deallocate(m_data, m_capacity);
				m_data = nullptr;
				m_capacity = 0;
				m_count = 0;
			}
			if (newSize) {
				m_data = al.allocate(newSize);
				m_capacity = newSize;
				m_count = 0;
			}
		}
	public:
		constexpr ContiguousIterator<T> begin() noexcept { return ContiguousIterator<T>(m_data); }
		constexpr ContiguousIterator<T> end() noexcept { return ContiguousIterator<T>(m_data + m_count); }
		constexpr ContiguousIterator<const T> begin() const noexcept { return ContiguousIterator<const T>(m_data); }
		constexpr ContiguousIterator<const T> end() const noexcept { return ContiguousIterator<const T>(m_data + m_count); }
		constexpr ContiguousIterator<const T> cbegin() const noexcept { return ContiguousIterator<const T>(m_data); }
		constexpr ContiguousIterator<const T> cend() const noexcept { return ContiguousIterator<const T>(m_data + m_count); }
	public:
		constexpr size_t Capacity() const noexcept { return m_capacity; }
		constexpr size_t Count() const noexcept { return m_count; }
		constexpr T* Items() noexcept { return m_data; }
		constexpr const T* Items() const noexcept { return m_data; }
		constexpr bool Reserve(size_t count) noexcept { return Allocate(count); }
	protected:
		constexpr bool Allocate(size_t n) noexcept {
			if (n > SIZE_MAX - m_count) return false;
			std::allocator<T> al;
			if (!m_capacity) {
				m_capacity = n;
				m_count = 0;
				m_data = al.allocate(m_capacity);
				return true;
			}
			const size_t newSize = m_count + n;
			const size_t prevCapacity = m_capacity;
			if (newSize <= m_capacity) return true;
			while (newSize > m_capacity) {
				if (m_capacity & Traits::BITMASK_M<64, 64>) m_capacity = SIZE_MAX;
				else m_capacity *= 2;
			}
			T* tmp = al.allocate(m_capacity);
			for (size_t i = 0; i < m_count; ++i) tmp[i] = static_cast<T&&>(m_data[i]);
			al.deallocate(m_data, prevCapacity);
			m_data = tmp;
			return true;
		}
	protected:
		constexpr size_t AddInternal(T&& value) noexcept {
			m_data[m_count] = static_cast<T&&>(value);
			return m_count++;
		}
		constexpr size_t InsertInternal(size_t index, T&& value) noexcept {
			m_data[index] = static_cast<T&&>(value);
			return index;
		}
	public:
		/// <summary>
		/// 指定した個数分のデフォルト初期化された要素を追加する
		/// </summary>
		/// <param name="count"></param>
		constexpr bool AddEmpty(size_t count) noexcept {
			if (Allocate(count)) {
				m_count += count;
				return true;
			}
			else return false;
		}
		constexpr size_t Add(const T& value) noexcept {
			if (Allocate(1)) {
				m_data[m_count] = value;
				return m_count++;
			}
			else return ErrorValue;
		}
		constexpr size_t Add(T&& value) noexcept {
			if (Allocate(1)) {
				m_data[m_count] = static_cast<T&&>(value);
				return m_count++;
			}
			else return ErrorValue;
		}
		constexpr size_t AddRange(const T* args, size_t count) noexcept {
			if (Allocate(count)) {
				for (size_t i = 0; i < count; ++i) m_data[m_count++] = args[i];
				return m_count - 1;
			}
			else return ErrorValue;
		}
		template<class ...Args>
		constexpr size_t AddRange(T&& head, Args&& ...args) noexcept {
			if (Allocate(sizeof...(Args) + 1)) {
				m_data[m_count++] = static_cast<T&&>(head);
				for (auto&& x : initializer_list<size_t>{ AddInternal(static_cast<Args&&>(args))... }) {}
				return m_count - 1;
			}
			else return ErrorValue;
		}
		constexpr size_t CopyRange(size_t startOffset, size_t count) noexcept {
			if (startOffset >= m_count) return ErrorValue;
			if (Allocate(count)) {
				for (size_t i = 0; i < count; ++i) m_data[m_count++] = m_data[startOffset++];
				return m_count - 1;
			}
			else return ErrorValue;
		}
		constexpr size_t InsertByIndex(size_t index, const T& value) noexcept {
			if (Allocate(1)) {
				if (index >= m_count) index = m_count;
				for (size_t i = m_count; i-- > index;) m_data[i + 1] = static_cast<T&&>(m_data[i]);
				m_data[index] = value;
				++m_count;
				return index;
			}
			else return ErrorValue;
		}
		constexpr size_t InsertByIndex(size_t index, T&& value) noexcept {
			if (Allocate(1)) {
				if (index >= m_count) index = m_count;
				for (size_t i = m_count; i-- > index;) m_data[i + 1] = static_cast<T&&>(m_data[i]);
				m_data[index] = static_cast<T&&>(value);
				++m_count;
				return index;
			}
			else return ErrorValue;
		}
		constexpr size_t Insert(const ContiguousIterator<T>& ite, const T& value) noexcept {
			const ContiguousIterator<T> beg = begin();
			if (ite < beg) return ErrorValue;
			size_t index = ite - beg;
			if (index > m_count) return ErrorValue;
			return InsertByIndex(index, value);
		}
		constexpr size_t Insert(const ContiguousIterator<T>& ite, T&& value) noexcept {
			const ContiguousIterator<const T> beg = cbegin();
			if (ite < beg) return ErrorValue;
			size_t index = ite - beg;
			if (index > m_count) return ErrorValue;
			return InsertByIndex(index, static_cast<T&&>(value));
		}
		constexpr size_t InsertRange(size_t index, const T* args, size_t count) noexcept {
			if (Allocate(count)) {
				if (index >= m_count) index = m_count;
				for (size_t i = m_count; i-- > index;) m_data[i + count] = static_cast<T&&>(m_data[i]);
				for (size_t i = 0; i < count; ++i) m_data[index + i] = args[i];
				m_count += count;
				return index + count - 1;
			}
			else return ErrorValue;
		}
		template<class ...Args>
		constexpr size_t InsertRange(size_t index, T&& head, Args&& ...args) noexcept {
			const size_t count = sizeof...(Args) + 1;
			if (Allocate(count)) {
				if (index >= m_count) index = m_count;
				for (size_t i = m_count; i-- > index;) m_data[i + count] = static_cast<T&&>(m_data[i]);
				size_t current = index;
				m_data[current++] = static_cast<T&&>(head);
				for (auto&& x : { InsertInternal(current++, static_cast<Args&&>(args))... }) {}
				m_count += count;
				return current - 1;
			}
			else return ErrorValue;
		}
		constexpr size_t RemoveByIndex(size_t index) noexcept {
			if (index >= m_count) return ErrorValue;
			for (size_t i = index + 1; i < m_count; ++i) m_data[i - 1] = static_cast<T&&>(m_data[i]);
			if (index + 1 == m_count) m_data[index].~T();
			return --m_count;
		}
		constexpr size_t RemoveValue(const T& value) noexcept { return RemoveByIndex(IndexOf(value)); }
		constexpr size_t Remove(const ContiguousIterator<T>& ite) noexcept {
			const ContiguousIterator<T> beg = begin();
			if (ite < beg) return ErrorValue;
			else return RemoveByIndex(ite - beg);
		}
	public:
		constexpr void Sort(bool ascendingOrder) noexcept { Sorts::QuickSort(begin(), end(), ascendingOrder); }
		template<Traits::Concepts::CCompareFunction<Traits::remove_cvref_t<T>> CompareFunc>
		constexpr void Sort(bool ascendingOrder, CompareFunc compare) noexcept {
			Sorts::QuickSort(begin(), end(), ascendingOrder, compare);
		}
	public:
		constexpr bool Contain(const T& value) const noexcept {
			for (const T& x : *this) if (x == value) return true;
			return false;
		}
		constexpr size_t IndexOf(const T& value) const noexcept {
			for (size_t i = 0; i < m_count; ++i) if (m_data[i] == value) return i;
			return m_count;
		}
		constexpr ContiguousIterator<T> GetIteratorFromValue(const T& value) noexcept {
			return ContiguousIterator<T>(m_data + IndexOf(value));
		}
		constexpr ContiguousIterator<const T> GetIteratorFromValue(const T& value) const noexcept {
			return ConstContiguousIterator<T>(m_data + IndexOf(value));
		}
		constexpr ContiguousIterator<T> GetIterator(size_t index) noexcept {
			return ContiguousIterator<T>(m_data + index);
		}
		constexpr ContiguousIterator<const T> GetIterator(size_t index) const noexcept {
			return ConstContiguousIterator<T>(m_data + index);
		}
	public:
		constexpr void DeleteAll() noexcept requires(Traits::is_pointer_v<T>) {
			for (T& x : *this) delete x;
			InternalReset();
		}
		constexpr void Clear() noexcept { InternalReset(); }
		constexpr T* Release() noexcept {
			T* ret = m_data;
			m_data = nullptr;
			m_count = 0;
			m_capacity = 0;
			return ret;
		}
	public:
		constexpr T& Last() { return m_data[m_count - 1]; }
		constexpr const T& Last() const { return m_data[m_count - 1]; }
	public:
		constexpr T& operator[](size_t index) { return m_data[index]; }
		constexpr const T& operator[](size_t index) const { return m_data[index]; }
	public:
		constexpr VectorBase& operator=(const VectorBase& rhs) noexcept {
			if (this == &rhs) return *this;
			InternalReset(rhs.m_capacity);
			m_count = rhs.m_count;
			for (size_t i = 0; i < m_count; ++i) m_data[i] = rhs.m_data[i];
			return *this;
		}
		constexpr VectorBase& operator=(VectorBase&& rhs) noexcept {
			if (this == &rhs) return *this;
			InternalReset();
			m_data = rhs.m_data;
			m_capacity = rhs.m_capacity;
			m_count = rhs.m_count;
			rhs.m_data = nullptr;
			rhs.m_capacity = 0;
			rhs.m_count = 0;
			return *this;
		}
		constexpr VectorBase& operator=(initializer_list<T> list) noexcept {
			InternalReset(list.size());
			m_count = m_capacity;
			size_t i = 0;
			for (T const& x : list) m_data[i++] = x;
			return *this;
		}
	};
	template<Traits::Concepts::CInputIterator InputIter>
	VectorBase(InputIter first, InputIter last) -> VectorBase<Traits::remove_cvref_t<decltype(*first)>>;
}
