module Common3D;
using namespace System;
using namespace System::Drawing;

//#define DirectX
//#ifdef DirectX
import DirectX;
using System::Application::Windows::DirectX::DirectXManager;
DirectXManager& GetManager() {
	return DirectXManager::GetManager();
}
//#else

//#endif

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
	RenderTarget CreateRenderTarget(const String& name, uint32_t width, uint32_t height, uint32_t bufferCount, uint32_t targetCount, const Color& defaultColor) noexcept {
		return RenderTarget(name, GetManager().CreateRenderTarget(width, height, bufferCount, targetCount, defaultColor));
	}
	RenderTarget CreateRenderTarget(const String& name, IWindow& window, uint32_t bufferCount, const Color& defaultColor) noexcept {
		return RenderTarget(name, GetManager().CreateRenderTarget(window, bufferCount, defaultColor));
	}
	Renderer CreateRenderer(const String& name, const RendererDesc& desc) noexcept {
		return Renderer(GetManager().CreateRenderer(name, desc));
	}
	Resource CreateResource(const String& name, const float* data, size_t count) noexcept {
		return Resource(name, GetManager().CreateResource(data, count));
	}
	Resource CreateResource(const String& name, const Matrix* data, size_t count) noexcept {
		return Resource(name, GetManager().CreateResource(data, count));
	}
	Resource CreateResource(const String& name, const Image& image) noexcept {
		return Resource(name, GetManager().CreateResource(image));
	}
	MeshResource CreateMeshResource(const String& name, const MeshDesc& desc) noexcept {
		return MeshResource(name, GetManager().CreateMeshResource(desc));
	}
	Heap CreateHeap(const String& name, HeapType type, uint32_t setCount) noexcept {
		return Heap(name, GetManager().CreateHeap(type, setCount));
	}
	CommandList CreateCommandList(const String& name, uint32_t materialCount) noexcept {
		return CommandList(name, GetManager().CreateCommandList(materialCount));
	}
}