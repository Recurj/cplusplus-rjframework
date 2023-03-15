#pragma once
#ifdef rjstorageoslibrary
#ifdef _rjOS_FAMILY_UNIX
#define RJSTORAGEOS_API __attribute__ ((visibility ("default")))
#endif 
#ifdef _rjOS_WIN
#define RJSTORAGEOS_API __declspec(dllexport)
#endif 
#define RJSTORAGEOS_TEMPLATE
#else 
#ifdef _rjOS_FAMILY_UNIX
#define RJSTORAGEOS_API
#endif 
#ifdef _rjOS_WIN
#define RJSTORAGEOS_API __declspec(dllimport)
#endif 
#define RJSTORAGEOS_TEMPLATE extern
#endif 

