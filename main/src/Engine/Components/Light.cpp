module Components:Light;
import :GameObject;
using namespace System;
using namespace System::Drawing;
using namespace Engine;

Vector3 Light::GetDirection() const noexcept {
	return this->GetObject().GetTransform().Forward();
}

Light* Light::Clone(GameObject* object) noexcept {
	Light* ret = new Light(object);
	ret->m_type = m_type;
	ret->m_color = m_color;
	return ret;
}
