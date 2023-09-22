export module Common3D;
export import System;
import ManagedObject;
import Application;
export import Common3DInterface;
export import Common3DUtilities;
using namespace System::Drawing;

//Resource
export namespace System::Application::Common3D {
	class Resource : public ManagedObject<IResource> {
		using basetype = ManagedObject<IResource>;
	public:
		using basetype::ManagedObject;
		Resource() noexcept {}
		Resource(const basetype& arg) noexcept : basetype(arg) {}
		~Resource() noexcept {}
	public:
		void SetDebugName(const String& name) noexcept {
			if (*this) (*(*this)).SetName(name);
		}
		const String& GetName() const noexcept {
			return basetype::GetName();
		}
		ResourceFormat GetFormat() const noexcept {
			if (*this) return (*(*this)).GetFormat();
			else return ResourceFormat::Error;
		}
		Size<uint32_t> GetSize() const noexcept {
			if (*this) return (*(*this)).GetSize();
			else return Size<uint32_t>{0, 0};
		}
		size_t GetCount() const noexcept {
			if (*this) return (*(*this)).GetCount();
			else return 0;
		}
	public:
		void Next() noexcept {
			if (*this) (*(*this)).Next();
		}
	public:
		bool UpdateResource(const float* data, size_t count) noexcept {
			if (*this) return (*(*this)).UpdateResource(data, count);
			else return false;
		}
		bool UpdateResource(const Matrix* data, size_t count) noexcept {
			if (*this) return (*(*this)).UpdateResource(data, count);
			else return false;
		}
		bool UpdateResource(const Image& image) noexcept {
			if (*this) return (*(*this)).UpdateResource(image);
			else return false;
		}
		bool GetData(float* out, size_t count) const noexcept {
			if (*this) return (*(*this)).GetData(out, count);
			else return false;
		}
		bool GetData(Matrix* out, size_t count) const noexcept {
			if (*this) return (*(*this)).GetData(out, count);
			else return false;
		}
		bool GetData(Image& out) const noexcept {
			if (*this) return (*(*this)).GetData(out);
			else return false;
		}
	};
}

//Heap
export namespace System::Application::Common3D {
	class Heap : public ManagedObject<IHeap> {
	public:
		using ManagedObject<IHeap>::ManagedObject;
		Heap() noexcept = default;
		Heap(const ManagedObject<IHeap>& arg) noexcept
			: ManagedObject<IHeap>(arg) {}
	public:
		void SetDebugName(const String& name) noexcept {
			if (*this) (*(*this)).SetName(name);
		}
		const String& GetName() const noexcept { return ManagedObject<IHeap>::GetName(); }
		uint32_t GetViewCount() const noexcept {
			if (*this) return (*(*this)).GetViewCount();
			else return 0;
		}
		uint32_t GetViewsSetCount() const noexcept {
			if (*this) return (*(*this)).GetViewsSetCount();
			else return 0;
		}
		uint32_t GetAllViewCount() const noexcept {
			if (*this) return (*(*this)).GetAllViewCount();
			else return 0;
		}
	public:
		bool SetView(uint32_t index, ViewFormat format, const Resource& resource) noexcept {
			if (*this) return (*(*this)).SetView(index, format, resource);
			else return false;
		}
	};
}

//CommandList
export namespace System::Application::Common3D {
	class CommandList : public ManagedObject<ICommandList> {
	public:
		using ManagedObject<ICommandList>::ManagedObject;
		CommandList() noexcept = default;
		CommandList(const ManagedObject<ICommandList>& arg) noexcept
			: ManagedObject<ICommandList>(arg) {}
	public:
		bool BeginCommand() noexcept {
			if (*this) return (*(*this)).BeginCommand();
			else return false;
		}
		bool SetRenderer(const ManagedObject<IRenderer>& renderer) noexcept {
			if (*this) return (*(*this)).SetRenderer(renderer);
			else return false;
		}
		bool SetMesh(const ManagedObject<IMeshResource>& mesh) noexcept {
			if (*this) return (*(*this)).SetMesh(mesh);
			else return false;
		}
		bool SetCameraHeap(const ManagedObject<IHeap>& heap) noexcept {
			if (*this) return (*(*this)).SetCameraHeap(heap);
			else return false;
		}
		bool SetObjectHeap(const ManagedObject<IHeap>& heap) noexcept {
			if (*this) return (*(*this)).SetObjectHeap(heap);
			else return false;
		}
		bool SetAnimationHeap(const ManagedObject<IHeap>& heap) noexcept {
			if (*this) return (*(*this)).SetAnimationHeap(heap);
			else return false;
		}
		bool SetMaterialHeap(const ManagedObject<IHeap>& heap) noexcept {
			if (*this) return (*(*this)).SetMaterialHeap(heap);
			else return false;
		}
		bool SetMaterialIndex(uint32_t index) noexcept {
			if (*this) return (*(*this)).SetMaterialIndex(index);
			else return false;
		}
		bool SetSceneHeap(ManagedObject<IHeap> heap) noexcept {
			if (*this) return (*(*this)).SetSceneHeap(heap);
			else return false;
		}
		bool DrawIndexed(uint32_t startIndex, uint32_t useIndex) noexcept {
			if (*this) return (*(*this)).DrawIndexed(startIndex, useIndex);
			else return false;
		}
		bool EndCommand() noexcept {
			if (*this) return (*(*this)).EndCommand();
			else return false;
		}
		bool UsedHeap(HeapType type) const noexcept {
			if (*this) return (*(*this)).UsedHeap(type);
			else return false;
		}
	};
}

//RenderTarget
export namespace System::Application::Common3D {
	class RenderTarget : public ManagedObject<IRenderTarget> {
	public:
		using ManagedObject<IRenderTarget>::ManagedObject;
		RenderTarget() noexcept = default;
		RenderTarget(const ManagedObject<IRenderTarget>& arg) noexcept
			: ManagedObject<IRenderTarget>(arg) {}
	public:
		void SetDebugName() noexcept {
			if (*this) (*(*this)).SetName(ManagedObject<IRenderTarget>::GetName());
		}
		const String& GetName() const noexcept { return ManagedObject<IRenderTarget>::GetName(); }
		uint32_t GetBufferCount() const noexcept {
			if (*this) return (*(*this)).GetBufferCount();
			else return 0;
		}
		uint32_t GetTargetCount() const noexcept {
			if (*this) return (*(*this)).GetTargetCount();
			else return 0;
		}
		Size<uint32_t> GetSize() const noexcept {
			if (*this) return (*(*this)).GetSize();
			else return Size<uint32_t>{0, 0};
		}
		Resource GetTexture(uint32_t targetIndex = 0) const noexcept {
			if (*this) return Resource{ (*(*this)).GetTexture(targetIndex) };
			else return Resource();
		}
		Resource GetDepthTexture() const noexcept {
			if (*this) return Resource{ (*(*this)).GetDepthTexture() };
			else return Resource();
		}
		void SetDefaultClearColor(const Color& color) noexcept {
			if (*this) (*(*this)).SetDefaultClearColor(color);
		}
	public:
		bool BeginCommand(bool colorClear, bool depthClear, bool stencilClear) noexcept {
			if (*this) return (*(*this)).BeginCommand(colorClear, depthClear, stencilClear);
			else return false;
		}
		bool SetViewport(const ViewportDesc& desc = ViewportDesc()) noexcept {
			if (*this) return (*(*this)).SetViewport(desc);
			else return false;
		}
		bool SetViewports(const ViewportDesc* descs, uint32_t count) noexcept {
			if (*this) return (*(*this)).SetViewports(descs, count);
			else return false;
		}
		bool SetScissorRect(const ScissorRectDesc& desc = ScissorRectDesc()) noexcept {
			if (*this) return (*(*this)).SetScissorRect(desc);
			else return false;
		}
		bool SetScissorRects(const ScissorRectDesc* descs, uint32_t count) noexcept {
			if (*this) return (*(*this)).SetScissorRects(descs, count);
			else return false;
		}
		bool SetHeap(HeapType type, const ManagedObject<IHeap>& heap, uint32_t setViewsIndex = 0) noexcept {
			if (*this) return (*(*this)).SetHeap(type, heap, setViewsIndex);
			else return false;
		}
		bool SetRenderer(const ManagedObject<IRenderer>& renderer) noexcept {
			if (*this) return (*(*this)).SetRenderer(renderer);
			else return false;
		}
		bool SetCommandList(const ManagedObject<ICommandList>& list) noexcept {
			if (*this) return (*(*this)).SetCommandList(list);
			else return false;
		}
		bool EndCommand(bool sync) noexcept {
			if (*this) return (*(*this)).EndCommand(sync);
			else return false;
		}
		bool ResizeBuffer(uint32_t width, uint32_t height) noexcept {
			if (*this) return (*(*this)).ResizeBuffer(width, height);
			else return false;
		}
	};
}

//Renderer
export namespace System::Application::Common3D {
	class Renderer : public ManagedObject<IRenderer> {
	public:
		using ManagedObject<IRenderer>::ManagedObject;
		Renderer() noexcept = default;
		Renderer(const ManagedObject<IRenderer>& arg) noexcept
			: ManagedObject<IRenderer>(arg) {}
	public:
		const String& GetName() const noexcept { return ManagedObject<IRenderer>::GetName(); }
	};
}

//MeshResource
export namespace System::Application::Common3D {
	class MeshResource : public ManagedObject<IMeshResource> {
	public:
		using ManagedObject<IMeshResource>::ManagedObject;
		MeshResource() noexcept = default;
		MeshResource(const ManagedObject<IMeshResource>& arg) noexcept
			: ManagedObject<IMeshResource>(arg) {}
	public:
		const String& GetName() const noexcept { return ManagedObject<IMeshResource>::GetName(); }
	public:
		uint32_t GetVertexCount() const noexcept {
			if (*this) return (*(*this)).GetVertexCount();
			else return false;
		}
		uint32_t GetIndexCount() const noexcept {
			if (*this) return (*(*this)).GetIndexCount();
			else return false;
		}
		uint32_t GetMaterialCount() const noexcept {
			if (*this) return (*(*this)).GetMaterialCount();
			else return false;
		}
		uint32_t GetIndexCountsInMaterial(uint32_t materialIndex) const noexcept {
			if (*this) return (*(*this)).GetIndexCountsInMaterial(materialIndex);
			else return false;
		}
	};
}

//Function
export namespace System::Application::Common3D {
	/// <summary>
	/// 3Dデバイスの初期化を行い、終了処理をatexit()に設定する
	/// Main関数で呼び出すことで、Engine等よりも後に終了できる
	/// </summary>
	bool Init() noexcept;
	RenderTarget CreateRenderTarget(const String& name, uint32_t width, uint32_t height, uint32_t bufferCount, uint32_t targetCount = 1, const Color& defaultColor = Colors::White) noexcept;
	RenderTarget CreateRenderTarget(const String& name, IWindow& window, uint32_t bufferCount, const Color& defaultColor = Colors::White) noexcept;
	RenderTarget GetRenderTarget(const String& name) noexcept {
		return RenderTarget(ManagedObject<IRenderTarget>::GetObject(name));
	}
	Renderer CreateRenderer(const String& name, const RendererDesc& desc) noexcept;
	Renderer GetRenderer(const String& name) noexcept {
		return Renderer(ManagedObject<IRenderer>::GetObject(name));
	}
	//登録済みのリソース名が指定されたときは、参照カウントを増やしてtrueを返す
	//ただし、リソースの種類や個数が異なる場合、falseを返す
	Resource CreateResource(const String& name, const float* data, size_t count) noexcept;
	Resource CreateResource(const String& name, const Matrix* data, size_t count) noexcept;
	Resource CreateResource(const String& name, const Image& image) noexcept;
	Resource GetResource(const String& name) noexcept {
		return Resource(ManagedObject<IResource>::GetObject(name));
	}
	MeshResource CreateMeshResource(const String& name, const MeshDesc& desc) noexcept;
	MeshResource GetMeshResource(const String& name) noexcept {
		return MeshResource(ManagedObject<IMeshResource>::GetObject(name));
	}
	Heap CreateHeap(const String& name, HeapType type, uint32_t setCount = 1) noexcept;
	Heap GetHeap(const String& name) noexcept {
		static Heap defaultCameraHeap;
		static Resource defaultCameraResource;
		if (!defaultCameraHeap) {
			defaultCameraHeap = CreateHeap(Common3D::DefaultCameraHeapName, HeapType::Camera);
			CameraResourceDesc desc{};
			desc.view = Matrix::Identity();
			desc.projection = Matrix::Identity();
			desc.eye = Vector3::Zero();
			defaultCameraResource = CreateResource(u"DefaultCameraResource", desc.GetData(), desc.GetCount());
			defaultCameraHeap.SetView(0, ViewFormat::CBV, defaultCameraResource);
		}
		static Heap defaultObjectHeap;
		static Resource defaultObjectResource;
		if (!defaultObjectHeap) {
			defaultObjectHeap = CreateHeap(Common3D::DefaultObjectHeapName, HeapType::Object);
			ObjectResourceDesc desc{};
			desc.world = Matrix::Identity();
			defaultObjectResource = CreateResource(u"DefaultObjectResource", desc.GetData(), desc.GetCount());
			defaultObjectHeap.SetView(0, ViewFormat::CBV, defaultObjectResource);
		}
		static Heap defaultAnimationHeap;
		static Resource defaultAnimationResource;
		if (!defaultAnimationHeap) {
			defaultAnimationHeap = CreateHeap(Common3D::DefaultAnimationHeapName, HeapType::Animation);
			AnimationResourceDesc desc{};
			defaultAnimationResource = CreateResource(u"DefaultAnimationResource", desc.GetData(), desc.GetCount());
			defaultAnimationHeap.SetView(0, ViewFormat::CBV, defaultAnimationResource);
		}
		static Heap defaultMaterialHeap;
		static Resource defaultReflectionsResource;
		if (!defaultMaterialHeap) {
			defaultMaterialHeap = CreateHeap(Common3D::DefaultMaterialHeapName, HeapType::Material, 1);
			ReflectionsResourceDesc desc{};
			defaultReflectionsResource = CreateResource(u"DefaultReflectionsResource", desc.GetData(), desc.GetCount());
			defaultMaterialHeap.SetView(0, ViewFormat::CBV, defaultReflectionsResource);
			defaultMaterialHeap.SetView(1, ViewFormat::SRV, GetResource(Common3D::DefaultWhiteTextureName));
			defaultMaterialHeap.SetView(2, ViewFormat::SRV, GetResource(Common3D::DefaultWhiteTextureName));
			defaultMaterialHeap.SetView(3, ViewFormat::SRV, GetResource(Common3D::DefaultBlackTextureName));
			defaultMaterialHeap.SetView(4, ViewFormat::SRV, GetResource(Common3D::DefaultToonTextureName));
		}
		static Heap defaultSceneHeap;
		static Resource defaultSceneResource;
		if (!defaultSceneHeap) {
			defaultSceneHeap = CreateHeap(Common3D::DefaultSceneHeapName, HeapType::Scene);
			SceneResourceDesc desc{};
			defaultSceneResource = CreateResource(u"DefaultSceneResource", desc.GetData(), desc.GetCount());
			defaultSceneHeap.SetView(0, ViewFormat::CBV, defaultSceneResource);
		}
		return Heap(ManagedObject<IHeap>::GetObject(name));
	}
	CommandList CreateCommandList(const String& name, uint32_t materialCount) noexcept;

	bool Release3DManager() noexcept;
}
