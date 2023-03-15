#pragma once
#define STRICT 1
#include <cstdint>
#ifdef _rjOS_LINUX
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <sys/un.h>
#include <poll.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <dlfcn.h>
using HLIBRARY=void*;
using HOS=int;
#define HOS_INVALID -1
#endif 
#ifdef _rjOS_FAMILY_UNIX
#include <string.h>
#include <cerrno>
#include <wchar.h>
#ifdef _rjOS_MACOS
#include <malloc/malloc.h>
#define SIZE__MEM_ALLOCED(p) malloc_size(p)
#else 
#include <malloc.h>
#define SIZE__MEM_ALLOCED(p) malloc_usable_size(p)
#endif 
#define _MEMCLR_BYTE(_buf)     ::memset(&_buf,0x00,sizeof(_buf))
#define _MEMCLR_BYTE_ARRAY(_buf)     ::memset(_buf,0x00,sizeof(_buf))
#define _MEMCLR_BYTE_POINTER(_buf,_len)     ::memset(_buf,0x00,_len)
#define _MEMSET_BYTE_POINTER(_buf,_char,_len) ::memset((uint8_t *)_buf,_char,_len)
#define _MEMFIND_BYTE_POINTER(_buf,_ch,_len) ::memchr(_buf,_ch,_len)
#define _MEMCOMPARE_BYTE_POINTER(_buf1,_buf2,_len) ::memcmp((_buf1),(_buf2),(_len))
#define _MEMMOVE_BYTE_POINTER(_buf1,_buf2,_len) ::memcpy(_buf1,_buf2,_len)
#define _MEMSET_UNICODE_POINTER(_buf,_char,_len)    ::wmemset(_buf,_char,_len)
#define _MEMFIND_UNICODE_POINTER(_buf,_ch,_len) ::wmemchr(_buf,_ch,_len)
#define _MEMCOMPARE_UNICODE_POINTER(_buf1,_buf2,_len) ::wmemcmp(_buf1,_buf2,_len)
#define _MEMMOVE_UNICODE_POINTER(_buf1,_buf2,_len) ::wmemcpy(_buf1,_buf2,_len)
#define ULABEL(_s) #_s
#define RJDTIME_LOCALTIME(_st,_t) (localtime_r(_t,_st)!=nullptr)
 constexpr size_t cRJOSPollMax=1024;
constexpr int cFileDefOpenReader=O_RDONLY;
constexpr int cFileDefOpenWriter=O_RDWR;
constexpr int cFileDefPermission=S_IRUSR| S_IWUSR;
using filepos_t=off64_t;
#endif 
#ifdef _rjOS_WIN
#define _WINSOCKAPI_
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <windows.h>
#include <shlwapi.h>
#include <winsock2.h>
#include <intrin.h>
#include <memory.h>
#include <wchar.h>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#define SIZE__MEM_ALLOCED(p) _msize(p)
#define _MEMCLR_BYTE(_buf)     __stosb((uint8_t *)&_buf,0x00,sizeof(_buf))
#define _MEMCLR_BYTE_ARRAY(_buf)     __stosb((uint8_t *)_buf,0x00,sizeof(_buf))
#define _MEMCLR_BYTE_POINTER(_buf,_len)    __stosb((uint8_t *)_buf,0x00,_len)
#define _MEMSET_BYTE_POINTER(_buf,_char,_len)    __stosb((uint8_t *)(_buf),(_char),(_len))
#define _MEMFIND_BYTE_POINTER(_buf,_ch,_len) (uint8_t *)::memchr(_buf,_ch,_len)
#define _MEMCOMPARE_BYTE_POINTER(_buf1,_buf2,_len) ::memcmp((_buf1),(_buf2),(_len))
#define _MEMCOMPARE_BYTE(_buf1,_buf2,_len) ::memcmp((&_buf1),(&_buf2),(_len))
#define _MEMMOVE_BYTE_POINTER(_buf1,_buf2,_len) ::memcpy(_buf1,_buf2,_len)
#define _MEMSET_UNICODE_POINTER(_buf,_char,_len) ::wmemset(_buf,_char,_len)
#define _MEMFIND_UNICODE_POINTER(_buf,_ch,_len) ::wmemchr(_buf,_ch,_len)
#define _MEMCOMPARE_UNICODE_POINTER(_buf1,_buf2,_len) ::wmemcmp(_buf1,_buf2,_len)
#define _MEMMOVE_UNICODE_POINTER(_buf1,_buf2,_len) ::wmemcpy(_buf1,_buf2,_len)
 union GlobalInt {
	int32_t i32t;
	uint32_t ui32t;
	DWORD dw;
	int64_t i64t;
	size_t st;
};
using HLIBRARY=HMODULE;
using HOS=HANDLE;
#define HOS_INVALID INVALID_HANDLE_VALUE
#define ULABEL(_s) L#_s
#define EPOCH_BIAS  116444736000000000i64
#define RJDTIME_LOCALTIME(_st,_t) (localtime_s(_st,_t)==0)
 union SDTimeWin {
	uint64_t ft_scalar;
	FILETIME ft_struct;
};
using filepos_t=int64_t;
constexpr size_t cRJOSPollMax=WSA_MAXIMUM_WAIT_EVENTS;
constexpr int cFileDefOpenReader=O_RDONLY| O_BINARY;
constexpr int cFileDefOpenWriter=O_RDWR| O_BINARY;
constexpr int cFileDefPermission=_S_IREAD| _S_IWRITE;
constexpr DWORD cHWaitEndless=0xFFFFFFFF;
#endif 
using rjPointer = uint8_t*;
