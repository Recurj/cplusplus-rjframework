#pragma once
#ifdef rjsecuritylibrary
#ifdef _rjOS_FAMILY_UNIX
#define RJSECURITY_API __attribute__ ((visibility ("default")))
#endif 
#ifdef _rjOS_WIN
#define RJSECURITY_API __declspec(dllexport)
#endif 
#define RJSECURITY_TEMPLATE
#else 
#ifdef _rjOS_FAMILY_UNIX
#define RJSECURITY_API
#endif 
#ifdef _rjOS_WIN
#define RJSECURITY_API __declspec(dllimport)
#endif 
#define RJSECURITY_TEMPLATE extern
#endif 

