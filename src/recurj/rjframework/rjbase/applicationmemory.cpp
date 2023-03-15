#include "stdafx.h"
#include <recurj/rjframework/libs/base/application/memory.h>
#include <recurj/rjframework/libs/base/memory/allocatorfixed.h>
#include <recurj/rjframework/libs/base/memory/allocatorcatalog.h>
#include <recurj/rjframework/libs/base/memory/allocatorany.h>
using namespace rjBase;
#ifdef _rjOS_WIN
#ifdef _DEBUG
#include <crtdbg.h>
int __cdecl WinCrtAllocHook(int nAllocType, void* pvData, size_t nSize, int nBlockUse, long lRequest, const unsigned char* szFileName, int nLine) {
	if (nBlockUse == _CRT_BLOCK) return true;
	try {
		if (nSize == 2304) {
			//CRJDataTools::DebugBreak();
			int i;
			i = 0;
		}
		if ((nAllocType > 0) &&
			(nAllocType < 4) &&
			(nBlockUse >= 0) &&
			(nBlockUse < 5)) {
			char s[1204];
			sprintf_s(s, sizeof(s), "Memory operation in %s line %d: %d a %d-byte '%d' block (#%ld) Data %p\n", (szFileName) ?
				szFileName : (const uint8_t*)"", nLine, nAllocType, (int32_t)nSize, nBlockUse, lRequest, pvData);
		}
	}
	catch (...) {}
	return true;
}
#endif 
#endif 
IRJAllocator& CRJApplicationMemory::GetAllocatorDefault() {
	return m_clAllocatorDefault;
}
CRJApplicationMemory::CRJApplicationMemory() {
#ifdef _rjOS_WIN
#ifdef _DEBUG
	_CrtSetAllocHook(WinCrtAllocHook);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif 
#endif 
}
CRJApplicationMemory::~CRJApplicationMemory() {
	Release();

}
IRJAllocator* CRJApplicationMemory::AppAllocatorRuntime(const SAllocatorRuntimeInfo& st) {
	CRJAllocator* p = nullptr;
	try {
		if (st.m_sSize == 0) p = AppAllocatorRuntimeAny(st);
		else if (st.m_sSize & AllocatorCatalog) p = AppAllocatorRuntimeCatalog(st);
		else p = AppAllocatorRuntimeFixed(st);
	}
	catch (...) {}
	if (p != nullptr) {
		Register(*p);
		return p;
	}
	return &m_clAllocatorDefault;
}
CRJAllocator* CRJApplicationMemory::AppAllocatorRuntimeAny(const SAllocatorRuntimeInfo& st)
{
	return new CRJAllocatorAny(st.m_sRegion, st.m_sIndex);
}
CRJAllocator* CRJApplicationMemory::AppAllocatorRuntimeFixed(const SAllocatorRuntimeInfo& st)
{
	return new CRJAllocatorFixed(st.m_sRegion, st.m_sIndex, st.m_sSize);
}
CRJAllocator* CRJApplicationMemory::AppAllocatorRuntimeCatalog(const SAllocatorRuntimeInfo& st)
{
	return new CRJAllocatorCatalog(st.m_sRegion, st.m_sIndex);
}
void CRJApplicationMemory::Register(CRJAllocator& cl) {
	std::lock_guard<std::mutex>lg(m_clMutex);
	m_clAllocators.Bot(cl);
}
void CRJApplicationMemory::Release() {
	auto p = m_clAllocators.PopTop();
	while (p != nullptr) {
		p->Release();
		delete p;
		p = m_clAllocators.PopTop();
	}
	m_clAllocators.Reset();
}

