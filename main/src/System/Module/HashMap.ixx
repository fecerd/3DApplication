module;
#include<crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
export module HashMap;
import CSTDINT;
import Exception;
import IEnumerable;
import Traits;
import Vector;
import Function;
using IEnumerable = System::IEnumerable;

//Hash�֐��I�u�W�F�N�g�^
export namespace System {
	template<class T> class Hash {};
	template<System::Traits::Arithmetic T>
	class Hash<T> {
	public:
		constexpr size_t operator()(T arg) const noexcept { return static_cast<size_t>(arg); }
	};
	template<class T> requires(System::Traits::is_same_v<System::Traits::remove_cv_t<T>, bool>)
	class Hash<T> {
	public:
		constexpr size_t operator()(T arg) const noexcept { return static_cast<size_t>(arg); }
	};
	template<System::Traits::Pointer T>
	class Hash<T> {
	public:
		inline size_t operator()(const Traits::remove_pointer_t<Traits::remove_cvref_t<T>>* arg) const noexcept { return reinterpret_cast<size_t>(arg); }
	};
	template<class T> requires requires(const T& x) { x.GetHashCode(); }
	class Hash<T> {
	public:
		inline size_t operator()(const T& arg) const noexcept {
			return static_cast<size_t>(arg.GetHashCode());
		}
	};
	template<Traits::Concepts::CEnum T>
	class Hash<T> {
	public:
		constexpr size_t operator()(T arg) const noexcept {
			return static_cast<size_t>(static_cast<System::Traits::underlying_type_t<T>>(arg));
		}
	};
}

//HashNodeBase, HashNode
export namespace System {
	/// <summary>
	/// HashNode�̊��N���X
	/// </summary>
	/// <typeparam name="Key">�L�[�^</typeparam>
	template<class Key>
	struct HashNodeBase {
	protected:
		HashNodeBase* m_prev = nullptr;	//HashMap���Œ��O�ɒǉ����ꂽ�m�[�h
		HashNodeBase* m_next = nullptr;	//HashMap���Œ���ɒǉ����ꂽ�m�[�h
		Key m_key;	//�L�[
	public:
		template<Traits::Constructible<Key> K>
		HashNodeBase(K&& key) noexcept : m_key(static_cast<K&&>(key)) {}
	public:
		/// <summary>
		/// �L�[�̎Q�Ƃ��擾����
		/// </summary>
		Key& GetKey() noexcept { return m_key; }
		const Key& GetKey() const noexcept { return m_key; }
	public:
		/// <summary>
		/// ���̃m�[�h�����݂���HashMap���ň�ԍŏ��ɒǉ����ꂽ�m�[�h���擾����
		/// </summary>
		HashNodeBase* GetFirst() noexcept { return m_prev ? m_prev->GetFirst() : this; }
		/// <summary>
		/// ���̃m�[�h�����݂���HashMap���ň�ԍŌ�ɒǉ����ꂽ�m�[�h���擾����
		/// </summary>
		HashNodeBase* GetLast() noexcept { return m_next ? m_next->GetLast() : this; }
		/// <summary>
		/// ���O�̃m�[�h��ݒ肷��
		/// </summary>
		/// <param name="prev">�ݒ肷��m�[�h</param>
		void SetPrev(HashNodeBase* prev) noexcept { m_prev = prev; }
		/// <summary>
		/// ���O�̃m�[�h���擾����
		/// </summary>
		HashNodeBase* GetPrev() const noexcept { return m_prev; }
		/// <summary>
		/// ����̃m�[�h��ݒ肷��
		/// </summary>
		/// <param name="prev">�ݒ肷��m�[�h</param>
		void SetNext(HashNodeBase* next) noexcept { m_next = next; }
		/// <summary>
		/// ����̃m�[�h���擾����
		/// </summary>
		HashNodeBase* GetNext() const noexcept { return m_next; }
	};

	/// <summary>
	/// HashNode�̂ЂȌ^
	/// </summary>
	/// <typeparam name="Key"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	template<class Key, class ...Args>
	struct HashNode : public HashNodeBase<Key> {
		template<size_t i>
		using value_type = System::Traits::one_of_t<i, Args...>;
	public:
		using HashNodeBase<Key>::GetKey;
		template<size_t i = 0> value_type<i>& GetValue() noexcept;
		template<size_t i = 0> value_type<i>* GetValuePtr() noexcept;
	};
}

//HashNodes
export namespace System {
	/// <summary>
	/// �l�������HashMap�p�m�[�h
	/// </summary>
	/// <typeparam name="Key">�L�[�^</typeparam>
	/// <typeparam name="Value">�l�^</typeparam>
	template<class Key, class Value>
	struct HashNode<Key, Value> : public HashNodeBase<Key> {
		using HashNodeBase<Key>::m_next;
		using HashNodeBase<Key>::m_key;
		Value m_value;
	public:
		template<Traits::Constructible<Key> K, Traits::Constructible<Value> V>
		HashNode<Key, Value>(K&& key, V&& value) noexcept
			: HashNodeBase<Key>(static_cast<K&&>(key)), m_value(static_cast<V&&>(value)) {}
	public:
		using HashNodeBase<Key>::GetKey;
		/// <summary>
		/// �l���擾����
		/// </summary>
		template<size_t i = 0> requires(i == 0)
		Value& GetValue() noexcept { return m_value; }
		/// <summary>
		/// �l�ւ̃|�C���^���擾����
		/// </summary>
		template<size_t i = 0> requires(i == 0)
		Value* GetValuePtr() noexcept { return &m_value; }
	};
}

namespace System {
	template<class T>
	struct is_node : System::Traits::false_type {};
	template<template<class, class...> class Node, class Key, class ...Args>
	struct is_node<Node<Key, Args...>> : System::Traits::true_type {};
	template<template<class, class...> class Node, class Key, class ...Args>
	struct is_node<const Node<Key, Args...>> : System::Traits::true_type {};

	template<class T>
	concept NodeType = is_node<T>::value;

	template<class T>
	struct toggle_const {
		using type = const T;
	};
	template<class T>
	struct toggle_const<const T> {
		using type = System::Traits::remove_const_t<T>;
	};
}

//HashMapIterator
export namespace System {
	/// <summary>
	/// HashMap::begin(), end()���Ԃ��o�����C�e���[�^
	/// </summary>
	/// <typeparam name="Key">�L�[�^</typeparam>
	/// <typeparam name="...Args">�l�^</typeparam>
	template<NodeType T>
	struct HashMapIterator {
		using iterator_concept = System::Traits::bidirectional_iterator_tag;
		using nodetype = T;
		using iterator_type = HashMapIterator<T>;
		using toggle_iterator_type = HashMapIterator<typename toggle_const<T>::type>;
		friend struct HashMapIterator<typename toggle_const<T>::type>;
	protected:
		//���ݎw���Ă���m�[�h�Bnullptr�̏ꍇ�A�ŏI�m�[�h�̌���\��
		nodetype* m_current = nullptr;
	public:
		constexpr HashMapIterator() noexcept : m_current(nullptr) {}
		constexpr HashMapIterator(nodetype* node) noexcept : m_current(node) {}
		constexpr ~HashMapIterator() noexcept { m_current = nullptr; }
	public:
		constexpr iterator_type& operator++() noexcept {
			if (!m_current) return *this;
			m_current = static_cast<nodetype*>(m_current->GetNext());
			return *this;
		}
		constexpr iterator_type operator++(int) noexcept {
			if (!m_current) return iterator_type(m_current);
			iterator_type ret(m_current);
			m_current = static_cast<nodetype*>(m_current->GetNext());
			return ret;
		}
		constexpr iterator_type& operator--() noexcept {
			if (!m_current) return *this;
			m_current = static_cast<nodetype*>(m_current->GetPrev());
			return *this;
		}
		constexpr iterator_type operator--(int) noexcept {
			if (!m_current) return iterator_type(m_current);
			iterator_type ret(m_current);
			m_current = static_cast<nodetype*>(m_current->GetPrev());
			return ret;
		}
		constexpr nodetype& operator*() const noexcept { return *m_current; }
		constexpr ptrdiff_t operator-(const iterator_type& rhs) const {
			if (!m_current && !rhs.m_current) return 0;
			ptrdiff_t ret = 0;
			nodetype* tmp = nullptr;
			if (!m_current || !rhs.m_current) {	//this��rhs�̕Е����ŏI�m�[�h�̎�(nullptr)���w���Ă���ꍇ
				if (!m_current) {
					tmp = rhs.m_current;
					while (tmp) {
						++ret;
						tmp = tmp->GetNext();
					}
					return ret;
				}
				else {
					tmp = m_current;
					while (tmp) {
						--ret;
						tmp = tmp->GetNext();
					}
					return ret;
				}
			}
			else {	//this��rhs���Ƃ��Ƀm�[�h���w���Ă���ꍇ
				tmp = m_current;
				while (tmp) {
					if (tmp == rhs.m_current) return ret;
					++ret;
					tmp = tmp->GetNext();
				}
				ret = -1;
				tmp = m_current->GetPrev();
				while (tmp) {
					if (tmp == rhs.m_current) return ret;
					--ret;
					tmp = tmp->GetPrev();
				}
			}
			throw InvalidOperationException(__FUNCSIG__, "�قȂ�HashMap�̃C�e���[�^�ԂŌv�Z���悤�Ƃ��܂����B", __FILE__, __LINE__);
		}
		constexpr ptrdiff_t operator-(const toggle_iterator_type& rhs) const {
			return operator-(iterator_type{ rhs.m_current });
		}
		constexpr bool operator==(const iterator_type& rhs) const noexcept { return m_current == rhs.m_current; }
		constexpr bool operator==(const toggle_iterator_type& rhs) const noexcept { return m_current == rhs.m_current; }
		constexpr iterator_type& operator=(const iterator_type&) noexcept = default;
	};
}

//argtypes
//one_of_t<i, Args>�͔͈͊O��void�^��Ԃ��ăR���p�C���G���[�������N�������߁A
//�_�~�[�^�𗘗p���ė}������B
//�����ȃC���f�b�N�Xi��static_assert�Œm�点��
namespace System {
	struct ArgDummy {};
	template<size_t i, class T, class ...Args>
	struct argtypes : argtypes<i - 1, Args...> {};
	template<class T, class ...Args>
	struct argtypes<0, T, Args...> { using type = T; };
	template<size_t i, class T>
	struct argtypes<i, T> { using type = ArgDummy; };
}

//HashMap
export namespace System {
	/// <summary>
	/// �L�[�ƒl�̑g�ݍ��킹���Ǘ�����R���e�i�N���X�B
	/// �L�[�̏d���͔F�߂Ȃ����A�m�[�h�ɂ͕����̒l���i�[�ł���
	/// </summary>
	/// <typeparam name="Key">�L�[�^</typeparam>
	/// <typeparam name="...Args">�l�^</typeparam>
	template<class Key, class ...Args>
	class HashMap : public ICollection<HashNode<Key, Args...>> {
		using nodetype = HashNode<Key, Args...>;	//�g�p�����m�[�h�^
		template<size_t i> using argtype = argtypes<i, Args...>::type;	//i�Ԗڂ̒l�^
		template<size_t i> static constexpr bool enable_argtype_v = i < sizeof...(Args);	//i�Ԗڂ̒l�����݂��邩
		using maptype = HashMap<Key, Args...>;	//����HashMap�̌^
		using ckeytype = System::Traits::conditional_t<Traits::is_pointer_v<Key>, const Traits::remove_pointer_t<Traits::remove_cvref_t<Key>>*, const Traits::remove_cvref_t<Key>&>;
	private:
		Vector<nodetype*>* m_nodes = nullptr;	//�m�[�h�̓��I�z��(Vector<HashNode<Key, Args...>>)�̔z��
		nodetype* m_first = nullptr;	//���݂���m�[�h�̒��ň�ԍŏ��ɒǉ����ꂽ�m�[�h
		size_t m_nodesSize = 0;	//m_nodes�̗v�f��
		System::Function<void(HashMap<Key, Args...>&)> m_destructor;	//�L�[��l�̃f�X�g���N�^
		bool m_destructorInitialized = false;	//m_destructor������������Ă��邩
	public:
		HashMap() noexcept : HashMap(10) {}
		HashMap(const HashMap&) noexcept = delete;
		HashMap(HashMap&& arg) noexcept : m_nodes(arg.m_nodes), m_nodesSize(arg.m_nodesSize), m_first(arg.m_first) {
			arg.m_nodes = nullptr;
			arg.m_nodesSize = 0;
			arg.m_first = nullptr;
		}
		/// <summary>
		/// 2�̏搔�Ńn�b�V���l�̖@���w�肵�āA����������
		/// </summary>
		/// <param name="n">2�̏搔�B3�̏ꍇ�A8(2^3)���n�b�V���l�̖@�Ƃ��Ďg�p����</param>
		HashMap(uint8_t n) noexcept : m_nodesSize(1ull << (n > 63 ? 63 : n)) {
			m_nodes = new Vector<nodetype*>[m_nodesSize];
		}
		~HashMap() noexcept {
			if (m_destructorInitialized) m_destructor(*this);
			if (m_nodes) {
				for (size_t i = 0; i < m_nodesSize; ++i) m_nodes[i].DeleteAll();
				delete[] m_nodes;
				m_nodes = nullptr;
			}
			m_first = nullptr;
			m_nodesSize = 0;
		}
	private:
		constexpr size_t GetCode(ckeytype key) const noexcept { return Hash<Key>()(key) & (m_nodesSize - 1); }
		constexpr nodetype* FindNode(ckeytype key) const noexcept {
			size_t index = GetCode(key);
			if (index >= m_nodesSize) return nullptr;
			Vector<nodetype*>& nodes = m_nodes[index];
			for (nodetype* node : nodes) if (node->GetKey() == key) return node;
			return nullptr;
		}
	public:
		constexpr HashMapIterator<nodetype> begin() noexcept {
			return HashMapIterator<nodetype>(m_first);
		}
		constexpr HashMapIterator<const nodetype> begin() const noexcept {
			return HashMapIterator<const nodetype>(m_first);
		}
		constexpr HashMapIterator<nodetype> end() noexcept {
			return HashMapIterator<nodetype>();
		}
		constexpr HashMapIterator<const nodetype> end() const noexcept {
			return HashMapIterator<const nodetype>();
		}
	public:
		/// <summary>
		/// �L�[���܂܂�Ă��邩���ׂ�
		/// </summary>
		/// <param name="key">�L�[</param>
		constexpr bool Contains(ckeytype key) const noexcept { return FindNode(key); }
		constexpr bool Any() const noexcept { return begin() != end(); }
		constexpr size_t Count() const noexcept {
			size_t ret = 0;
			for (size_t i = 0; i < m_nodesSize; ++i) {
				Vector<nodetype*>& nodes = m_nodes[i];
				ret += nodes.Count();
			}
			return ret;
		}
	public:
		/// <summary>
		/// �L�[�ƒl���w�肵��HashMap�ɒǉ�����
		/// </summary>
		/// <param name="key">�L�[�BKey�^��������Key�^�R���X�g���N�^�ɓn����^</param>
		/// <param name="...args">
		/// �l�̃p�����[�^�p�b�N�BArgs�^��������Args�^�R���X�g���N�^�ɓn����^�B
		/// ���я���Args�ƈ�v����
		/// </param>
		/// <returns>�ǉ��ɐ��������Ƃ��Atrue�B���łɑ��݂���L�[���w�肵���ꍇ�Afalse</returns>
		template<Traits::Constructible<Key> K, Traits::Constructible<Args...> ...A>
		bool Insert(K&& key, A&& ...args) noexcept {
			Key tmpkey = Key(static_cast<K&&>(key));
			size_t index = GetCode(tmpkey);
			if (index >= m_nodesSize) return false;	//���ʂ͂��肦�Ȃ�
			Vector<nodetype*>& nodes = m_nodes[index];
			for (nodetype* node : nodes) if (node->GetKey() == tmpkey) return false;
			nodes.Add(new HashNode<Key, Args...>(static_cast<Key&&>(tmpkey), static_cast<A&&>(args)...));
			HashNodeBase<Key>* tmp = nodes.Last();
			HashNodeBase<Key>* last = m_first ? m_first->GetLast() : nullptr;
			if (last) {
				last->SetNext(tmp);
				tmp->SetPrev(last);
			}
			else m_first = static_cast<nodetype*>(tmp);
			return true;
		}
		/// <summary>
		/// �L�[���w�肵��HashMap����f�[�^���폜����
		/// </summary>
		/// <param name="key">�L�[</param>
		/// <returns>�폜�ɐ��������ꍇ�Atrue�B���݂��Ȃ��L�[���w�肵���ꍇ�Afalse</returns>
		bool Remove(ckeytype key) noexcept {
			size_t index = GetCode(key);
			if (index >= m_nodesSize) return false;
			Vector<nodetype*>& nodes = m_nodes[index];
			for (size_t i = 0, count = nodes.Count(); i < count; ++i) {
				nodetype* node = nodes[i];
				if (node->GetKey() == key) {
					HashNodeBase<Key>* prev = node->GetPrev();
					HashNodeBase<Key>* next = node->GetNext();
					if (prev || next) {	//�Ō�̈�ł͂Ȃ�
						if (prev) {
							prev->SetNext(next);
							if (next) next->SetPrev(prev);
						}
						else {
							next->SetPrev(nullptr);
							m_first = static_cast<nodetype*>(next);
						}
					}
					else m_first = nullptr;
					delete node;
					nodes.RemoveByIndex(i);
					return true;
				}
			}
			return false;
		}
		/// <summary>
		/// ���ׂẴm�[�h���������B�֐��Ăяo����̓R���X�g���N�g����̏�ԂƂȂ�
		/// </summary>
		void Clear() noexcept {
			for (size_t i = 0; i < m_nodesSize; ++i) {
				Vector<nodetype*>& nodes = m_nodes[i];
				nodes.DeleteAll();
			}
			m_first = nullptr;
		}
	public:
		/// <summary>
		/// �L�[�ƒl�̃C���f�b�N�X���w�肵�Ēl�ւ̎Q�Ƃ��擾����B
		/// ���̊֐��͒l����|�C���^�^�̏ꍇ�̂ݒ�`�����
		/// </summary>
		/// <param name="key">�L�[</param>
		/// <exception>���݂��Ȃ��L�[���w�肵���ꍇ�AInvalidOperationException�𑗏o����</exception>
		template<size_t i = 0>
		argtype<i>& At(ckeytype key, srcloc loc = srcloc::current()) requires(!System::Traits::is_pointer_v<argtype<i>>) {
			static_assert(enable_argtype_v<i>, "Invalid index specified for HashMap::AtPtr().");
			nodetype* node = FindNode(key);
			if (!node) throw InvalidOperationException(__FUNCSIG__, "���݂��Ȃ��L�[���w�肳��܂����B", loc);
			return node->GetValue<i>();
		}
		/// <summary>
		/// �L�[�ƒl�̃C���f�b�N�X���w�肵�Ēl���擾����B
		/// ���̊֐��͒l���|�C���^�^�̏ꍇ�̂ݒ�`�����
		/// </summary>
		/// <param name="key">�L�[</param>
		template<size_t i = 0>
		argtype<i> At(ckeytype key) const noexcept requires(System::Traits::is_pointer_v<argtype<i>>) {
			static_assert(enable_argtype_v<i>, "Invalid index specified for HashMap::AtPtr().");
			nodetype* node = FindNode(key);
			if (!node) return nullptr;
			return node->GetValue<i>();
		}
		/// <summary>
		/// �L�[�ƒl�̃C���f�b�N�X���w�肵�Ēl�ւ̃|�C���^���擾����
		/// </summary>
		/// <param name="key">�L�[</param>
		template<size_t i = 0>
		argtype<i>* AtPtr(ckeytype key) const noexcept requires(!System::Traits::is_pointer_v<argtype<i>>) {
			static_assert(enable_argtype_v<i>, "Invalid index specified for HashMap::AtPtr().");
			nodetype* node = FindNode(key);
			if (!node) return nullptr;
			return node->GetValuePtr<i>();
		}
	public:
		/// <summary>
		/// �f�X�g���N�^�̍ŏ��ɌĂяo�����֐���ݒ肷��B
		/// Key��Value������K�{�̃|�C���^�̏ꍇ�Ɏg�p����
		/// </summary>
		/// <param name="destructor">���̃C���X�^���X�ւ̎Q�Ƃ������ɂƂ�֐�</param>
		void SetDestructor(const System::Function<void(HashMap<Key, Args...>&)>& destructor) noexcept {
			m_destructor = destructor;
			m_destructorInitialized = true;
		}
		void SetDestructor(System::Function<void(HashMap<Key, Args...>&)>&& destructor) noexcept {
			m_destructor = static_cast<System::Function<void(HashMap<Key, Args...>&)>&&>(destructor);
			m_destructorInitialized = true;
		}
		/// <summary>
		/// �f�X�g���N�^�̍ŏ��ɌĂяo�����֐����ݒ肳��Ă��邩���ׂ�
		/// </summary>
		bool IsInitializedDestructor() const noexcept { return m_destructorInitialized; }
	public://ICollection�I�[�o�[���C�h
		IEnumerator<nodetype> GetEnumerator() noexcept override {
			nodetype* current = m_first;
			while (current) {
				co_yield *current;
				current = static_cast<nodetype*>(current->GetNext());
			}
		}
		IEnumerator<nodetype const> GetEnumerator() const noexcept override {
			nodetype* current = m_first;
			while (current) {
				co_yield *current;
				current = static_cast<nodetype*>(current->GetNext());
			}
		}
		IEnumerator<nodetype> GetReverseEnumerator() noexcept override {
			HashNodeBase<Key>* current = m_first ? m_first->GetLast() : nullptr;
			while (current) {
				co_yield *static_cast<nodetype*>(current);
				current = current->GetPrev();
			}
		}
		IEnumerator<nodetype const> GetReverseEnumerator() const noexcept override {
			HashNodeBase<Key>* current = m_first ? m_first->GetLast() : nullptr;
			while (current) {
				co_yield *static_cast<nodetype*>(current);
				current = current->GetPrev();
			}
		}
	public:
		/// <summary>
		/// �w�肵���C���f�b�N�X�̒l(�Q��)��񋓂���
		/// </summary>
		template<size_t i = 0>
		IEnumerable<argtype<i>> Values() noexcept {
			if constexpr (i == 0)
				return IEnumerable<argtype<i>>(
					new MemberSelectEnumerator<argtype<i>, nodetype>(
						new IEnumerator<nodetype>([this](bool r) { return r ? this->GetReverseEnumerator() : this->GetEnumerator(); }, false),
						&nodetype::m_value
						)
					);
			else {
				static_assert(i <= 0, "Invalid index specified for HashMap::Values().");
			}
		}
		template<size_t i = 0>
		IEnumerable<argtype<i> const> Values() const noexcept {
			if constexpr (i == 0)
				return IEnumerable<argtype<i> const>(
					new MemberSelectEnumerator<argtype<i> const, nodetype const>(
						new IEnumerator<nodetype const>([this](bool r) { return r ? this->GetReverseEnumerator() : this->GetEnumerator(); }, false),
						&nodetype::m_value
						)
					);
			else {
				static_assert(i <= 0, "Invalid index specified for HashMap::Values().");
			}
		}
	public:
		HashMap& operator=(HashMap&& rhs) noexcept {
			if (this == &rhs) return *this;
			Clear();
			delete[] m_nodes;
			m_nodes = rhs.m_nodes;
			m_first = rhs.m_first;
			m_nodesSize = rhs.m_nodesSize;
			rhs.m_nodes = nullptr;
			rhs.m_first = nullptr;
			rhs.m_nodesSize = 0;
			return *this;
		}
	};
}