#pragma once

#define CRTDBG_WRAPPER_H

#ifndef CRTDBG_WRAPPER_H
#define CRTDBG_WRAPPER_H
#include <crtdbg.h>
#ifndef CRTDBG_NODEFINE_NEW
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
