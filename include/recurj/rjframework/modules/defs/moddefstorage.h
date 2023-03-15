#pragma once
#ifdef uwlstoragelibrary
#ifdef _rjOS_FAMILY_UNIX
#define RJXML_API __attribute__ ((visibility ("default")))
#endif 
#ifdef _rjOS_WIN
#define UWLSTORAGE_API __declspec(dllexport)
#endif 
#define UWLSTORAGE_TEMPLATE
#else 
#ifdef _rjOS_FAMILY_UNIX
#define UWLSTORAGE_API
#endif 
#ifdef _rjOS_WIN
#define UWLSTORAGE_API __declspec(dllimport)
#endif 
#define UWLSTORAGE_TEMPLATE extern
#endif 

