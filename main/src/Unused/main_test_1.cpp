﻿//clangの場合、#includeはファイルの先頭になければならない
#include "System/Module/FUNCSIG.hpp"
import <new>;	//placement new
//#define DEFINE_CRTDBG_new
#include "CRTDBG/crtdbg_wrapper.hpp"
import <iostream>;
import <cassert>;
import <source_location>;
import <functional>;	//std::hash

import System;
using namespace System;

int func_null();

template<class T>
struct Con { T* m_ptr = nullptr; };

void test_func0() {
	StringBlock<char16_t> str = u"Hello";
	String str3 = u"Hello";

	std::wcout << str3.w_str() << std::endl;
}

void test_func1() {
	System::Thread th1 = System::Thread(
		[](){ 
			for (int i = 0; i < 10; ++i) std::cout << i << std::endl;
			//return 0;
		}
	);
	th1.join();

	auto f2 = []() {
		for (int i = 0; i < 10; ++i) std::cout << i << std::endl;
	};
	System::Thread th2 = System::Thread(f2);
	th2.join();

	auto f3 = [](int x) {
		for (int i = 0; i < x; ++i) std::cout << i << std::endl;
	};
	System::Thread th3 = System::Thread(f3, 10);
	th3.join();

	auto f4 = [](int b, int e) {
		try {
			for (int i = b; i < e; ++i) std::cout << i << std::endl;
		} catch (std_exception& e) {
			std::cout << e.what() << std::endl;
			return;
		}
	};
	System::Thread th4 = System::Thread(f4, 0, 10);
	th4.join();

	std::cout << "Thread End." << std::endl;

	UniquePtr<int> i { new int(10) };

	{
		Allocator<int> alloc;
		int* i2 = alloc.allocate(1);
		using traits = AllocatorTraits<Allocator<int>>;
		traits::construct(alloc, i2, 10);
		traits::destroy(alloc, i2);
		alloc.deallocate(i2, 1);		
	}

	{
		using type = int;
		Allocator<type> alloc;
		using traits = AllocatorTraits<decltype(alloc)>;
		type* ptr = traits::allocate(alloc, 1);
		traits::construct(alloc, ptr, 10);
		std::cout << *ptr << std::endl;
		traits::destroy(alloc, ptr);
		alloc.deallocate(ptr, 1);
	}

	{
		using type = Vector3;
		Allocator<type> alloc;
		using traits = AllocatorTraits<decltype(alloc)>;
		type* ptr = traits::allocate(alloc, 1);
		traits::construct(alloc, ptr, Vector3(1, 2, 3));
		std::cout << ptr->ToString() << std::endl;
		traits::destroy(alloc, ptr);
		alloc.deallocate(ptr, 1);
	}

	{
		using type = Thread;
		Allocator<type> alloc;
		using traits = AllocatorTraits<decltype(alloc)>;
		type* ptr = traits::allocate(alloc, 1);
		traits::construct(alloc, ptr);
		traits::destroy(alloc, ptr);
		alloc.deallocate(ptr, 1);
	}

	{
		using type = Thread;
		UniquePtr<type> th1 = MakeUnique<type>();
		th1->join();
	}

	{
		Thread* threads = DefaultNew<Thread[]>()(4);
		UniquePtr<Thread[]> th1 { threads };
		for (size_t i = 0; i < 4; ++i) th1[i].join();
	}

	if constexpr (true) {
		int i = 0;

		TaskWorker* worker = new TaskWorker(1);
		TaskFuture<bool, bool> fence = worker->PushFence(1, false);
		TaskFuture<bool, void> f1 = worker->Push<bool, void>(
			[i](TaskPromise<bool, void>& promise) mutable {
				std::cout << i++ << std::endl;
			},
			2, false
		);
		fence.Wait(TaskState::Suspended);
		fence.Run();
		std::cout << "delete worker." << std::endl;
		delete worker;
	}

	std::cout << cout_green << "allocator test end." << cout_reset << std::endl;

	Disposer* disposer = new Disposer();
	Con<int> con;
	con.m_ptr = new int(10);
	System::Function f = [](Con<int>& x){ delete x.m_ptr; x.m_ptr = nullptr; };
	disposer->Push<Con<int>>(
		System::move(con), 1, f
	);
	std::cout << *con.m_ptr << std::endl;
	disposer->Update();
	Timer::Sleep(milliseconds(100));
	std::cout << *con.m_ptr << std::endl;	
	delete disposer;
	std::cout << *con.m_ptr << std::endl;
}

template<class ...Args>
void get() {
	//std::source_location loc = std::source_location::current();
	//std::cout << loc.function_name() << std::endl;
	std::cout << __FUNCSIG__ << std::endl;
}

void test_func2() { get<int, double>(); }

struct T1 {
	int i = 0;
	double d = 0.0;
	float f = 0.f;
public:
	constexpr T1() = default;
	constexpr T1(int _i) : T1(_i, 0.0, 0.f) {}
	constexpr T1(int _i, double _d, float _f) : i(_i), d(_d), f(_f) {}
};

void test_func3() {
	UniquePtr<int> ptr1 { new int(10) };
	UniquePtr<T1> ptr2 = MakeUnique<T1>(10, 15.0, 20.f);
	UniquePtr<T1[]> ptr3 = MakeUnique<T1[]>(5);
	auto c1 = ptr3 <=> nullptr;
	auto c2 = nullptr <=> ptr3;
	auto c3 = ptr2 <=> nullptr;
	auto c4 = nullptr <=> ptr2;
	UniquePtr<char[]> ptr4 = MakeUnique<char[]>(13);
	int i = 0;
	for (const char& c : "Hello World!") ptr4[i++] = c;
	std::cout << ptr4 << std::endl;

	size_t h = std::hash<decltype(ptr4)>()(ptr4);

	std::cout << sizeof(int*) << std::endl;
	std::cout << sizeof(T1*) << std::endl;
}

template<class T>
struct Deleter {
	void operator()(T* ptr) const { delete ptr; }
};
template<class T>
struct Deleter<T[]> {
	void operator()(T* ptr) const { delete[] ptr; }
};

void test_func4() {
	Deleter<int> delInt;
	Deleter<T1> delT1;
	Deleter<T1[]> delT1Arr;
	UniquePtr<int> ptr1_1 = MakeUnique<int>(11);
	UniquePtr<int, decltype(delInt)> ptr1_2{ new int(12) };

	auto c1_1 = ptr1_1 <=> ptr1_2;

	UniquePtr<T1> ptr2_1 = MakeUnique<T1>(21, 0.0, 0.f);
	UniquePtr<T1, decltype(delT1)> ptr2_2{ new T1(22, 0.0, 0.f) };

	UniquePtr<T1[]> ptr3_1 = MakeUnique<T1[]>(1);
	UniquePtr<T1[], decltype(delT1Arr)> ptr3_2{ new T1[3]() };

	auto c3_1 = ptr3_1 <=> ptr3_2;
}

struct T2 {
	static int get() {
		static int ret = 0;
		return ret++;
	}
	int m_value;
	T2() : m_value(get()) {}
};

template<class A, class Y>
SharedPtr<A> func6_internal(SharedPtr<A> shared, Y* ptr) {
	using type = Traits::remove_all_extents_t<A>;
	type* p = nullptr;
	return SharedPtr<A>(p);
}

void test_func5() {
	SharedPtr<int> ptr1_1 { new int(11) };
	int* p1_1 = ptr1_1.Get();
	SharedPtr<int> ptr1_2 { new int(12), Deleter<int>() };
	ptr1_1.Swap(ptr1_2);

	SharedPtr<T1> ptr2_1 { new T1(21) };
	T1* p2_1 = ptr2_1.Get();
	SharedPtr<T1> ptr2_2 { new T1(22), Deleter<T1>() };
	ptr2_1.Swap(ptr2_2);
	float f = 5.f;
	SharedPtr<T1> ptr2_3 = MakeShared<T1>(13, 3.0, f);
	T1* p2_3 = ptr2_3.Get();
	std::cout << p2_3->i << ", " << p2_3->d << ", " << p2_3->f << std::endl;

	SharedPtr<T1[]> ptr3_1 { new T1[3]() };
	T1* p3_1 = ptr3_1.Get();
	SharedPtr<T1[]> ptr3_2 { new T1[3](), Deleter<T1[]>() };
	ptr3_1.Swap(ptr3_2);
}

template<class T>
concept CCanOutput = requires(T const& x) {
	{ std::cout << x << std::endl } -> Traits::Concepts::convertible_to<decltype(std::cout)&>;
};

template<class T>
void func6_internal(const SharedPtr<T>& shared, size_t count = 1) {
	std::cout << __FUNCSIG__ << std::endl;
	Traits::remove_all_extents_t<T>* p1 = shared.Get();
	auto p2 = reinterpret_cast<Traits::remove_all_extents_t<T>*>(p1);
	for (size_t i = 0; i < Traits::element_count_v<T> * count; ++i) {
		if constexpr (CCanOutput<Traits::remove_all_extents_t<T>>) std::cout << p2[i] << std::endl;
		else if constexpr (Traits::is_same_v<decltype(p2), T1*>) std::cout << p2[i].i << ", " << p2[i].d << ", " << p2[i].f << std::endl;
		else std::cout << p2[i].m_value << std::endl;
	}
}

void test_func6() {
	//1
	SharedPtr<T1> ptr1 = MakeShared<T1>();
	//2
	SharedPtr<T1> ptr2 = MakeShared<T1>(10, 11.0, 12.f);
	//3
	SharedPtr<T2[]> ptr3 = MakeShared<T2[]>(5);
	//4
	SharedPtr<T2[][4][3][2][1]> ptr4 = MakeShared<T2[][4][3][2][1]>(5);
	//5
	SharedPtr<T2[5]> ptr5 = MakeShared<T2[5]>();
	//6
	SharedPtr<T2[5][4][3][2][1]> ptr6 = MakeShared<T2[5][4][3][2][1]>();
	//7
	T2 init1 = {};
	SharedPtr<T2[]> ptr7 = MakeShared<T2[]>(5, init1);
	//8
	T2 init2[4][3][2][1] = {};
	SharedPtr<T2[][4][3][2][1]> ptr8 = MakeShared<T2[][4][3][2][1]>(5, init2);
	//9
	SharedPtr<T2[5]> ptr9 = MakeShared<T2[5]>(init1);
	//10
	SharedPtr<T2[5][4][3][2][1]> ptr10 = MakeShared<T2[5][4][3][2][1]>(init2);
	//11
	SharedPtr<T1> ptr11 = MakeShared<T1>(T1(15));
	//12
	SharedPtr<T2> ptr12 = MakeSharedForOverwrite<T2>();
	//13
	SharedPtr<T2[5]> ptr13 = MakeSharedForOverwrite<T2[5]>();
	//14
	SharedPtr<T2[5][4][3][2][1]> ptr14 = MakeSharedForOverwrite<T2[5][4][3][2][1]>();
	//15
	SharedPtr<T2[]> ptr15 = MakeSharedForOverwrite<T2[]>(5);
	//16
	SharedPtr<T2[][4][3][2][1]> ptr16 = MakeSharedForOverwrite<T2[][4][3][2][1]>(5);
	
	func6_internal(ptr1);
	func6_internal(ptr2);
	func6_internal(ptr3, 5);
	func6_internal(ptr4, 5);
	func6_internal(ptr5);
	func6_internal(ptr6);
	func6_internal(ptr7, 5);
	func6_internal(ptr8, 5);
	func6_internal(ptr9);
	func6_internal(ptr10);
	func6_internal(ptr11);
	func6_internal(ptr12);
	func6_internal(ptr13);
	func6_internal(ptr14);
	func6_internal(ptr15, 5);
	func6_internal(ptr16, 5);
}

void test_func6_int() {
	//1
	SharedPtr<int> ptr1 = MakeShared<int>();
	//2
	SharedPtr<int> ptr2 = MakeShared<int>(10);
	//3
	SharedPtr<int[]> ptr3 = MakeShared<int[]>(5);
	//4
	SharedPtr<int[][4][3][2][1]> ptr4 = MakeShared<int[][4][3][2][1]>(5);
	//5
	SharedPtr<int[5]> ptr5 = MakeShared<int[5]>();
	//6
	SharedPtr<int[5][4][3][2][1]> ptr6 = MakeShared<int[5][4][3][2][1]>();
	//7
	int init1 = { 1 };
	SharedPtr<int[]> ptr7 = MakeShared<int[]>(5, init1);
	//8
	int init2[4][3][2][1] = {};
	for (int i = 0; i < 24; ++i) reinterpret_cast<int*>(init2)[i] = i;
	SharedPtr<int[][4][3][2][1]> ptr8 = MakeShared<int[][4][3][2][1]>(5, init2);
	//9
	SharedPtr<int[5]> ptr9 = MakeShared<int[5]>(init1);
	//10
	SharedPtr<int[5][4][3][2][1]> ptr10 = MakeShared<int[5][4][3][2][1]>(init2);
	//11
	SharedPtr<int> ptr11 = MakeShared<int>(int(15));
	//12
	SharedPtr<int> ptr12 = MakeSharedForOverwrite<int>();
	//13
	SharedPtr<int[5]> ptr13 = MakeSharedForOverwrite<int[5]>();
	//14
	SharedPtr<int[5][4][3][2][1]> ptr14 = MakeSharedForOverwrite<int[5][4][3][2][1]>();
	//15
	SharedPtr<int[]> ptr15 = MakeSharedForOverwrite<int[]>(5);
	//16
	SharedPtr<int[][4][3][2][1]> ptr16 = MakeSharedForOverwrite<int[][4][3][2][1]>(5);
	
	func6_internal(ptr1);
	func6_internal(ptr2);
	func6_internal(ptr3, 5);
	func6_internal(ptr4, 5);
	func6_internal(ptr5);
	func6_internal(ptr6);
	func6_internal(ptr7, 5);
	func6_internal(ptr8, 5);
	func6_internal(ptr9);
	func6_internal(ptr10);
	func6_internal(ptr11);
	func6_internal(ptr12);
	func6_internal(ptr13);
	func6_internal(ptr14);
	func6_internal(ptr15, 5);
	func6_internal(ptr16, 5);
}

class Hoge {
	String m_str;
public:
	explicit Hoge(const String& str) : m_str(str) {
		std::cout << "Hoge::Hoge(" << m_str << ")" << std::endl;
	}
	~Hoge() {
		std::cout << "Hoge::~Hoge(" << m_str << ")" << std::endl;
	}
};

struct Fuga;

struct Piyo {
	SharedPtr<Fuga> p;
	~Piyo() {
		std::cout << "Piyo::~Piyo()" << std::endl;
	}
};
struct Fuga {
	WeakPtr<Piyo> p;
	~Fuga() {
		std::cout << "Fuga::~Fuga()" << std::endl;
	}
};

void test_func7() {
	if (true) {
		SharedPtr<Hoge> hoge_ptr0;
		{
			std::cout << "---Construct A, B and C.---" << std::endl;
			SharedPtr<Hoge> hoge_ptr1(new Hoge(u"A"));
			SharedPtr<Hoge> hoge_ptr2(new Hoge(u"B"));
			SharedPtr<Hoge> hoge_ptr3(new Hoge(u"C"));

			assert(hoge_ptr1.UseCount() == 1);
			assert(hoge_ptr2.UseCount() == 1);
			assert(hoge_ptr3.UseCount() == 1);

			std::cout << "---Destruct A.---" << std::endl;
			hoge_ptr0 = hoge_ptr1 = hoge_ptr2;

			assert(hoge_ptr2.UseCount() == 3);
			assert(hoge_ptr3.UseCount() == 1);
			std::cout << "---Destruct C.---" << std::endl;
		}
		std::cout << "---Destruct B.---" << std::endl;
	}
	if (true) {
		WeakPtr<Hoge> weak;
		{
			SharedPtr<Hoge> shared0(new Hoge(u"A"));
			assert(shared0.UseCount() == 1);
			SharedPtr<Hoge> shared1 = shared0;
			assert(shared0.UseCount() == 2);
			weak = shared0;
			assert(shared0.UseCount() == 2);
		}
		assert(weak.Expired());
	}
	if (true) {
		SharedPtr<Piyo> piyo_ptr(new Piyo());
		SharedPtr<Fuga> fuga_ptr(new Fuga());

		piyo_ptr->p = fuga_ptr;
		fuga_ptr->p = WeakPtr<Piyo>(piyo_ptr);
	}
}

void test_func8() {
	std::cout << cout_green << "test_func8() Start." << cout_reset << std::endl;

	const auto lam1 = [](TaskPromise<bool, bool>& p) { p.Suspend(); };
	Function f1{ [](TaskPromise<bool, bool>& p) { p.Suspend(); } };
	Function f2{ lam1 };

	const auto lam2 = [](T1& x) { std::cout << x.i << std::endl; ++x.i; };
	Function f3 { [](T1& x) { std::cout << x.i << std::endl; ++x.i; } };
	Function f4 { lam2 };
	T1 x = { 15 };
	f3(x);
	f4(x);
	f4(T1(100));

	Function<void(T1&)> f5 = f3;
	f5(x);
	Function<void(T1&)> f6 = System::move(f3);
	f6(x);
	f6.Reset();

	Function<void(void)> f7;
	f7 = Function{ []{ std::cout << 150 << std::endl; } };
	f7();
	Function<void()> f8 = []{ std::cout << 200 << std::endl; };
	f8();
	f8 = f7;
	f8();
	Function f9 = [](int x, double& y) { return y = x * y; };
	double d = 1.5;
	f9(10, d);
	std::cout << d << std::endl;
	std::cout << cout_green << "test_func8() End." << cout_reset << std::endl;
}

struct T3 {
	int m_data = 1;
	T3() = default;
	T3(int data) : m_data(data) {}
	int func(int x, int y) { return x * y * m_data; }
	int func_n(int x, int y) { return x * y * m_data; }
	int func_c(int x, int y) const { return x * y * m_data; }
	int func_cv(int x, int y) const volatile { return x * y * m_data; }
	int func_cn(int x, int y) const { return x * y * m_data; }
	int func_rx(int& x, int y) { x *= 2; return x * y * m_data; }
	int func_ry(int x, int& y) { y *= 2; return x * y * m_data; }
	int func_rxy(int& x, int& y) { x *= 2; y *= 2; return x * y * m_data; }
};

int t3_function(T3 t3, int x, int y) { return t3.func(x, y); }
int t3_function_c(T3 const t3, int x, int y) { return t3.func_c(x, y); }
int t3_func(T3* t3, int x, int y) { return t3->func(x, y); }
int t3_func_r(T3& t3, int x, int y) { return t3.func(x, y); }
int t3_func_cr(T3 const& t3, int x, int y) { return t3.func_c(x, y); }
int t3_func_c(T3 const* t3, int x, int y) { return t3->func_c(x, y); }
int t3_func_cv(T3 const volatile* t3, int x, int y) { return t3->func_cv(x, y); }
int t3_func_rx(T3* t3, int& x, int y) { return t3->func_rx(x, y); }
int t3_func_ry(T3* t3, int x, int& y) { return t3->func_ry(x, y); }
int t3_func_rxy(T3* t3, int& x, int& y) { return t3->func_rxy(x, y); }
int t3_func_cp(T3* const t3, int x, int y) { return t3->func(x, y); }

struct T3Func {
	static int func(T3 t3, int x, int y) {
		t3.m_data += 1;
		return t3.func_cv(x, y);
	}
	static int func_c(T3 const t3, int x, int y) {
		//t3.m_data += 1;
		return t3.func_cv(x, y);
	}
	static int func_p(T3* t3, int x, int y) {
		t3->m_data += 1;
		return t3->func_cv(x, y);
	}
	static int func_r(T3& t3, int x, int y) {
		t3.m_data += 1;
		return t3.func_cv(x, y);
	}
	static int func_cr(T3 const& t3, int x, int y) {
		//t3.m_data += 1;
		return t3.func_cv(x, y);
	}
	static int func_cp(T3 const* t3, int x, int y) {
		//t3->m_data += 1;
		return t3->func_cv(x, y);
	}
	static int func_cvp(T3 const volatile* t3, int x, int y) {
		//t3->m_data += 1;
		return t3->func_cv(x, y);
	}
	static int func_cvr(T3 const volatile& t3, int x, int y) {
		//t3.m_data += 1;
		return t3.func_cv(x, y);
	}
	static int func_p_c(T3* const t3, int x, int y) {
		t3->m_data += 1;
		return t3->func_cv(x, y);
	}
};

void test_func9() {
	std::cout << cout_cyan << "test_func9() Start." << cout_reset << std::endl;

	Function<void(void)> global = &test_func8;
	Function global_dd = &test_func8;
	if (false) {
		global_dd.Reset();
		global_dd = System::move(global);
		global_dd();
	}
	T3 obj = T3(3);
	const T3 cobj = T3(7);
	Function<int(T3*, int, int)> mem_p = &T3::func;
	Function<int(T3&, int, int)> mem_r = &T3::func;
	Function mem_dd = &T3::func;
	Function mem_dd2 = mem_r;
	{
		std::cout << mem_p(&obj, 1, 1) << std::endl;
		std::cout << mem_r(obj, 1, 1) << std::endl;
		std::cout << mem_dd(obj, 1, 1) << std::endl;
		mem_dd = mem_r;
	}
	Function<int(T3 const*, int, int)> mem_c_p = &T3::func_c;
	Function<int(T3 const&, int, int)> mem_c_r = &T3::func_c;
	Function mem_c_dd = &T3::func_c;
	//非const Function <- const関数の代入可能
	Function<int(T3*, int, int)> mem_p2 = &T3::func_c;
	{
		std::cout << mem_c_p(&obj, 1, 1) << std::endl;
		std::cout << mem_c_r(obj, 1, 1) << std::endl;
		std::cout << mem_c_dd(obj, 1, 1) << std::endl;
		std::cout << mem_c_p(&cobj, 1, 1) << std::endl;
		std::cout << mem_c_r(cobj, 1, 1) << std::endl;
		std::cout << mem_c_dd(cobj, 1, 1) << std::endl;
		mem_c_dd = mem_c_r;
		mem_p2 = mem_c_p;
		std::cout << mem_p2(&obj, 1, 1) << std::endl;
	}
	Function mem_cn_r = &T3::func_cn;
	Function mem_n_r = &T3::func_n;
	Function mem_rx = &T3::func_rx;
	Function mem_ry = &T3::func_ry;
	Function mem_rxy = &T3::func_rxy;
	{
		std::cout << mem_cn_r(obj, 1, 1) << std::endl;
		std::cout << mem_n_r(obj, 1, 1) << std::endl;
		int x = 1;
		int y = 1;
		std::cout << mem_rx(obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;
		std::cout << mem_ry(obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;
		std::cout << mem_rxy(obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;

		//mem_p = mem_rx;
	}
	Function func_rx = &t3_func_rx;
	Function func_ry = &t3_func_ry;
	Function func_rxy = &t3_func_rxy;
	{
		int x = 1;
		int y = 1;
		std::cout << func_rx(&obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;
		std::cout << func_ry(&obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;
		std::cout << func_rxy(&obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;
	}
	Function lambda = [](T3* t3, int x, int y) { return t3->func(x, y); };
	Function lambda_m = [](T3* t3, int x, int y) mutable { return t3->func(x, y); };
	Function lambda_c = [](T3 const* t3, int x, int y) { return t3->func_c(x, y); };
	Function lambda_n = [](T3* t3, int x, int y) { return t3->func_n(x, y); };
	Function lambda_cn = [](T3 const* t3, int x, int y) { return t3->func_c(x, y); };
	Function lambda_rx = [](T3* t3, int& x, int y) { return t3->func_rx(x, y); };
	Function lambda_ry = [](T3* t3, int x, int& y) { return t3->func_ry(x, y); };
	Function lambda_rxy = [](T3* t3, int& x, int& y) { return t3->func_rxy(x, y); };
	{		
		int x = 1;
		int y = 1;
		std::cout << lambda(&obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;
		std::cout << lambda_m(&obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;
		std::cout << lambda_c(&obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;
		std::cout << lambda_n(&obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;
		std::cout << lambda_cn(&obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;
		std::cout << lambda_rx(&obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;
		std::cout << lambda_ry(&obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;
		std::cout << lambda_rxy(&obj, x, y) << std::endl;
		std::cout << "x: " << x << ", y: " << y << std::endl;
	}

	//cv参照/ポインタにできる動作は非cv参照/ポインタにもできる
	if constexpr (true) {
		//T3 const*->T3* : const*にできる動作は*にもできる
		Function<int(T3*, int, int)> test1 = &T3Func::func_cp;
		//T3 const volatile& -> T3& : const volatile&にできる動作は&にもできる
		Function<int(T3&, int, int)> test2 = &T3Func::func_cvr;
		//T3 const volatile* -> T3 const* : const volatile*にできる動作はconst*にもできる
		Function<int(T3 const*, int, int)> test3 = &T3Func::func_cvp;

		std::cout << "obj: " << obj.m_data << std::endl;
		std::cout << "cobj: " << cobj.m_data << std::endl;

		std::cout << "test1: " << test1(&obj, 1, 1) << std::endl;
		std::cout << "obj: " << obj.m_data << std::endl;
		std::cout << "test2: " << test2(obj, 1, 1) << std::endl;
		std::cout << "obj: " << obj.m_data << std::endl;
		std::cout << "test3: " << test3(&obj, 1, 1) << std::endl;
		std::cout << "obj: " << obj.m_data << std::endl;
		std::cout << "test3: " << test3(&cobj, 1, 1) << std::endl;
		std::cout << "cobj: " << cobj.m_data << std::endl;

	}	
	//参照/ポインタ <-> 右辺値
	if constexpr (true) {

		//int from(T3& x, int y, int z)をint to(T3 x, int y, int z)に入れると、
		//T3 xの変更を捨てつつ呼び出せる。
		Function<int(T3, int, int)> test4 = &T3Func::func_r;
		//int from(T3 x, int y, int z)をint to(T3& x, int y, int z)に入れると、
		//T3& xが変更されないが呼び出せる。
		Function<int(T3&, int, int)> test5 = &T3Func::func;
		Function<int(T3&, int, int)> test6 = &T3Func::func_c;
		Function<int(T3 const&, int, int)> test7 = &T3Func::func_c;
		Function<int(T3* const&, int, int)> test8 = &T3Func::func_p_c;

		std::cout << "test4: " << test4(obj, 1, 1) << std::endl;
		std::cout << "obj: " << obj.m_data << std::endl;
		std::cout << "test4: " << test4(cobj, 1, 1) << std::endl;
		std::cout << "cobj: " << cobj.m_data << std::endl;
		std::cout << "test5: " << test5(obj, 1, 1) << std::endl;
		std::cout << "obj: " << obj.m_data << std::endl;
		std::cout << "test6: " << test6(obj, 1, 1) << std::endl;
		std::cout << "obj: " << obj.m_data << std::endl;
		std::cout << "test7: " << test7(obj, 1, 1) << std::endl;
		std::cout << "obj: " << obj.m_data << std::endl;
		std::cout << "test7: " << test7(cobj, 1, 1) << std::endl;
		std::cout << "cobj: " << cobj.m_data << std::endl;

		T3* const obj_p = &obj;

		std::cout << "test8: " << test8(obj_p, 1, 1) << std::endl;
		std::cout << "obj_p: " << obj_p->m_data << std::endl;

		//int from(T3& x, int y, int z)をint to(T3 const& x, int y, int z)に入れると、
		//T3 const& xが実際には変更される可能性があるため不適。
		//Function<int(T3 const&, int, int)> test9 = &t3_func_r;
	}
	if constexpr (true) {
		Function dd_1 = &T3Func::func;
		Function dd_2 = &T3Func::func_c;
		Function dd_3 = &T3Func::func_cp;
		Function dd_4 = &T3Func::func_cr;
		Function dd_5 = &T3Func::func_cvp;
		Function dd_6 = &T3Func::func_cvr;
		Function dd_7 = &T3Func::func_p;
		Function dd_8 = &T3Func::func_r;
		Function dd_9 = &T3Func::func_p_c;
	}

	std::cout << cout_cyan << "test_func9() End." << cout_reset << std::endl;
}

int main(int argc, char **argv)
{
	SetCrtDbg();
	std::cout << cout_green << "main start." << cout_reset << std::endl;

	try {
		test_func0();
		test_func1();
		test_func2();
		test_func3();
		test_func4();
		test_func5();
		test_func6();
		test_func7();
		test_func8();
		test_func9();
	}
	catch(std_exception& ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}

	int ret = func_null();
	std::cout << cout_green << "main end.(return value: " << ret << ")" << cout_reset << std::endl;
	return ret;
}
