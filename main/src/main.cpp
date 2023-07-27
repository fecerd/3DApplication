#include <crtdbg.h>
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
import System;	//ResourcePath
import Application;	//RunLoop
import Common3D;	//Init()
using namespace System;

//import Font;
//import Resource1;

template<Traits::Concepts::CForwardIterator Ite>
Ite Increment(const Ite& ite, uint32_t count) noexcept {
	Ite ret = ite;
	for (uint32_t i = 0; i < count; ++i) ++ret;
	return ret;
}

import MyWindow1;
import MyWindow2;

import Engine;
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

//import Font;
//import Resource1;
//void font_test() noexcept {
//	bool success = FontFactory::SetFont(ResourcePaths::GetPathName(ResourcePaths::Font::Century), u"Century");
//	IFont* century = FontFactory::GetFont(u"Century");
//	if (!century) return;
//	century->SetBaseLineColor(System::Drawing::Colors::White);
//	century->SetContoursColor(System::Drawing::Colors::Black);
//	System::Drawing::Image image(1920, 1080);
//	uint32_t permilli = 50;
//	int32_t originy = century->GetAscenderLine(permilli);
//	if (originy < 0) originy = 0;
//	for (bool b : century->UpdateStringImage(image, u"Hello World!", Point<int32_t>(0, originy), permilli));
//	System::Drawing::BMP::FromImage(image).Save(ResourcePaths::GetPathName(uR"(.\TMP\Hello_World.bmp)"));
//}

import SplashScreenScene;

namespace System::Traits {
	template<class T> struct is_volatile : false_type {};
	template<class T>
	struct is_volatile<T volatile> : true_type {};
	template<class T>
	inline constexpr bool is_volatile_v = is_volatile<T>::value;

	template<class T>
	struct remove_extent {
		using type = T;
	};
	template<class T>
	struct remove_extent<T[]> {
		using type = T;
	};
	template<class T, size_t N>
	struct remove_extent<T[N]> {
		using type = T;
	};
	template<class T> using remove_extent_t = remove_extent<T>::type;

	template<class T>
	struct remove_all_extents {
		using type = T;
	};
	template<class T>
	struct remove_all_extents<T[]> {
		using type = remove_all_extents<T>::type;
	};
	template<class T, size_t N>
	struct remove_all_extents<T[N]> {
		using type = remove_all_extents<T>::type;
	};
	template<class T> using remove_all_extents_t = remove_all_extents<T>::type;

	template<class T>
	struct remove_all : remove_all<Traits::remove_pointer_t<Traits::remove_cvref_t<T>>> {};
	template<class T> requires(Traits::is_same_v<T, Traits::remove_pointer_t<Traits::remove_cvref_t<T>>>)
	struct remove_all<T> {
		using type = T;
	};
	template<class T> using remove_all_t = remove_all<T>::type;

	template<class T>
	struct args_count {
		static constexpr size_t value = 0;
	};
	template<template<class ...U> class T, class ...Args>
	struct args_count<T<Args...>> {
		static constexpr size_t value = sizeof...(Args);
	};
	template<class T> inline constexpr size_t args_count_v = args_count<T>::value;

	template<class T>
	struct levels_of_pointer {
	private:
		using next_type = conditional_t<
			!is_same_v<remove_pointer_t<T>, T>,
			remove_pointer_t<T>,
			conditional_t<!is_same_v<remove_reference_t<T>, T>, remove_reference_t<T>, remove_cv_t<T>>
		>;
	public:
		static consteval size_t GetLevelsOfPointer() noexcept {
			if constexpr (Traits::is_same_v<remove_all_t<T>, T>) return 0;
			else return (!Traits::is_same_v<remove_pointer_t<T>, T> ? 1 : 0) + levels_of_pointer<next_type>::GetLevelsOfPointer();
		}
		static constexpr size_t value = GetLevelsOfPointer();
	};
	template<class T>
	inline constexpr size_t levels_of_pointer_v = levels_of_pointer<T>::value;
}

namespace System {
	template<class T>
	class TestCollection {
	public:
		constexpr TestCollection() noexcept {}
		//virtual constexpr ~TestCollection() noexcept {}
	public:
		//virtual int func() noexcept = 0;
	};

	struct View : public TestCollection<char> {
		const char* m_begin = nullptr;
		const char* m_end = nullptr;
	public:
		constexpr View() noexcept = default;
		constexpr View(char const* pBegin, char const* pEnd) noexcept
			: m_begin(pBegin), m_end(pEnd) {}
		constexpr ~View() noexcept {}
	public:
		//constexpr int func() noexcept override { return 0; }
	public:
		constexpr size_t Size() const noexcept { return m_end - m_begin + 1; }
		template<size_t N>
		constexpr auto GetCString() const noexcept {
			return CStringBlock<char, N>{ m_begin, static_cast<size_t>(m_end - m_begin + 1) };
		}
	public:
	/*	IEnumerator<char> GetEnumerator() noexcept override {
			for (auto ite = m_begin; ite != m_end; ++ite) co_yield const_cast<char&>(*ite);
		}
		IEnumerator<char const> GetEnumerator() const noexcept override {
			for (auto ite = m_begin; ite != m_end; ++ite) co_yield *ite;
		}
		IEnumerator<char> GetReverseEnumerator() noexcept override {
			for (auto ite = m_end; ite-- != m_begin;) co_yield const_cast<char&>(*ite);
		}
		IEnumerator<char const> GetReverseEnumerator() const noexcept override {
			for (auto ite = m_end; ite-- != m_begin;) co_yield *ite;
		}*/
	};
}

template<class T>
struct TypeNameTest {
	static constexpr bool HasConst = !Traits::is_same_v<T, Traits::remove_const_t<T>>;
	static constexpr bool HasVolatile = !Traits::is_same_v<T, Traits::remove_volatile_t<T>>;
	static constexpr bool IsPointer = !Traits::is_same_v<T, Traits::remove_pointer_t<T>>;
	static constexpr bool IsLValueReference = Traits::is_lvalue_reference_v<T>;
	static constexpr bool IsRValueReference = Traits::is_rvalue_reference_v<T>;
	static constexpr bool IsReference = IsLValueReference || IsRValueReference;
	static constexpr bool HasNoQualifiers = !HasConst && !HasVolatile;
	static constexpr bool IsSimpleType = HasNoQualifiers && !IsPointer && !IsReference;
	static constexpr bool IsFundamental = Traits::is_fundamental_v<T>;
	static constexpr bool IsClass = Traits::is_class_v<T>;
	static constexpr bool IsUnion = Traits::is_union_v<T>;
	static constexpr bool IsEnum = Traits::is_enum_v<T>;
	static constexpr bool IsEnumClass = Traits::is_scoped_enum_v<T>;
	static constexpr bool IsArray = Traits::is_array_v<T>;
public:
	using remove_cv_type = Traits::remove_cv_t<T>;
	using remove_pointer_type = Traits::remove_pointer_t<T>;
	using remove_reference_type = Traits::remove_reference_t<T>;
	using remove_all_extents_type = Traits::remove_all_extents_t<T>;
	using next_type = Traits::conditional_t<
		IsPointer,
		remove_pointer_type,
		Traits::conditional_t<IsReference, remove_reference_type, remove_cv_type>
	>;
	using root_type = Traits::remove_all<T>::type;
public:
	static constexpr size_t TemplateArgsCount = Traits::args_count_v<root_type>;
	static constexpr size_t LevelsOfPointer = Traits::levels_of_pointer_v<T>;
private:
	static constexpr bool Compare(const char* lhs, const char* rhs, size_t count) noexcept {
		for (size_t i = 0; i < count; ++i) if (lhs[i] != rhs[i]) return false;
		return true;
	}
	static constexpr auto ToTypeNameView(const View& rootFullNameView) noexcept {
		View ret = rootFullNameView;
		int32_t templateLevel = 0;
		for (const char* c = ret.m_end; c-- > ret.m_begin;) {
			if (*c == '>') ++templateLevel;
			else if (*c == '<') --templateLevel;
			else if (*c == ':' && templateLevel == 0) {
				if (c != ret.m_begin && *(c - 1) == ':') {
					ret.m_begin = c + 1;
					break;
				}
			}
		}
		return ret;
	}
	static constexpr auto ToNamespaceView(const View& rootFullNameView) noexcept {
		View ret = ToTypeNameView(rootFullNameView);
		ret.m_end = ret.m_begin;
		ret.m_begin = rootFullNameView.m_begin;
		for (const char* c = ret.m_end; c-- > ret.m_begin;) {
			if (*c != ' ' && *c != ':') {
				ret.m_end = c + 1;
				break;
			}
		}
		return ret;
	}
public:
	static consteval auto GetView() noexcept {
#if defined(__clang__) || defined(__GNUC__)
		return View{ __PRETTY_FUNCTION__, __PRETTY_FUNCTION__ + sizeof(__PRETTY_FUNCTION__) - sizeof("]") };
#elif defined(_MSC_VER)
		return View{ __FUNCSIG__, __FUNCSIG__ + sizeof(__FUNCSIG__) - sizeof(">::GetView(void) noexcept") };
#else
		return View{ };
#endif
	}
	static constexpr View TrimView(const View& view) noexcept {
		View ret = view;
		int32_t templateCount = 0;
		//先頭をトリミングする
		for (char const* c = ret.m_end; c-- > ret.m_begin; ) {
			if (*c == '>') ++templateCount;
			else if (*c == '<') {
				--templateCount;
				if (templateCount < 0) {
					ret.m_begin = c + 1;
					constexpr size_t size[5] = { sizeof("class"), sizeof("struct"), sizeof("union"), sizeof("enum class"), sizeof("enum") };
					constexpr char const* cstr[5] = { "class", "struct", "union", "enum class", "enum" };
					for (size_t i = 0; i < 5; ++i) {
						if (Compare(ret.m_begin, cstr[i], size[i] - 1)) {
							ret.m_begin += size[i];
							break;
						}
					}
					break;
				}
			}
			else if (*c == '[' && c[1] == 'w' && c[2] == 'i' && c[3] == 't' && c[4] == 'h') {
				ret.m_begin = c + sizeof("with T = ");
				break;
			}
		}
		//末尾をトリミングする
		for (const char* c = ret.m_end; c-- > ret.m_begin;) {
			if (*c != ' ') break;
			else ret.m_end = c;
		}
		return ret;
	}
public:
	static constexpr View FullNameView = TrimView(GetView());
	static constexpr View TypeNameView = ToTypeNameView(TypeNameTest<root_type>::FullNameView);
	static constexpr View NamespaceView = ToNamespaceView(TypeNameTest<root_type>::FullNameView);
public:
	static constexpr auto GetFullName() noexcept {
		return FullNameView.GetCString<FullNameView.Size()>();
	}
	static constexpr auto GetTypeName() noexcept {
		return TypeNameView.GetCString<TypeNameView.Size()>();
	}
	static constexpr auto GetNamespace() noexcept {
		return NamespaceView.GetCString<NamespaceView.Size()>();
	}
};

template<auto V> struct EnumNameTest {};
template<Traits::Concepts::CEnum E, E val>
struct EnumNameTest<val> {
private:
	static constexpr bool Compare(const char* lhs, const char* rhs, size_t count) noexcept {
		for (size_t i = 0; i < count; ++i) if (lhs[i] != rhs[i]) return false;
		return true;
	}
	template<E v>
	static constexpr auto GetView() noexcept {
#if defined(__clang__) || defined(__GNUC__)
		return View{ __PRETTY_FUNCTION__, __PRETTY_FUNCTION__ + sizeof(__PRETTY_FUNCTION__) - sizeof("]") };
#elif defined(_MSC_VER)
		return View{ __FUNCSIG__, __FUNCSIG__ + sizeof(__FUNCSIG__) - sizeof(">(void) noexcept") };
#else
		return View{ };
#endif
	}
	static constexpr View ToFullNameView(const View& view) noexcept {
		View ret = view;
		int32_t templateCount = 0;
		//先頭をトリミングする
		for (char const* c = ret.m_end; c-- > ret.m_begin; ) {
			if (*c == '>') ++templateCount;
			else if (*c == '<') {
				--templateCount;
				if (templateCount < 0) {
					ret.m_begin = c + 1;
					constexpr size_t size[5] = { sizeof("class"), sizeof("struct"), sizeof("union"), sizeof("enum class"), sizeof("enum") };
					constexpr char const* cstr[5] = { "class", "struct", "union", "enum class", "enum" };
					for (size_t i = 0; i < 5; ++i) {
						if (Compare(ret.m_begin, cstr[i], size[i] - 1)) {
							ret.m_begin += size[i];
							break;
						}
					}
					break;
				}
			}
			else if (*c == '[' && c[1] == 'w' && c[2] == 'i' && c[3] == 't' && c[4] == 'h') {
				ret.m_begin = c + sizeof("with T = ");
				break;
			}
		}
		//末尾をトリミングする
		for (const char* c = ret.m_end; c-- > ret.m_begin;) {
			if (*c != ' ') break;
			else ret.m_end = c;
		}
		return ret;
	}
	static constexpr auto ToValueNameView(const View& rootFullNameView) noexcept {
		View ret = rootFullNameView;
		int32_t templateLevel = 0;
		for (const char* c = ret.m_end; c-- > ret.m_begin;) {
			if (*c == '>') ++templateLevel;
			else if (*c == '<') --templateLevel;
			else if (*c == ':' && templateLevel == 0) {
				if (c != ret.m_begin && *(c - 1) == ':') {
					ret.m_begin = c + 1;
					break;
				}
			}
		}
		return ret;
	}
private:
	static constexpr View FullNameView = ToFullNameView(GetView<val>());
	static constexpr View ValueNameView = ToValueNameView(FullNameView);
public:
	static constexpr E GetValue() noexcept { return val; }
	static constexpr auto GetFullName() noexcept {
		return FullNameView.GetCString<FullNameView.Size()>();
	}
	static constexpr auto GetValueName() noexcept {
		return ValueNameView.GetCString<ValueNameView.Size()>();
	}
};

struct Dummy {};
namespace System::Application::Common3D {
	template<class T> class TestClass {};
}

constexpr char const msvc[] = "auto __cdecl TypeNameTest<class System::Application::Common3D::TestClass<struct Dummy> const volatile * const &>::GetView(void) noexcept";
constexpr char const gcc[] = "static consteval auto TypeNameTest<T>::GetView() [with T = const volatile System::Application::Common3D::TestClass<Dummy>* const&]";

void typename_test() noexcept {
	using type = System::Application::Common3D::TestClass<Dummy> const volatile* const&;
	using root_type = TypeNameTest<type>::root_type;

	constexpr size_t levels = TypeNameTest<String const* const**const volatile* const>::LevelsOfPointer;
	constexpr size_t argsCount = TypeNameTest<type>::TemplateArgsCount;

	TypeNameTest<root_type> typeNameTest{};
	auto typeNameTestView = typeNameTest.GetView();
	auto typeNameTestStr = typeNameTest.GetFullName();

	constexpr View gccView = View{ gcc, gcc + (sizeof(gcc) - 2) };
	constexpr auto gccCstr = gccView.GetCString<gccView.Size()>();
	constexpr View msvcView = TypeNameTest<type>::GetView();	//View{ msvc, msvc + (sizeof(msvc) - 17) };
	constexpr auto msvcCstr = msvcView.GetCString<msvcView.Size()>();

	constexpr View gccTrim = TypeNameTest<type>::TrimView(gccView);
	//const volatile System::Application::Common3D::TestClass<Dummy>* const&
	constexpr View msvcTrim = TypeNameTest<type>::TrimView(msvcView);
	//System::Application::Common3D::TestClass<struct Dummy> const volatile * const &

	constexpr auto fullnameCstr = TypeNameTest<type>::GetFullName();
	constexpr auto typenameCstr = TypeNameTest<type>::GetTypeName();
	constexpr auto namespaceCstr = TypeNameTest<type>::GetNamespace();

	constexpr auto enumFullnameCstr = EnumNameTest<Application::Common3D::BlendOp::Subtract>::GetFullName();
	constexpr auto enumValuenameCstr = EnumNameTest<Application::Common3D::BlendOp::ReverseSubtract>::GetValueName();

	return;
}

int main(int argc, char** argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//リソースパスのルートを設定
	ResourcePaths::RootPath = String(uR"(C:\Users\祐貴\source\repos\fecerd\DesktopApp1\ApplicationTest\Resources)");
	//ResourcePaths::RootPath = String(uR"(C:\source\repos\DesktopApp1\ApplicationTest\Resources)");

	//FPSを自動同期(DirectX)に設定
	System::Application::SetFPS(0);
	System::Application::Common3D::Init();
	
	

	//MyWindow2 my2;
	//my2.Show();

	typename_test();
	System::Application::RunLoop();
	return 0;

	MyWindow1 my1;
	my1.Show();
	int main_return = System::Application::RunLoop();
	return main_return;
}
