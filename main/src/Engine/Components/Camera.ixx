export module Camera;
export import Component;
import System;
import LayerMask;
import Common3D;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;

//Camera Flags/Structure
namespace Engine {
	/// @brief カメラが使用する情報を表すフラグ
	enum class CameraUseInfoFlags : uint8_t {
		/// @brief 垂直視野角を使用する
		UseFovY = 0,
		/// @brief 水平視野角を使用する
		UseFovX = 1,
		/// @brief 視錘台を使用する
		UseFrustum = 2,
		/// @brief アスペクト比を使用する
		UseAspectHW = 4
	};

	/// @brief カメラが置かれる空間情報を表すフラグ
	enum class CameraSpaceFlags : uint8_t {
		/// @brief ワールド空間に置かれた3Dカメラ
		InWorldSpace = 0,
		/// @brief UIなどスクリーン空間を移すためのカメラ
		OnScreenSpace = 1,
	};

	/// @brief 視野角情報構造体
	struct FovInfo {
		/// @brief 視野角(度数法)
		float degree;
		/// @brief アスペクト比(高さ/幅)
		float aspectHW;
	};

	/// @brief 視錘台情報構造体(最近面の四隅)
	struct FrustumInfo {
		float top;
		float left;
		float right;
		float bottom;
	};

	/*
		視野角と最近/最遠面の変換
			距離をzとすると、
			水平視野角:
				幅	:	w = tan(fovx/2) * z
				高さ:	h = tan(fovx/2) * z * aspectHW

			垂直視野角:
				幅	:	w = w = tan(fovy/2) * z / aspectHW
				高さ:	h = tan(fovy/2) * z
	*/
}

//Camera
export namespace Engine {
	template<class GameObject>
	class Camera_Impl : public Component_Impl<GameObject> {
		/// @brief 最近面までの距離
		float m_zNear = 0.f;
		/// @brief 最遠面までの距離
		float m_zFar = 1.f;
		/// @brief カメラの映す範囲を表すためにどの構造体を使用するかを表すフラグ
		CameraUseInfoFlags m_infoFlag = CameraUseInfoFlags::UseFovY;
		/// @brief カメラがどの空間に置かれているか表すフラグ
		CameraSpaceFlags m_spaceFlag = CameraSpaceFlags::InWorldSpace;
		union {
			/// @brief 視野角(fovX, fovY)を使用する際に参照する構造体
			FovInfo m_fov = FovInfo{ 90.f, 1080.f / 1920.f };
			/// @brief 視錘台を使用する際に参照する構造体
			FrustumInfo m_frustum;
			/// @brief アスペクト比
			float m_aspectHW;
		};
	private:/* Common3Dで使用される変数 */
		/// @brief カメラが映すレイヤーを表す構造体
		FlagSet<LayerMask> m_cullingMask = LayerMask::All;
		/// @brief カメラの持つレンダーターゲット
		RenderTarget m_renderTarget;
		/// @brief シェーダに渡されるカメラ情報リソース(view, proj行列など)
		Resource m_cameraResource;
		/// @brief カメラ情報リソースへのDescriptorを配置するヒープ
		Heap m_cameraHeap;
		/// @brief このカメラのレンダーターゲットがウィンドウ上に反映されるか
		bool m_useWindowRendering = true;
		/// @brief このカメラのレンダーターゲットが反映される順序(低いほど奥に描画される)
		uint8_t m_renderingOrder = 128;
	public:
		/// @brief このカメラのレンダーターゲットへの描画後に呼び出されるイベント
		EventHandler<Camera_Impl<GameObject>&> PostDraw;
	public:
		Camera_Impl() noexcept {}
		Camera_Impl(const Camera_Impl&) noexcept = default;
		Camera_Impl(Camera_Impl&&) noexcept = default;
		Camera_Impl(GameObject* gObj) noexcept : Component_Impl<GameObject>(gObj) {}
		virtual ~Camera_Impl() noexcept {}
	public:
		void SetNearFar(float zNear, float zFar) noexcept {
			m_zNear = zNear;
			m_zFar = zFar;
		}
		float GetNear() const noexcept { return m_zNear; }
		float GetFar() const noexcept { return m_zFar; }
		void SetFovX(float aspectHW, float fovX) noexcept {
			m_fov.aspectHW = aspectHW;
			m_fov.degree = fovX;
			m_infoFlag = CameraUseInfoFlags::UseFovX;
			m_spaceFlag = CameraSpaceFlags::InWorldSpace;
		}
		void SetFovY(float aspectHW, float fovY) noexcept {
			m_fov.aspectHW = aspectHW;
			m_fov.degree = fovY;
			m_infoFlag = CameraUseInfoFlags::UseFovY;
			m_spaceFlag = CameraSpaceFlags::InWorldSpace;
		}
		void SetFrustum(const Point<float>& topleft, const Point<float>& bottomright) noexcept {
			m_frustum.top = topleft.y;
			m_frustum.left = topleft.x;
			m_frustum.right = bottomright.x;
			m_frustum.bottom = bottomright.y;
			m_infoFlag = CameraUseInfoFlags::UseFrustum;
			m_spaceFlag = CameraSpaceFlags::InWorldSpace;
		}
		void SetFrustum(float top, float left, float bottom, float right) noexcept {
			SetFrustum(Point<float>(left, top), Point<float>(right, bottom));
		}
		void SetScreenAspect(float aspectHW) noexcept {
			m_aspectHW = aspectHW;
			m_infoFlag = CameraUseInfoFlags::UseAspectHW;
			m_spaceFlag = CameraSpaceFlags::OnScreenSpace;
		}
		float GetAspectHW() const noexcept {
			switch (m_infoFlag) {
			case CameraUseInfoFlags::UseFrustum:
				return static_cast<float>(m_frustum.top - m_frustum.bottom)
					/ static_cast<float>(m_frustum.right - m_frustum.left);
			case CameraUseInfoFlags::UseAspectHW:
				return m_aspectHW;
			case CameraUseInfoFlags::UseFovX:
			case CameraUseInfoFlags::UseFovY:
			default:
				return m_fov.aspectHW;
			}
		}
		float GetFovX() const noexcept {
			switch (m_infoFlag) {
			case CameraUseInfoFlags::UseFovX:
				return m_fov.degree;
			case CameraUseInfoFlags::UseFovY:
				return Math::AtanDeg(Math::TanDeg(m_fov.degree * 0.5f) / m_fov.aspectHW) * 2.f;
			case CameraUseInfoFlags::UseFrustum: {
				Vector3 v1 = Vector3(m_frustum.left, (m_frustum.top - m_frustum.bottom) * 0.5f, m_zNear);
				Vector3 v2 = Vector3(m_frustum.right, (m_frustum.top - m_frustum.bottom) * 0.5f, m_zNear);
				float ret = Vector3::Angle(v1, v2);
				return ret >= 0.f ? ret : -ret;
			}
			case CameraUseInfoFlags::UseAspectHW:
			default:
				return 0.f;
			}
		}
		float GetFovY() const noexcept {
			switch (m_infoFlag) {
			case CameraUseInfoFlags::UseFovX:
				return Math::AtanDeg(Math::TanDeg(m_fov.degree * 0.5f) * m_fov.aspectHW) * 2.f;
			case CameraUseInfoFlags::UseFovY:
				return m_fov.degree;
			case CameraUseInfoFlags::UseFrustum: {
				Vector3 v1 = Vector3(m_frustum.left, (m_frustum.top - m_frustum.bottom) * 0.5f, m_zNear);
				Vector3 v2 = Vector3(m_frustum.right, (m_frustum.top - m_frustum.bottom) * 0.5f, m_zNear);
				float ret = Vector3::Angle(v1, v2);
				return ret >= 0.f ? ret : -ret;
			}
			case CameraUseInfoFlags::UseAspectHW:
			default:
				return 0.f;
			}
		}
		void AddCullingMask(LayerMask mask) noexcept { m_cullingMask |= mask; }
		void SetCullingMask(const FlagSet<LayerMask>& mask) noexcept { m_cullingMask = mask; }
		const FlagSet<LayerMask>& GetCullingMask() const noexcept { return m_cullingMask; }
	public:
		bool SetRenderTarget(uint32_t width, uint32_t height, uint32_t targetCount = 1) noexcept {
			bool result = false;
			if (m_renderTarget && m_renderTarget.GetTargetCount() == targetCount) {
				Size<uint32_t> size = m_renderTarget.GetSize();
				if (size.width != width || size.height != height) result = m_renderTarget.ResizeBuffer(width, height);
			}
			else {
				m_renderTarget = CreateRenderTarget(String::Empty(), width, height, 2, targetCount);
				result = static_cast<bool>(m_renderTarget);
			}
			return result;
		}
		void SetWindowRendering(bool useWindowRendering) noexcept { m_useWindowRendering = useWindowRendering; }
		bool IsUsedWindowRendering() const noexcept { return m_useWindowRendering; }
		void SetRenderingOrder(uint8_t order) noexcept { m_renderingOrder = order; }
		uint8_t GetRenderingOrder() const noexcept { return m_renderingOrder; }
	public:
		/// @brief カメラのワールド空間上の位置を取得する
		Vector3 Eye() const noexcept { return Component_Impl<GameObject>::GetObject().GetTransform().Position(); }
		/// @brief ワールド空間->カメラ空間への変換行列を取得する
		Matrix View() const noexcept {
			if (m_spaceFlag == CameraSpaceFlags::OnScreenSpace) return Matrix::Identity();
			return Component_Impl<GameObject>::GetObject().GetTransform().WorldInverse();
		}
		/// @brief カメラ空間->クリッピング空間への透視投影変換行列を取得する
		Matrix Projection() const noexcept {
			if (m_spaceFlag == CameraSpaceFlags::OnScreenSpace) return Matrix::Identity();
			const float f_n = m_zFar - m_zNear;
			switch (m_infoFlag){
			case CameraUseInfoFlags::UseFrustum: {
				const float w = m_frustum.right - m_frustum.left;
				const float h = m_frustum.top - m_frustum.bottom;
				return Matrix{
					m_zNear * 2.f / w, 0, -(m_frustum.right + m_frustum.left) / w, 0,
					0, m_zNear * 2.f / h, -(m_frustum.top + m_frustum.bottom) / h, 0,
					0, 0, m_zFar / f_n, -m_zFar * m_zNear / f_n,
					0, 0, 1, 0
				};
			}
			case CameraUseInfoFlags::UseFovX: {
				const float cot = 1.f / Math::TanDeg(m_fov.degree * 0.5f);
				return Matrix{
					cot, 0, 0, 0,
					0, cot / m_fov.aspectHW, 0, 0,
					0, 0, m_zFar / f_n, -m_zFar * m_zNear / f_n,
					0, 0, 1, 0
				};				
			}
			case CameraUseInfoFlags::UseFovY: {
				float cot = 1.f / Math::TanDeg(m_fov.degree * 0.5f);
				return Matrix{
					cot * m_fov.aspectHW, 0, 0, 0,
					0, cot, 0, 0,
					0, 0, m_zFar / f_n, -m_zFar * m_zNear / f_n,
					0, 0, 1, 0
				};				
			}
			default:
				return Matrix::Identity();
			}
		}
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
	public:/* Component override */
		Type GetType() const noexcept override { return Type::CreateType<Camera_Impl<GameObject>>(); }
		String ToString() const noexcept override { return String(u"Camera Component"); }
		uint32_t GetTypeID() const noexcept override { return GetID<Camera_Impl<GameObject>>(); }
		/// @brief カメラ情報をコピーし、新たなレンダーターゲットを持つCameraを作成する。PostDrawイベントはコピーされない。
		/// @param object 作成されるCameraが参照するGameObject
		/// @return newで作成されたCamera
		Camera_Impl* Clone(GameObject* object) noexcept override {
			Camera_Impl<GameObject>* ret = new Camera_Impl<GameObject>(object);
			ret->SetNearFar(m_zNear, m_zFar);
			switch (m_infoFlag) {
			case CameraUseInfoFlags::UseFovX:
				ret->SetFovX(m_fov.aspectHW, m_fov.degree);
				break;
			case CameraUseInfoFlags::UseFovY:
				ret->SetFovY(m_fov.aspectHW, m_fov.degree);
				break;
			case CameraUseInfoFlags::UseFrustum:
				ret->SetFrustum(m_frustum.top, m_frustum.left, m_frustum.bottom, m_frustum.right);
				break;
			case CameraUseInfoFlags::UseAspectHW:
				ret->SetScreenAspect(m_aspectHW);
				break;
			}
			ret->SetCullingMask(m_cullingMask);
			const Size<uint32_t> size = m_renderTarget.GetSize();
			ret->SetRenderTarget(size.width, size.height, m_renderTarget.GetTargetCount());
			return ret;
		}
	public:
		Camera_Impl& operator=(const Camera_Impl&) noexcept = delete;
		Camera_Impl& operator=(Camera_Impl&& rhs) noexcept {
			if (this == &rhs) return *this;
			m_zNear = rhs.m_zNear;
			m_zFar = rhs.m_zFar;
			m_infoFlag = rhs.m_infoFlag;
			m_spaceFlag = rhs.m_spaceFlag;
			switch (m_infoFlag) {
			case CameraUseInfoFlags::UseFovX:
			case CameraUseInfoFlags::UseFovY:
				m_fov.degree = rhs.m_fov.degree;
				m_fov.aspectHW = rhs.m_fov.aspectHW;
				break;
			case CameraUseInfoFlags::UseFrustum:
				m_frustum = rhs.m_frustum;
				break;
			case CameraUseInfoFlags::UseAspectHW:
				m_aspectHW = rhs.m_aspectHW;
				break;
			}
			m_cullingMask = rhs.m_cullingMask;
			m_renderTarget = System::move(rhs.m_renderTarget);
			m_cameraResource = System::move(rhs.m_cameraResource);
			m_cameraHeap = System::move(rhs.m_cameraHeap);
			m_useWindowRendering = rhs.m_useWindowRendering;
			m_renderingOrder = rhs.m_renderingOrder;
			PostDraw = System::move(rhs.PostDraw);
			return *this;
		}
	};
}
