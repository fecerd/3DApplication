module;
#if defined(__GNUC__) && !defined(__clang__)
#include <boost/thread/thread.hpp>
#endif
export module BoostThread;
//Boostライブラリは現状g++のみ使用可能
#if defined(__GNUC__) && !defined(__clang__)

export namespace System::Boost {
	using Thread = boost::thread;
}

#endif
