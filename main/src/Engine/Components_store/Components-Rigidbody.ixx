export module Components:Rigidbody;
import :Component;
import :Declaration;
import System;
using namespace System;

export namespace Engine { class Rigidbody; }

export class Engine::Rigidbody : public Engine::Component {
	Vector3 m_velocity = Vector3(0.f, 0.f, 0.f);
public:
	using Component::Component;
public:
	void FixedUpdate() noexcept;
public:
	Type GetType() const noexcept override { return Type::CreateType<Rigidbody>(); }
	String ToString() const noexcept override { return String(u"Rigidbody Component"); }
	uint32_t GetTypeID() const noexcept override { return GetID<Rigidbody>(); }
	Rigidbody* Clone(GameObject* object) noexcept override;
};
