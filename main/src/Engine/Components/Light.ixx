export module Components:Light;
import :Component;
import System;
using namespace System;
using namespace System::Drawing;

export namespace Engine {
	enum class LightType {
		Directional,
		Point
	};
}

export namespace Engine {
	class Light : public Component {
		LightType m_type = LightType::Directional;
		Color m_color = Colors::White;
	public:
		using Component::Component;
	public:
		LightType GetLightType() const noexcept { return m_type; }
		void SetLightType(LightType type) noexcept { m_type = type; }
		void SetColor(Color color) noexcept { m_color = color; }
		const Color& GetColor() const noexcept { return m_color; }
		Vector3 GetDirection() const noexcept;
	public:
		Type GetType() const noexcept override { return Type::CreateType<Light>(); }
		String ToString() const noexcept override { return String(u"Light Component"); }
		uint32_t GetTypeID() const noexcept override { return System::GetID<Light>(); }
		Light* Clone(GameObject* object) noexcept override;
	};
}
