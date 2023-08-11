//g++では、class Stringの宣言(in String.hpp)が実体化されておらず、基底クラスのStringBlock<char16_t>が
//Undefined Referenceになる。
//ここで再度読み込むことでコンパイルには成功するが...。
//#include "System/Module/Objects/String.hpp"

//g++ではModule内でatexit()(in stdlib.h)のコンフリクトが起きてコンパイルエラーとなる。(Thread -> thread -> .. -> cstdlib -> stdlib.h)
//StringBlock, Thread, Col, iostreamの同時importはコンパイル可能であるため、
//どこかで異なるヘッダをincludeしている可能性がある。
//import Module;

import CSTDINT;
import Objects_Internal;
import StringBlock;
import Thread;

import Col;
import <iostream>;

int func_null();

//IEnumerator<int> En(int, double){
//	for (int i = 0; i < 5; ++i) co_yield i;
//}

int main(int argc, char **argv)
{
	std::cout << "main start." << std::endl;

	System::StringBlock<char16_t> str = u"Hello";
	System::StringBlock<char16_t> str2;
	System::String str3;

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
