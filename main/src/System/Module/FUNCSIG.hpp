#pragma once
#ifndef FUNCSIG_H
#define FUNCSIG_H

#if defined(__clang__) || defined(__GNUC__)
#define __FUNCSIG__ __PRETTY_FUNCTION__
#elif defined(_MSC_VER)

#else

#endif

#endif