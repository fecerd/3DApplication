#pragma once
#ifndef EnableVirtual_H
#define EnableVirtual_H

#if defined(__GNUC__) && !defined(__clang__)
#define NO_VIRTUAL
#else
#define USE_VIRTUAL
#endif

#endif
