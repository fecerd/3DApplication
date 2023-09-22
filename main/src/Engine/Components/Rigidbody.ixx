export module Rigidbody;
export import Component;
export import Transform;
import System;
using namespace System;

export namespace Engine {
	template<class GameObject>
	class Rigidbody_Impl : public Component_Impl<GameObject> {
		Vector3 m_velocity = Vector3(0.f, 0.f, 0.f);
	public:
		Rigidbody_Impl() noexcept {}
		Rigidbody_Impl(const Rigidbody_Impl&) noexcept = default;
		Rigidbody_Impl(Rigidbody_Impl&&) noexcept = default;
		Rigidbody_Impl(GameObject* gObj) noexcept : Component_Impl<GameObject>(gObj) {}
		~Rigidbody_Impl() noexcept {}
	public:
		void FixedUpdate() noexcept {
			Transform_Impl<GameObject>& transform = Component_Impl<GameObject>::GetObject().GetTransform();
			transform.LocalPosition += transform.Rotation() * m_velocity;
		}
	public:
		Type GetType() const noexcept override { return Type::CreateType<Rigidbody_Impl<GameObject>>(); }
		String ToString() const noexcept override { return String(u"Rigidbody Component"); }
		uint32_t GetTypeID() const noexcept override { return GetID<Rigidbody_Impl<GameObject>>(); }
		Rigidbody_Impl<GameObject>* Clone(GameObject* object) noexcept override {
			Rigidbody_Impl<GameObject>* ret = new Rigidbody_Impl<GameObject>(object);
			return ret;
		}
	};
}
