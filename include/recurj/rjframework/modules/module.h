#pragma once
#ifdef _rjOS_FAMILY_UNIX
#include <stddef.h>
#define DLL_ENTRY __attribute__ ((visibility ("default")))
#endif 
#ifdef _rjOS_WIN
#include <cstdint>
#define DLL_ENTRY __declspec(dllexport)
#endif 
#include <recurj/rjframework/cpp.h>

