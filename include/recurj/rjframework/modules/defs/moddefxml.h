#pragma once
#ifdef rjxmllibrary
#ifdef _rjOS_FAMILY_UNIX
#define RJXML_API __attribute__ ((visibility ("default")))
#endif 
#ifdef _rjOS_WIN
#define RJXML_API __declspec(dllexport)
#endif 
#define RJXML_TEMPLATE
#else 
#ifdef _rjOS_FAMILY_UNIX
#define RJXML_API
#endif 
#ifdef _rjOS_WIN
#define RJXML_API __declspec(dllimport)
#endif 
#define RJXML_TEMPLATE extern
#endif 

