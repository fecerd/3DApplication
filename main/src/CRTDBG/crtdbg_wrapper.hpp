#pragma once

#if defined(_MSC_VER) && defined(_DEBUG)

#ifdef DEFINE_CRTDBG
#ifndef CRTDBG_H
#define CRTDBG_H
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#endif	//CRTDBG_H
#endif	//DEFINE_CRTDBG

#ifdef DEFINE_CRTDBG_new
#ifndef CRTDBG_H
#define CRTDBG_H
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#endif	//CRTDBG_H
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif	//DEFINE_CRTDBG_NEW

#ifdef DEFINE_CRTDBG_NEW
#ifndef CRTDBG_H
#define CRTDBG_H
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#endif	//CRTDBG_H
#define NEW ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif	//DEFINE_CRTDBG_NEW

inline void SetCrtDbg() noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

#else	//defined(_MSC_VER) && defined(_DEBUG)

inline void SetCrtDbg() noexcept {}

#endif	//defined(_MSC_VER) && defined(_DEBUG)
