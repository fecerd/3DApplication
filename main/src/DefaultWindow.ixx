export module DefaultWindow;
export import System;
export import Application;
export import InputSystem;
export import Engine;
using namespace System;
using namespace System::Application;
using namespace Engine;

export namespace Engine
{
	template <class S>
	struct StartScene
	{
		using type = System::Traits::remove_cvref_t<S>;
	};
	template <class S>
	using scene_type = typename StartScene<S>::type;
}

import Common3D;
using namespace System::Application::Common3D;

export namespace Engine
{
	class DefaultWindow : public IWindow
	{
	public:
		DefaultWindow() noexcept : DefaultWindow(u"DefaultWindow") {}
		template <class S = void>
		DefaultWindow(const String &name, StartScene<S> scene = StartScene<S>{}, int32_t width = 1600, int32_t height = 900, uint64_t fixedTimestep = 20) noexcept
			: IWindow(WindowDesc{name, DEFAULT_WINDOWPOS, DEFAULT_WINDOWPOS, width, height, CursorType::Arrow, fixedTimestep})
		{
			if constexpr (!Traits::is_void_v<typename StartScene<S>::type>)
			{
				String scenename = String(TypeName<typename StartScene<S>::type>::GetTypeName());
				SceneManager::GetManager().LoadScene<scene_type<S>>(scenename);
				SceneManager::GetManager().ActivateScene(scenename, 0);
			}
		}
		~DefaultWindow() noexcept = default;

	public:
		RenderTarget m_renderTarget;
		void OnUpdate() noexcept override
		{
			SceneManager::GetManager().Update();
			if (!m_renderTarget)
			{
				m_renderTarget = CreateRenderTarget(this->GetName() + u"RenderTarget", *this, 2);
				m_renderTarget.SetDebugName();
			}
			SceneManager::GetManager().Rendering(m_renderTarget);
		}
		void OnResized(const ResizeEventArgs &e) noexcept override
		{
			if (e.Width && e.Height)
			{
				m_renderTarget.ResizeBuffer(e.Width, e.Height);
			}
		}
		void OnClosing(CloseEventArgs &e) noexcept override
		{
			SceneManager::GetManager().Closing(e);
			if (e.Closing)
			{
				MessageBoxResult result = System::Application::MessageBox(this, u"ゲームの終了", u"ゲームを終了しますか？", MessageBoxType::YesNo);
				e.Closing = result == MessageBoxResult::Yes;
			}
			if (!e.Closing)
				SceneManager::GetManager().CloseCanceled();
		}
		void OnClosed() noexcept override
		{
			SceneManager::GetManager().Closed();
		}
	};
}

// export namespace Engine {
//	class DefaultWindow : public IWindow {
//	public:
//		DefaultWindow() noexcept : DefaultWindow(u"DefaultWindow") {}
//		template<class S = void>
//		DefaultWindow(const String& name, StartScene<S> scene = StartScene<S>{}, int32_t width = 1600, int32_t height = 900, uint64_t fixedTimestep = 20) noexcept
//			: IWindow(WindowDesc{ name, DEFAULT_WINDOWPOS, DEFAULT_WINDOWPOS, width, height, CursorType::Arrow, fixedTimestep })
//		{
//			if constexpr (!Traits::is_void_v<typename StartScene<S>::type>) {
//				String scenename = String(TypeName<typename StartScene<S>::type>::GetTypeName());
//				SceneManager::LoadScene<typename StartScene<S>::type>(scenename);
//				SceneManager::SetNextScene(scenename);
//			}
//		}
//		~DefaultWindow() noexcept = default;
//	public:
//		void OnAwake() noexcept override {}
//		//SharedPtr<Scene> m_scene = nullptr;
//		Scene* m_scene = nullptr;
//		void OnUpdate() noexcept override {
//			System::Application::InputSystem::Update();
//			if (m_scene) {
//				m_scene->Update();
//				m_scene->LastUpdate();
//				m_scene->Draw();
//			}
//			if (SceneManager::Update()) {
//				m_scene = SceneManager::Current();
//				if (m_scene) m_scene->SetWindow(this);
//			}
//		}
//		void OnFixedUpdate() noexcept override {
//			if (m_scene) m_scene->FixedUpdate();
//		}
//		void OnMouseMove(const MouseEventArgs& e) noexcept override {}
//		void OnResized(const ResizeEventArgs& e) noexcept override {
//			if (e.Width && e.Height) {
//				if (m_scene) m_scene->Resized(e);
//			}
//		}
//		void OnClosing(CloseEventArgs& e) noexcept override {
//			if (!m_scene) return;
//			m_scene->Closing(e);
//			if (e.Closing) {
//				MessageBoxResult result = System::Application::MessageBox(this, u"ゲームの終了", u"ゲームを終了しますか？", MessageBoxType::YesNo);
//				e.Closing = result == MessageBoxResult::Yes;
//			}
//			if (!e.Closing) m_scene->CloseCanceled();
//		}
//		void OnClosed() noexcept override {
//			if (m_scene) m_scene->Closed();
//		}
//	};
// }