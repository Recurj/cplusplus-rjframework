#pragma once
#ifdef rjnetlibrary
#ifdef _rjOS_FAMILY_UNIX
#define RJNET_API __attribute__ ((visibility ("default")))
#endif 
#ifdef _rjOS_WIN
#define RJNET_API __declspec(dllexport)
#endif 
#define RJNET_TEMPLATE
#else 
#ifdef _rjOS_FAMILY_UNIX
#define RJNET_API
#endif 
#ifdef _rjOS_WIN
#define RJNET_API __declspec(dllimport)
#endif 
#define RJNET_TEMPLATE extern
#endif 

