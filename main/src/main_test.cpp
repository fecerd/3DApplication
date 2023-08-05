import <iostream>;
import CSTDINT;
//import Time;
//import Function;
//import IEnumerable;
//import Exception;
using namespace System;

int func_null();

//IEnumerator<int> En(int, double){
//	for (int i = 0; i < 5; ++i) co_yield i;
//}

import Col;
import StringBlock;
import Objects;

// namespace System {
// 	template<>
// 	StringBlock<char16_t>::StringBlock() noexcept {}
// 	template<>
// 	StringBlock<char16_t>::StringBlock(char16_t const*, size_t) noexcept {}
// 	template<>
// 	StringBlock<char16_t>::~StringBlock() noexcept {}
// }

int main(int argc, char **argv)
{
	std::cout << "main start." << std::endl;

	StringBlock<char16_t> str = u"Hello";
	StringBlock<char16_t> str2;
	String str3;

	auto col = Test::CreateCol();
	for (auto ite = col.begin(), e = col.end(); ite != e; ++ite) {
		auto& x = *ite;
		std::cout << x << std::endl;
	}

	// for (auto ite = col.begin(), e = col.end(); ite != e; ++ite) {
	// 	std::cout << "loop:" << i++ << std::endl;
	// 	if (!ite.m_obj) {
	// 		std::cout << "no obj" << std::endl;
	// 		break;
	// 	}
	// 	else {
	// 		auto& p = ite.m_obj->m_handle.promise();
	// 		int* v = p.value;
	// 		if (!v) {
	// 			std::cout << "no value ptr" << std::endl;
	// 			break;
	// 		}
	// 	}
	// 	int& x = *ite;
	// 	std::cout << x << std::endl;
	// }
	//for  (int& x : col) std::cout << x << std::endl;

	//Vector<int> vec;
	//vec.Add(100);

	//Timer timer;
	//timer.Start();

	return func_null();
}
