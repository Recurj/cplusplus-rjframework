#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/strings/unicode/allocated.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/datatools/atomicnumber.h>
#include <recurj/rjframework/libs/base/application/memory.h>
#include <recurj/rjframework/libs/base/application/application.h>
#include <recurj/rjframework/libs/base/collections//comporator.h>
#include <recurj/rjframework/libs/base/logger/writers/task.h>
#pragma warning(suppress: 4073)
#pragma init_seg (lib)
using namespace rjBase;
CRJApplication* CRJApplication::s_pApplication=nullptr;
const CRJStringByte CRJApplication::s_NoMemory("Not enough memory");
CRJAllocatorDefault CRJApplicationMemory::m_clAllocatorDefault;
#ifdef _rjOS_FAMILY_UNIX
CRJStringByte CRJApplication::s_utf8("UTF8");
CRJStringByte CRJApplication::s_appStarted("Started at");
CRJStringByte CRJApplication::s_appDone("Done at");
CRJStringByte CRJApplication::s_appDuration("(Duration ");
CRJStringByte CRJApplication::s_appLogError("Thread's logging is unavailable");
CRJStringByte CRJApplication::s_appConfigExt(".xml", 4);
CRJStringByte CRJApplication::s_appOptionConfig("config");
CRJStringByte CRJApplication::s_appOptionLog("log");
CRJStringByte CRJApplication::s_appOptionDebug("debug");
CRJStringByte CRJApplication::s_appOptionInit("init");
CRJStringByte CRJApplication::s_appOptionDebug("work");
#endif 
#ifdef _rjOS_WIN
CRJStringUnicode CRJApplication::s_utf8(L"UTF8");
CRJStringUnicode CRJApplication::s_appStarted(L"Started at");
CRJStringUnicode CRJApplication::s_appDone(L"Done at");
CRJStringUnicode CRJApplication::s_appDuration(L"(Duration ");
CRJStringUnicode CRJApplication::s_appLogError(L"Thread's logging is unavailable");
CRJStringUnicode CRJApplication::s_appConfigExt(L".xml", 4);
CRJStringUnicode CRJApplication::s_appOptionConfig(L"config");
CRJStringUnicode CRJApplication::s_appOptionLog(L"log");
CRJStringUnicode CRJApplication::s_appOptionDebug(L"debug");
CRJStringUnicode CRJApplication::s_appOptionInit(L"init");
CRJStringUnicode CRJApplication::s_appOptionWork(L"work");
#endif 
std::shared_mutex CRJApplication::s_appDebugMutex;
RJBASE_API void InitTemplates() {
	CRJCompareSize s(0, 0);
	CRJCompareInt64 v(0, 0);
	v.Compare(0);
}

