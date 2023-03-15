#pragma once
#ifdef rjdblibrary
#ifdef _rjOS_FAMILY_UNIX
#define RJDB_API __attribute__ ((visibility ("default")))
#endif 
#ifdef _rjOS_WIN
#define RJDB_API __declspec(dllexport)
#endif 
#define RJDB_TEMPLATE
#else 
#ifdef _rjOS_FAMILY_UNIX
#define RJDB_API
#endif 
#ifdef _rjOS_WIN
#define RJDB_API __declspec(dllimport)
#endif 
#define RJDB_TEMPLATE extern
#endif 

