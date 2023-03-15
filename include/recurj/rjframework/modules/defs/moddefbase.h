#pragma once
#ifdef rjbaselibrary
#ifdef _rjOS_FAMILY_UNIX
#define RJBASE_API __attribute__ ((visibility ("default")))
#endif 
#ifdef _rjOS_WIN
#define RJBASE_API __declspec(dllexport)
#endif 
#define RJBASE_TEMPLATE
#else 
#ifdef _rjOS_FAMILY_UNIX
#define RJBASE_API
#endif 
#ifdef _rjOS_WIN
#define RJBASE_API __declspec(dllimport)
#endif 
#define RJBASE_TEMPLATE extern
#endif 

