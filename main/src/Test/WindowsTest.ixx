module;
#include <cstdlib>	//clangに必要
#include <cstddef>	//clangに必要
#include <windows.h>
export module WindowsTest;
import NamespaceHelper;
import VectorBase;

struct DIJOYSTATE {

};
struct DIMOUSESTATE {

};

export namespace Test {
	struct DI {
		DIJOYSTATE joy;
		DIMOUSESTATE mouse;
	public:
		System::VectorBase<int> func() noexcept {
			return System::VectorBase<int>{};
		}
	};
}

