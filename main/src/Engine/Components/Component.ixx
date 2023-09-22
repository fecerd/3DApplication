export module Component;
export import System;
using namespace System;

export namespace Engine {
	template<class GameObject>
	class Component_Impl {
	public:
		//g++に必要
		using virtual_delete_type = Component_Impl<GameObject>;
	protected:
		GameObject* m_object = nullptr;
	public:
		Component_Impl() noexcept {}
		Component_Impl(const Component_Impl&) noexcept = default;
		Component_Impl(Component_Impl&& arg) noexcept : m_object(arg.m_object) { arg.m_object = nullptr; }
		Component_Impl(GameObject* object) noexcept : m_object(object) {}
		virtual ~Component_Impl() noexcept {}
	public:
		GameObject& GetObject() const noexcept { return *m_object; }
	public:
		virtual Type GetType() const noexcept = 0;
		virtual String ToString() const noexcept { return String(u"Undefined Component"); }
		virtual uint32_t GetTypeID() const noexcept = 0;
		virtual Component_Impl* Clone(GameObject* object) noexcept = 0;
	public:
		Component_Impl& operator=(const Component_Impl&) noexcept = default;
		Component_Impl& operator=(Component_Impl&& rhs) noexcept {
			if (this == &rhs) return *this;
			m_object = rhs.m_object;
			rhs.m_object = nullptr;
			return *this;
		}
	};
}
