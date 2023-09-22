export module Light;
export import Component;
import System;
using namespace System;

//LightType
export namespace Engine {
	/// @brief ライトの種類
	enum class LightType {
		/// @brief 平行光線
		Directional,
		/// @brief ポイントライト
		Point
	};
}

//Light
export namespace Engine {
	template<class GameObject>
	class Light_Impl : public Component_Impl<GameObject> {
		LightType m_type = LightType::Directional;
		Drawing::Color m_color = Drawing::Colors::White;
	public:
		Light_Impl() noexcept {}
		Light_Impl(const Light_Impl&) noexcept = default;
		Light_Impl(Light_Impl&&) noexcept = default;
		Light_Impl(GameObject* gObj) noexcept : Component_Impl<GameObject>(gObj) {}
		~Light_Impl() noexcept {}
	public:
		LightType GetLightType() const noexcept { return m_type; }
		void SetLightType(LightType type) noexcept { m_type = type; }
		void SetColor(Drawing::Color color) noexcept { m_color = color; }
		const Drawing::Color& GetColor() const noexcept { return m_color; }
		Vector3 GetDirection() const noexcept {
			return Component_Impl<GameObject>::GetObject().GetTransform().Forward();
		}
	public:
		Type GetType() const noexcept override { return Type::CreateType<Light_Impl<GameObject>>(); }
		String ToString() const noexcept override { return String(u"Light Component"); }
		uint32_t GetTypeID() const noexcept override { return GetID<Light_Impl<GameObject>>(); }
		Light_Impl<GameObject>* Clone(GameObject* object) noexcept override {
			Light_Impl<GameObject>* ret = new Light_Impl<GameObject>(object);
			ret->m_type = m_type;
			ret->m_color = m_color;
			return ret;
		}
	};
}
