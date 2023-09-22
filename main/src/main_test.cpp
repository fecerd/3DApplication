import System;
import BoostCoroutine;
import <iostream>;
using namespace System;

#if !defined(__GNUC__)
//#define __GNUC__
#endif
#if defined(__GNUC__) && !defined(__clang__)

void coro_1(Boost::push_type<int>& sink) {
	std::cout << 2 << " ";
	sink(3);
	std::cout << 5 << " ";
	sink(6);
}

void CoroutineTest1() noexcept {
	std::cout << 1 << " ";
	Boost::pull_type<int> source(coro_1);
	if (source) std::cout << source.get() << " ";
	std::cout << 4 << " ";
	if (source) {
		source();
		std::cout << source.get() << " ";
	}
	std::cout << 7 << std::endl;
}

void coro_2(Boost::pull_type<int>& source) {
	std::cout << source.get() << " ";
	std::cout << 3 << " ";
	source();
	std::cout << source.get() << " ";
}

void CoroutineTest2() noexcept {
	std::cout << 1 << " ";
	Boost::push_type<int> sink(coro_2);
	sink(2);
	std::cout << 4 << " ";
	sink(5);
	std::cout << 6 << " " << std::endl;
}

void CoroutineTest3() noexcept {
	Boost::pull_type<int> source{
		[](Boost::push_type<int>& sink) {
			for (int i = 0; i < 10; ++i) sink(i);
		}
	};
	for (int& i : source) std::cout << i << " ";
	std::cout << std::endl;
}

void CoroutineTest4() noexcept {
	Boost::push_type<int> sink{
		[&](Boost::pull_type<int>& source) {
			while (source) {
				std::cout << source.get() << " ";
				source();
			}
		}
	};
	Vector<int> v{ 1, 1, 2, 3, 5 };
	auto ite = begin(sink);
	for (int& i : v) {
		*ite = i;
		++ite;
	}
	std::cout << std::endl;
}

IEnumerator<int> GetIEnumerator(bool rev) {
	auto internal1 = [](Boost::push_type<int&>& sink) {
		for (int i = 0; i < 20; ++i) sink(i);
	};
	auto internal2 = [](Boost::push_type<int&>& sink) {
		for (int i = 20; i-- >  0;) sink(i);
	};
	return IEnumerator<int>(rev ? internal2 : internal1);
}

void CoroutineTest5() noexcept {
	std::cout << "CoroutineTest5 start." << std::endl;
	IEnumerator<int> ien1 = GetIEnumerator(false);
	for (int& i : ien1) std::cout << i << " ";
	std::cout << std::endl;

	IEnumerator<int> ien2{ &GetIEnumerator, true };
	for (int& i : ien2) std::cout << i << " ";
	std::cout << std::endl;
}

#else

System::IEnumerator<int> GetIEn1(bool rev) {
	if (rev) {
		for (int i = 20; i-- >  0;) co_yield i;
	}
	else {
		for (int i = 0; i < 20; ++i) co_yield i;
	}
}

struct Test1 {
	int data = 0;
};

System::IEnumerator<Test1> GetIEn2(bool rev) {
	if (rev) {
		for (int i = 20; i-- >  0;) {
			Test1 tmp; tmp.data = i;
			co_yield tmp;
		}
	} else {
		for (int i = 0; i < 20; ++i) {
			Test1 tmp; tmp.data = i;
			co_yield tmp;
		}
	}
}

void CoroutineTest1() noexcept {
	std::cout << "CoroutineTest1 start." << std::endl;
	System::IEnumerator<int> ien1 = GetIEn1(false);
	for (int& i : ien1) std::cout << i << " ";
	std::cout << std::endl;

	System::IEnumerator<int> ien2{ &GetIEn1, true };
	for (int& i : ien2) std::cout << i << " ";
	std::cout << std::endl;	
}
void CoroutineTest2() noexcept {
	std::cout << "CoroutineTest2 start." << std::endl;
	System::IEnumerator<Test1> ien1 = GetIEn2(false);
	for (Test1& i : ien1) std::cout << i.data << " ";
	std::cout << std::endl;

	System::IEnumerator<Test1> ien2{ &GetIEn2, true };
	auto mul = [](Test1& x) -> int { return x.data * x.data; };
	System::SelectEnumerator<int, Test1> sel2 { new IEnumerator<Test1>(&GetIEn2, true), Function<int(Test1&)>(mul) };
	for (int& i : sel2) std::cout << i << " ";
	std::cout << std::endl;	
}
void CoroutineTest3() noexcept {}
void CoroutineTest4() noexcept {}
void CoroutineTest5() noexcept {}
#endif

struct Select {
	IEnumerator<HashNode<int, String>> m_prev;
	String HashNode<int, String>::* m_member = &HashNode<int, String>::m_value;
public:
	Select(IEnumerator<HashNode<int, String>>&& prev) noexcept : m_prev(System::move(prev)) {}
public:
#if defined(__GNUC__) && !defined(__clang__)
	void select(Boost::push_type<String&>& sink) {
		for (HashNode<int, String>& node : m_prev) {
			String& x = node.*m_member;
			sink(x);
		}
	}
	IEnumerator<String> Get() {
		auto internal = [this](bool) {
			return IEnumerator<String>(Function([this](Boost::push_type<String&>& sink){ select(sink); }));
		};

		return IEnumerator<String>(internal, false);
	}
#else
	IEnumerator<String> select() {
		for (HashNode<int, String>& node : m_prev) {
			String& x = node.*m_member;
			co_yield x;
		}		
	}
	IEnumerator<String> Get() {
		// if (m_prev.m_enumFunc) {
		// 	if (m_prev.m_handle) m_prev.m_handle.destroy();
		// 	IEnumerator<HashNode<int, String>>&& tmp = m_prev.m_enumFunc(m_prev.m_reverse);
		// 	m_prev.m_handle = tmp.m_handle;
		// 	m_prev.m_done = false;
		// 	tmp.m_handle = IEnumerator<HashNode<int, String>>::handle();
		// 	tmp.m_done = true;
		// }
		auto internal = [this](bool) {
			return select();
		};
		return IEnumerator<String>(internal, false);
	}
#endif
};

void CoroutineTest6() noexcept {
	std::cout << "CoroutineTest6 start." << std::endl;

	Vector<String> vec;
	vec.Add(u"Hello");
	vec.Add(u"Hoge");
	vec.Add(u"Piyo");

	if constexpr (true) {
		for (String& str : vec) std::cout << str << std::endl;
		std::cout << "--1--" << std::endl;

		for (String& str : vec.Chain()) std::cout << str << std::endl;
		std::cout << "--2--" << std::endl;
	}

	if constexpr (true) {
		std::cout << "Block start." << std::endl;
		Vector<HashNode<String, String>*> nodeVec;
		String key0{ "Hello" };
		nodeVec.Add(new HashNode<String, String>(System::move(key0), u"World"));
		String key1{ "Hoge" };
		nodeVec.Add(new HashNode<String, String>(System::move(key1), u"HogeHoge"));
		String key2{ "Piyo" };
		nodeVec.Add(new HashNode<String, String>(System::move(key2), u"PiyoPiyo"));

		for (auto& x : nodeVec) std::cout << x->GetValue<0>() << std::endl;
		std::cout << "Block end." << std::endl;
	}

	if constexpr(true) {
		HashMap<int, String> intmap = 3;
		intmap.Insert(16, u"100");
		intmap.Insert(32, u"400");
		intmap.Insert(48, u"900");

		{
			std::cout << "Test Start." << std::endl;

			for (HashNode<int, String>& x : intmap) std::cout << x.GetValue() << std::endl;

			IEnumerator<HashNode<int, String>> ie = intmap.GetEnumerator(false);

			auto ite = ie.begin();
			auto e = ie.end();
			if (ite != e) {
				HashNode<int, String>& x = *ite;
				std::cout << x.GetValue() << std::endl;
			}

			std::cout << "Test End." << std::endl;
		}

		for (HashNode<int, String>& x : intmap.GetEnumerator(false)) std::cout << x.GetKey() << ":" << x.GetValue() << std::endl;

		if constexpr (true) {
			std::cout << "Block start." << std::endl;

			String c16 = u"Hello";
			std::wcout << c16.w_str() << std::endl;
			std::cout << c16 << std::endl;

			Select sel(intmap.GetEnumerator(false));
			IEnumerator<String> strIe = sel.Get();
#if !defined(__GNUC_) || defined(__clang__)
			++strIe;
#endif
			bool cond = !strIe.Done();
			if (cond) {
				String& str = *strIe;
				std::cout << str << std::endl;
				++strIe;
			}
			cond = !strIe.Done();
			if (cond) {
				String& str = *strIe;
				std::cout << str << std::endl;
				++strIe;
			}
			cond = !strIe.Done();
			if (cond) {
				String& str = *strIe;
				std::cout << str << std::endl;
				++strIe;
			}
			cond = !strIe.Done();
			if (cond) {
				String& str = *strIe;
				std::cout << str << std::endl;
				++strIe;
			}

			std::cout << "----" << std::endl;
			{
				auto ie = sel.Get();
				for (auto ite = ie.begin(), e = ie.end(); ite != e; ++ite) {
					String& x = *ite;
					std::cout << x << std::endl;
				}
			}
			std::cout << "Block end." << std::endl;
		}
		std::cout << "HashMap start." << std::endl;
		MemberSelectEnumerator<String, HashNode<int, String>> select { new IEnumerator<HashNode<int, String>>(intmap.GetEnumerator(false)), &HashNode<int, String>::m_value };
		for (String& x : select) std::cout << x << std::endl;
		std::cout << "HashMap end." << std::endl;

		std::cout << "intIe start." << std::endl;
		IEnumerable<int&> intie = intmap.Select(
			Function([](HashNode<int, String>& node) -> int& { return node.GetKey(); })
		).Where([](int& x) { return x % 3 != 0; });
		for (int& x : intie) std::cout << x << std::endl;
		
		IEnumerable<int const&> cintie = static_cast<const HashMap<int, String>&>(intmap).Select(
			Function([](HashNode<int, String> const& node) -> int const& { return node.GetKey(); })
		).Where([](int const& x) { return x % 3 != 0; });
		for (int const& x : cintie) std::cout << x << std::endl;

		std::cout << "intIe end." << std::endl;
	}

	{
		HashMap<String, String> map = 3;
		map.Insert(u"Hello", u"World");
		std::cout << "1" << std::endl;
		map.Insert(u"Hoge", u"HogeHoge");
		std::cout << "2" << std::endl;
		map.Insert(u"Piyo", u"Piyopiyo");
		std::cout << "3" << std::endl;
		for (String& x : map.Values()) {
			std::cout << x << std::endl;
		}
		std::cout << "4" << std::endl;
	}
	std::cout << "5" << std::endl;
	

	std::cout << "CoroutineTest6 end." << std::endl;
}

void font_test() noexcept {
	String path = ResourcePaths::GetPathName(u"./Font/century.ttf");
	ManagedObject<FontFactory> factory = FontFactory::GetFontFactory();
	bool success = factory->SetFont(path, u"Century");
	IFont* century = factory->GetFont(u"Century");
	std::cout << path << std::endl;
	if (!century) return;
	std::cout << "Load Success!" << std::endl;
	century->SetBaseLineColor(Drawing::Colors::White);
	century->SetContoursColor(Drawing::Colors::Black);
	Drawing::Image image(1920, 1080);
	uint32_t permilli = 50;
	int32_t originy = century->GetAscenderLine(permilli);
	if (originy < 0) originy = 0;
	for (bool b : century->UpdateStringImage(image, u"Hello World!", Point<int32_t>(0, originy), permilli)) {}
	String savePath = ResourcePaths::ExePath.CreatePath(u"./Hello_World.bmp").PathName();
	Drawing::BMP::FromImage(image).Save(savePath);
}

void macro_test() noexcept {
#if !defined(_WIN32) && !defined(_WIN64)
	static_assert(false, "Not Windows.");
#endif
}

int main(int argc, char** argv) {
	//リソースパスのルートを設定
	ResourcePaths::RootPath = String(u"C:/source/vscode/3DApplication/main/Resources");
	ResourcePaths::ExePath = String(argv[0]);

	std::cout << ResourcePaths::RootPath.PathName() << std::endl;
	std::cout << ResourcePaths::ExePath.PathName() << std::endl;

	CoroutineTest1();
	CoroutineTest2();
	CoroutineTest3();
	CoroutineTest4();
	CoroutineTest5();
	CoroutineTest6();

	font_test();
	macro_test();

	return 0;
}
