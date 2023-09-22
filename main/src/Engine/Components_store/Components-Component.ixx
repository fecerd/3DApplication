export module Components:Component;
export import System;
import :Declaration;
using namespace System;

export namespace Engine {
	class Component {
	public:
		//g++に必要
		using virtual_delete_type = Component;
	protected:
		GameObject* m_object = nullptr;
	public:
		Component() noexcept {}
		Component(GameObject* object) noexcept : m_object(object) {}
		virtual ~Component() noexcept {}
	public:
		GameObject& GetObject() const noexcept { return *m_object; }
		UIBase& GetUIBase() const;
	public:
		virtual Type GetType() const noexcept = 0;
		virtual String ToString() const noexcept { return String(u"Undefined Component"); }
		virtual uint32_t GetTypeID() const noexcept = 0;
		virtual Component* Clone(GameObject* object) noexcept = 0;
	};
}