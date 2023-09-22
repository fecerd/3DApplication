#if defined(__GNUC__) //&& !defined(__clang__)
module Components;
import :GameObject;
#else
module Components:Camera;
import Components;
#endif
import System;
import LayerMask;
import Common3D;
using namespace System;
using namespace System::Application::Common3D;
using namespace Engine;
using namespace Engine::Internal;

bool Camera::SetRenderTarget(uint32_t width, uint32_t height, uint32_t targetCount) noexcept {
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

Vector3 Camera::Eye() const noexcept {
	return GetObject().GetTransform().Position();
}

Matrix Camera::View() const noexcept {
	if (m_flag & CameraFlag::OnScreenSpace) return Matrix::Identity();
	const Transform& cameraTr = GetObject().GetTransform();
	return cameraTr.WorldInverse();
}

Matrix Camera::Projection() const noexcept {
	if (m_flag & CameraFlag::OnScreenSpace) return Matrix::Identity();
	const float f_n = m_zFar - m_zNear;
	if (m_flag & CameraFlag::UseFrustum) {
		float w = static_cast<float>(m_frustum.right - m_frustum.left);
		float h = static_cast<float>(m_frustum.top - m_frustum.bottom);
		return Matrix{
			m_zNear * 2.f / w, 0, -static_cast<float>(m_frustum.right + m_frustum.left) / w, 0,
			0, m_zNear * 2.f / h, -static_cast<float>(m_frustum.top + m_frustum.bottom) / h, 0,
			0, 0, m_zFar / f_n, -m_zFar * m_zNear / f_n,
			0, 0, 1, 0
		};
	}
	else if (m_flag & CameraFlag::UseFovX) {
		float cot = 1.f / Math::TanDeg(m_fov.x * 0.5f);
		return Matrix{
			cot, 0, 0, 0,
			0, cot / m_aspectHW, 0, 0,
			0, 0, m_zFar / f_n, -m_zFar * m_zNear / f_n,
			0, 0, 1, 0
		};
	}
	else {
		float cot = 1.f / Math::TanDeg(m_fov.y * 0.5f);
		return Matrix{
			cot * m_aspectHW, 0, 0, 0,
			0, cot, 0, 0,
			0, 0, m_zFar / f_n, -m_zFar * m_zNear / f_n,
			0, 0, 1, 0
		};
	}
}

Camera* Camera::Clone(GameObject* object) noexcept {
	Camera* ret = new Camera(object);
	ret->SetNearFar(m_zNear, m_zFar);
	if (m_flag & CameraFlag::OnScreenSpace) {
		ret->SetScreenAspect(m_aspectHW);
	}
	else if (m_flag & CameraFlag::UseFrustum) {
		ret->SetFrustum(m_frustum.top, m_frustum.left, m_frustum.bottom, m_frustum.right);
	}
	else if (m_flag & CameraFlag::UseFovX) {
		ret->SetFovX(m_aspectHW, m_fov.x);
	}
	else {
		ret->SetFovY(m_aspectHW, m_fov.y);
	}
	ret->SetCullingMask(m_cullingMask);
	Size<uint32_t> size = m_renderTarget.GetSize();
	ret->SetRenderTarget(size.width, size.height, m_renderTarget.GetTargetCount());
	return ret;
}
