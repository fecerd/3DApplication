#include <crtdbg.h>
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
import System;	//ResourcePath
import Application;	//RunLoop
import Common3D;	//Init()

import MyWindow1;
import MyWindow2;

import Engine;

import SplashScreenScene;

import Resource1;

using namespace System;
using namespace Engine;


namespace Engine {
	class RectTransform : public Transform {
		Point<float> AnchorMin = { 0.5f, 0.5f };
		Point<float> AnchorMax = { 0.5f, 0.5f };
		Point<float> Pivot = { 0.5f, 0.5f };
		Vector2 AnchoredPosition = Vector2::Zero();
	public:
		using Transform::Transform;
	public:
		Type GetType() const noexcept override { return Type::CreateType<RectTransform>(); }
		String ToString() const noexcept override { return String(u"RectTransform Component"); }
		uint32_t GetTypeID() const noexcept override { return System::GetID<RectTransform>(); }
		RectTransform* Clone(GameObject* object) noexcept override {
			RectTransform* ret = new RectTransform(object);
			ret->LocalPosition = LocalPosition;
			ret->LocalRotation = LocalRotation;
			ret->LocalScale = LocalScale;
			ret->AnchorMin = AnchorMin;
			ret->AnchorMax = AnchorMax;
			ret->Pivot = Pivot;
			ret->AnchoredPosition = AnchoredPosition;
			return ret;
		}
	};

	enum class CanvasRenderMode : uint8_t {
		ScreenOverlay,
		ScreenCamera,
		WorldSpace
	};

	class Canvas : public Component {
		GameObject* m_cameraObj = nullptr;
		float m_planeDistance = 0.f;
	public:
		CanvasRenderMode RenderMode = CanvasRenderMode::ScreenOverlay;
		bool ReceivesEvents = true;
	public:
		using Component::Component;
	public:
		Canvas& GetRootCanvas() const noexcept {
			GameObject* current = &this->GetObject();
			GameObject* parent = current->GetParent();
			while (parent) {
				if (parent->HasComponent<Canvas>()) {
					current = parent;
					parent = parent->GetParent();
				}
				else break;
			}
			return current->GetComponent<Canvas>();
		}
		CanvasRenderMode GetRenderMode() const noexcept {
			return GetRootCanvas().RenderMode;
		}
	public:
		Type GetType() const noexcept override { return Type::CreateType<Canvas>(); }
		uint32_t GetTypeID() const noexcept override { return System::GetID<Canvas>(); }
		Canvas* Clone(GameObject* object) noexcept override {
			Canvas* ret = new Canvas(object);
			return ret;
		}
	};
	using namespace System::Application::Common3D;
	class Shape : public Component {
		MeshResource m_meshResource;
	};

	class UIObject : public GameObject {
	public:
		UIObject() noexcept : UIObject(String::Empty()) {}
		UIObject(const String& name) noexcept : GameObject(name) {
			RemoveComponent<Transform>();
			m_transform = &AddComponent<RectTransform>();
		}
	public:

	};
}

bool font_test() noexcept {
	ManagedObject<FontFactory> factory = FontFactory::GetFontFactory();
	bool success = factory->SetFont(ResourcePaths::GetPathName(ResourcePaths::Font::Century), u"Century");
	System::IFont* century = factory->GetFont(u"Century");
	if (!century) return false;
	century->SetBaseLineColor(Drawing::Colors::White);
	century->SetContoursColor(Drawing::Colors::Black);
	Drawing::Image image(1920, 1080);
	uint32_t permilli = 50;
	int32_t originy = century->GetAscenderLine(permilli);
	if (originy < 0) originy = 0;
	for (bool b : century->UpdateStringImage(image, u"Hello World!", Point<int32_t>(0, originy), permilli)) {}
	return Drawing::BMP::FromImage(image).Save(ResourcePaths::ExePath.CreatePath(u"./Hello_World.bmp").PathName());
}

int main(int argc, char** argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//リソースパスのルートを設定
	ResourcePaths::RootPath = String(u"C:/source/vscode/3DApplication/main/Resources");
	ResourcePaths::ExePath = String(argv[0]);

	//FPSを自動同期(DirectX)に設定
	Application::SetFPS(0);

	Application::Common3D::Init();

	//MyWindow2 my2;
	//my2.Show();

	MyWindow1 my1;
	my1.Show();

	int main_return = Application::RunLoop(true);
	return main_return;
}
