#if defined(__GNUC__) //&& !defined(__clang__)
module Components;
import :GameObject;
#else
module Components:Rigidbody;
import Components;
#endif
import System;
using namespace System;
using namespace Engine;

void Rigidbody::FixedUpdate() noexcept {
	GameObject& gObj = GetObject();
	Transform& transform = gObj.GetTransform();
	transform.LocalPosition += transform.Rotation() * m_velocity;
}

Rigidbody* Rigidbody::Clone(GameObject* object) noexcept {
	Rigidbody* ret = new Rigidbody(object);
	return ret;
}
