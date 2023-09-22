export module Components:Camera;
import :Component;
import :Declaration;
import System;
import LayerMask;
import Common3D;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;
using namespace Engine;

export namespace Engine { class Camera; }

export namespace Engine::Internal {
	enum class CameraFlag : uint8_t {
		UseFovY = 0,
		UseFovX = 2,
		UseFrustum = 1,
		InWorldSpace = 0,
		OnScreenSpace = 4,
	};
}
using namespace Engine::Internal;

export class Engine::Camera : public Component {
	float m_zNear = 0.f;
	float m_zFar = 1.f;
	FlagSet<CameraFlag> m_flag = { CameraFlag::UseFovY, CameraFlag::InWorldSpace };
	union {
		struct {
			float m_aspectHW;
			union {
				float x;
				float y;
			} m_fov;
		};
		struct {
			float top;
			float left;
			float right;
			float bottom;
		} m_frustum;
	};
	FlagSet<LayerMask> m_cullingMask = LayerMask::All;
	RenderTarget m_renderTarget;
	Resource m_cameraResource;
	Heap m_cameraHeap;
	bool m_useWindowRendering = true;
	uint8_t m_renderingOrder = 128;
public:
	EventHandler<Camera&> PostDraw;
public:
	using Component::Component;
public:
	void SetNearFar(float zNear, float zFar) noexcept {
		m_zNear = zNear;
		m_zFar = zFar;
	}
	float GetNear() const noexcept { return m_zNear; }
	float GetFar() const noexcept { return m_zFar; }
	void SetFovX(float aspectHW, float fovX) noexcept {
		m_aspectHW = aspectHW;
		m_fov.x = fovX;
		m_flag = { CameraFlag::UseFovX, CameraFlag::InWorldSpace };
	}
	void SetFovY(float aspectHW, float fovY) noexcept {
		m_aspectHW = aspectHW;
		m_fov.y = fovY;
		m_flag = { CameraFlag::UseFovY, CameraFlag::InWorldSpace };
	}
	void SetFrustum(const Point<float>& topleft, const Point<float>& bottomright) noexcept {
		m_frustum.top = topleft.y;
		m_frustum.left = topleft.x;
		m_frustum.right = bottomright.x;
		m_frustum.bottom = bottomright.y;
		m_flag = { CameraFlag::UseFrustum, CameraFlag::InWorldSpace };
	}
	void SetFrustum(float top, float left, float bottom, float right) noexcept {
		SetFrustum(Point<float>(left, top), Point<float>(right, bottom));
	}
	void SetScreenAspect(float aspectHW) noexcept {
		m_aspectHW = aspectHW;
		m_flag = CameraFlag::OnScreenSpace;
	}
	float GetAspectHW() const noexcept {
		if (m_flag & CameraFlag::UseFrustum) {
			return static_cast<float>(m_frustum.top - m_frustum.bottom)
				/ static_cast<float>(m_frustum.right - m_frustum.left);
		}
		else return m_aspectHW;
	}
	float GetFovX() const noexcept {
		if (m_flag & CameraFlag::UseFovX) return m_fov.x;
		else if (m_flag & CameraFlag::OnScreenSpace) return 0.f;
		else if (m_flag & CameraFlag::UseFrustum) {
			Vector3 v1 = Vector3(m_frustum.left, (m_frustum.top - m_frustum.bottom) * 0.5f, m_zNear);
			Vector3 v2 = Vector3(m_frustum.right, (m_frustum.top - m_frustum.bottom) * 0.5f, m_zNear);
			float ret = Vector3::Angle(v1, v2);
			return ret >= 0.f ? ret : -ret;
		}
		else {
			//fovx使用時には、w = tan(fovx/2) * nearZ
			//fovy使用時には、w = tan(fovy/2) * nearZ / aspect
			return Math::AtanDeg(Math::TanDeg(m_fov.y * 0.5f) / m_aspectHW) * 2.f;
		}
	}
	float GetFovY() const noexcept {
		if (m_flag & CameraFlag::OnScreenSpace) return 0.f;
		if (m_flag & CameraFlag::UseFrustum) {
			Vector3 v1 = Vector3(m_frustum.left, (m_frustum.top - m_frustum.bottom) * 0.5f, m_zNear);
			Vector3 v2 = Vector3(m_frustum.right, (m_frustum.top - m_frustum.bottom) * 0.5f, m_zNear);
			float ret = Vector3::Angle(v1, v2);
			return ret >= 0.f ? ret : -ret;
		}
		else if (m_flag & CameraFlag::UseFovX) {
			//fovx使用時には、h = tan(fovx/2) * nearZ * aspect
			//fovy使用時には、h = tan(fovy/2) * nearZ
			return Math::AtanDeg(Math::TanDeg(m_fov.x * 0.5f) * m_aspectHW) * 2.f;
		}
		else return m_fov.y;
	}
	bool SetRenderTarget(uint32_t width, uint32_t height, uint32_t targetCount = 1) noexcept;
	void SetWindowRendering(bool useWindowRendering) noexcept { m_useWindowRendering = useWindowRendering; }
	bool IsUsedWindowRendering() const noexcept { return m_useWindowRendering; }
	void SetRenderingOrder(uint8_t order) noexcept { m_renderingOrder = order; }
	uint8_t GetRenderingOrder() const noexcept { return m_renderingOrder; }
public:
	void AddCullingMask(LayerMask mask) noexcept { m_cullingMask |= mask; }
	void SetCullingMask(const FlagSet<LayerMask>& mask) noexcept { m_cullingMask = mask; }
	const FlagSet<LayerMask>& GetCullingMask() const noexcept { return m_cullingMask; }
public:
	Vector3 Eye() const noexcept;
	Matrix View() const noexcept;
	Matrix Projection() const noexcept;
public:
	RenderTarget GetRenderTarget() const noexcept { return m_renderTarget; }
	Heap GetHeap() const noexcept { return m_cameraHeap; }
	void UpdateHeap() noexcept {
		if (!m_cameraHeap) m_cameraHeap = CreateHeap(String::Empty(), HeapType::Camera);
		if (!m_cameraHeap) return;
		Common3D::CameraResourceDesc desc{};
		desc.view = View();
		desc.projection = Projection();
		desc.eye = Eye();
		if (!m_cameraResource) {
			m_cameraResource = CreateResource(String::Empty(), desc.GetData(), desc.GetCount());
			m_cameraHeap.SetView(0, ViewFormat::CBV, m_cameraResource);
		}
		else m_cameraResource.UpdateResource(desc.GetData(), desc.GetCount());
	}
	Resource GetTexture(uint8_t targetIndex = 0) const noexcept { return m_renderTarget.GetTexture(targetIndex); }
public:
	Type GetType() const noexcept override { return Type::CreateType<Camera>(); }
	String ToString() const noexcept override { return String(u"Camera Component"); }
	uint32_t GetTypeID() const noexcept override { return GetID<Camera>(); }
	Camera* Clone(GameObject* object) noexcept override;
};
