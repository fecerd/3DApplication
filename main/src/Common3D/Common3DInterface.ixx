export module Common3DInterface;
export import System;
export import Common3DUtilities;
using namespace System::Drawing;

//Interface
export namespace System::Application::Common3D {
	class I3DBase {
	public:
		virtual ~I3DBase() noexcept {}
	public:
		virtual PlatformID GetPlatformID() const noexcept = 0;
		bool IsValidPlatform(PlatformID id) const noexcept {
			return GetPlatformID() == id;
		}
		virtual void SetName(const String& name) noexcept {}
	public:
		void Dispose() noexcept {
			delete this;
		}
	};

	class IResource : public I3DBase {
	public:
		~IResource() noexcept {}
	public:
		virtual ResourceFormat GetFormat() const noexcept = 0;
		virtual Size<uint32_t> GetSize() const noexcept = 0;
		virtual uint32_t GetCount() const noexcept = 0;
	public:
		virtual void Next() noexcept = 0;
	public:
		virtual bool UpdateResource(const float* data, size_t count) noexcept = 0;
		virtual bool UpdateResource(const Matrix* data, size_t count) noexcept = 0;
		virtual bool UpdateResource(const Image& image) noexcept = 0;
		virtual bool GetData(float* out, size_t count) const noexcept = 0;
		virtual bool GetData(Matrix* out, size_t count) const noexcept = 0;
		virtual bool GetData(Image& out) const noexcept = 0;
	};

	class IHeap : public I3DBase {
	public:
		virtual ~IHeap() noexcept {}
	public:
		virtual uint32_t GetViewCount() const noexcept = 0;
		virtual uint32_t GetViewsSetCount() const noexcept = 0;
		virtual uint32_t GetAllViewCount() const noexcept = 0;
	public:
		virtual bool SetView(uint32_t index, ViewFormat format, ManagedObject<IResource> resource) noexcept = 0;
	};

	class IShader : public I3DBase {
	public:
		~IShader() noexcept {}
	};

	class IRenderer : public I3DBase {
	public:
		virtual ~IRenderer() noexcept {}
	};

	class IMeshResource : public I3DBase {
	public:
		virtual ~IMeshResource() noexcept {}
	public:
		virtual uint32_t GetVertexCount() const noexcept = 0;
		virtual uint32_t GetIndexCount() const noexcept = 0;
		virtual uint32_t GetMaterialCount() const noexcept = 0;
		virtual uint32_t GetIndexCountsInMaterial(uint32_t materialIndex) const noexcept = 0;
	};

	class IGraphicsCommand : public I3DBase {
		RenderingMode m_mode = RenderingMode::Standard;
	public:
		virtual ~IGraphicsCommand() noexcept {}
	public:
		virtual bool BeginCommand(RenderingMode mode) noexcept = 0;
		virtual bool SetRenderer(const ManagedObject<IRenderer>& renderer) noexcept = 0;
		virtual bool SetMesh(const ManagedObject<IMeshResource>& mesh) noexcept = 0;
		virtual bool SetHeap(HeapType type, const ManagedObject<IHeap>& heap) noexcept = 0;
		virtual bool SetHeapIndex(HeapType type, uint32_t index) noexcept = 0;
		virtual bool DrawIndexed(uint32_t startIndex, uint32_t useIndexCount) noexcept = 0;
		virtual bool EndCommand() noexcept = 0;
		virtual bool UsedHeap(HeapType type) const noexcept = 0;
	public:
		RenderingMode GetRenderingMode() const noexcept { return m_mode; }
	};

	class ICommandList : public I3DBase {
	public:
		virtual ~ICommandList() noexcept {}
	public:
		virtual bool BeginCommand() noexcept = 0;
		virtual bool SetRenderer(ManagedObject<IRenderer> renderer) noexcept = 0;
		virtual bool SetMesh(ManagedObject<IMeshResource> mesh) noexcept = 0;
		virtual bool SetCameraHeap(ManagedObject<IHeap> heap) noexcept = 0;
		virtual bool SetObjectHeap(ManagedObject<IHeap> heap) noexcept = 0;
		virtual bool SetAnimationHeap(ManagedObject<IHeap> heap) noexcept = 0;
		virtual bool SetMaterialHeap(ManagedObject<IHeap> heap) noexcept = 0;
		virtual bool SetMaterialIndex(uint32_t index) noexcept = 0;
		virtual bool SetSceneHeap(ManagedObject<IHeap> heap) noexcept = 0;
		virtual bool DrawIndexed(uint32_t startIndex, uint32_t useIndexCount) noexcept = 0;
		virtual bool EndCommand() noexcept = 0;
		virtual bool UsedHeap(HeapType type) const noexcept = 0;
	};

	class IRenderTarget : public I3DBase {
	public:
		virtual ~IRenderTarget() noexcept {}
	public:
		virtual bool IsSwapChain() const noexcept = 0;
		virtual uint32_t GetBufferCount() const noexcept = 0;
		virtual uint32_t GetTargetCount() const noexcept = 0;
		virtual Size<uint32_t> GetSize() const noexcept = 0;
		virtual ManagedObject<IResource> GetTexture(uint32_t targetIndex) const noexcept = 0;
		virtual ManagedObject<IResource> GetDepthTexture() const noexcept = 0;
		virtual void SetDefaultClearColor(const Color& color) noexcept = 0;
	public:
		virtual bool BeginCommand(bool colorClear, bool depthClear, bool stencilClear) noexcept = 0;
		virtual bool SetViewports(const ViewportDesc* descs, uint32_t count) noexcept = 0;
		virtual bool SetViewport(const ViewportDesc& desc) noexcept = 0;
		virtual bool SetScissorRects(const ScissorRectDesc* descs, uint32_t count) noexcept = 0;
		virtual bool SetScissorRect(const ScissorRectDesc& desc) noexcept = 0;
		virtual bool SetHeap(HeapType type, const ManagedObject<IHeap>& heap, uint32_t viewsSetIndex = 0) noexcept = 0;
		virtual bool SetRenderer(ManagedObject<IRenderer> renderer) noexcept = 0;
		virtual bool SetMesh(ManagedObject<IMeshResource> mesh) noexcept = 0;
		virtual bool DrawIndexed(uint32_t startIndex, uint32_t useIndexCount) noexcept = 0;
		virtual bool SetCommandList(ManagedObject<ICommandList> list) noexcept = 0;
		virtual bool EndCommand(bool sync) noexcept = 0;
		virtual bool ResizeBuffer(uint32_t width, uint32_t height) noexcept = 0;
	};
}
