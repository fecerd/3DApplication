module Common3D;
import System;
import Application;
import Common3DInterface;
import Common3DUtilities;
import NamespaceHelper;

#define USE_DirectX
#if defined(USE_DirectX)
import DirectX;

System::ExclusiveObject<System::Application::Windows::DirectX::DirectXManager> GetManager() {
	return System::Application::Windows::DirectX::DirectXManager::GetManager();
}
bool ReleaseManager() {
	return System::Application::Windows::DirectX::DirectXManager::CloseManager();
}

#else
#endif

namespace System::Application::Common3D {
	bool Init() noexcept {
		try {
			GetManager();
		}
		catch (std_exception&) {
			return false;
		}
		return true;
	}
	bool Release3DManager() noexcept { return ReleaseManager(); }
	RenderTarget CreateRenderTarget(const String& name, uint32_t width, uint32_t height, uint32_t bufferCount, uint32_t targetCount, const Drawing::Color& defaultColor) noexcept {
		return RenderTarget(name, GetManager()->CreateRenderTarget(width, height, bufferCount, targetCount, defaultColor));
	}
	RenderTarget CreateRenderTarget(const String& name, IWindow& window, uint32_t bufferCount, const Drawing::Color& defaultColor) noexcept {
		return RenderTarget(name, GetManager()->CreateRenderTarget(window, bufferCount, defaultColor));
	}
	Renderer CreateRenderer(const String& name, const RendererDesc& desc) noexcept {
		return Renderer(GetManager()->CreateRenderer(name, desc));
	}
	Resource CreateResource(const String& name, const float* data, size_t count) noexcept {
		return Resource(name, GetManager()->CreateResource(data, count));
	}
	Resource CreateResource(const String& name, const Matrix* data, size_t count) noexcept {
		return Resource(name, GetManager()->CreateResource(data, count));
	}
	Resource CreateResource(const String& name, const Drawing::Image& image) noexcept {
		return Resource(name, GetManager()->CreateResource(image));
	}
	MeshResource CreateMeshResource(const String& name, const MeshDesc& desc) noexcept {
		return MeshResource(name, GetManager()->CreateMeshResource(desc));
	}
	Heap CreateHeap(const String& name, HeapType type, uint32_t setCount) noexcept {
		return Heap(name, GetManager()->CreateHeap(type, setCount));
	}
	CommandList CreateCommandList(const String& name, uint32_t materialCount) noexcept {
		return CommandList(name, GetManager()->CreateCommandList(materialCount));
	}
}
