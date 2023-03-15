#pragma once
#ifdef rjwwwlibrary
#ifdef _rjOS_FAMILY_UNIX
#define RJWWW_API __attribute__ ((visibility ("default")))
#endif 
#ifdef _rjOS_WIN
#define RJWWW_API __declspec(dllexport)
#endif 
#define RJWWW_TEMPLATE
#else 
#ifdef _rjOS_FAMILY_UNIX
#define RJWWW_API
#endif 
#ifdef _rjOS_WIN
#define RJWWW_API __declspec(dllimport)
#endif 
#define RJWWW_TEMPLATE extern
#endif 

