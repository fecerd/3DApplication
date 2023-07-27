export module Components:Component;
export import System;
import :Declaration;
using namespace System;

export namespace Engine {
	class Component {
	protected:
		GameObject* m_object = nullptr;
	public:
		Component() noexcept = default;
		Component(GameObject* object) noexcept : m_object(object) {}
		virtual ~Component() noexcept = default;
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