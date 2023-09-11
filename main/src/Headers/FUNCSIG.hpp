#pragma once
#ifndef FUNCSIG_H
//#define FUNCSIG_H

#if defined(__clang__) || defined(__GNUC__)	//Clang or GCC
#define __FUNCSIG__ __PRETTY_FUNCTION__
#elif defined(_MSC_VER)	//MSVC(cl) or MSBUILD
#else	//Others
#endif

#endif