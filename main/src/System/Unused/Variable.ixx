export module Variable;
import Exception;

export namespace System {
	class Variable {
		using destructor_type = void(*)(void*&) noexcept;
		void* m_data = nullptr;
		destructor_type m_destructor = nullptr;
	public:
		Variable() noexcept = default;
		Variable(const Variable&) noexcept = default;
		Variable(Variable&& arg) noexcept : m_data(arg.m_data) {
			arg.m_data = nullptr;
		}
		template<class T>
		Variable(const T& value) noexcept {
			Set(value);
		}
		~Variable() noexcept {
			Clear();
		}
		template<class T>
		static void Destruct(void*& ptr) noexcept {
			delete static_cast<T*>(ptr);
			ptr = nullptr;
		}
	public:
		template<class T>
		void Set(const T& value) noexcept {
			if (m_destructor) m_destructor(m_data);
			m_data = new T(value);
			m_destructor = Destruct<T>;
		}
		template<class T>
		T& Get() {
			if (!m_data) throw InvalidOperationException(__FUNCSIG__, u"Variable�^������������Ă��܂���B", __FILE__, __LINE__);
			if (m_destructor != Destruct<T>) throw InvalidOperationException(__FUNCSIG__, u"Variable::Get()�������l�ƈقȂ�^��v�����܂����B", __FILE__, __LINE__);
			return *static_cast<T*>(m_data);
		}
		void Clear() noexcept {
			if (m_destructor) m_destructor(m_data);
			m_destructor = nullptr;
		}
	};
}
