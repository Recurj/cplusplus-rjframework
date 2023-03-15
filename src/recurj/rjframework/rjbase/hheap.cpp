#include "stdafx.h"
#include <recurj/rjframework/libs/base/os/hheap.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/errorcodes.h>
using namespace rjBase;
#ifdef _rjOS_WIN
 bool RJHeap::RJHeapOS::HandleDestroy() {
	try {
		return ::HeapDestroy(m_hHandle)==1;
	}
	catch(...) {}
	return false;
}
bool RJHeap::RJHeapOS::Create(size_t lOptions, size_t lIniSize, size_t lMaxSize) {
	CRJHandleCreator clCreator(*this);
	try {
		clCreator.SetResult(HandleAttach(::HeapCreate((uint32_t) lOptions| HEAP_NO_SERIALIZE,(SIZE_T) lIniSize,(SIZE_T) lMaxSize)));
	}
	catch(...) {}
	return clCreator.GetResult();
	return false;
}
size_t RJHeap::RJHeapOS::Free(uint8_t* p) {
	if (RJHandle::HandleIsValid(m_hHandle)) {
		size_t s=(size_t)::HeapSize(m_hHandle, 0, p);
		if (::HeapFree(m_hHandle, 0L, p)!=0) return s;
	}
	return 0;
}
uint8_t* RJHeap::RJHeapOS::Alloc(size_t s) {
	if (RJHandle::HandleIsValid(m_hHandle)) return (uint8_t*) ::HeapAlloc(m_hHandle, HEAP_ZERO_MEMORY, s);
	return nullptr;
}
size_t RJHeap::RJHeapOS::Size(uint8_t *p) {
	if (RJHandle::HandleIsValid(m_hHandle)) return (size_t)::HeapSize(m_hHandle, 0, p);
	return 0;
}
bool RJHeap::RJHeapOS::Validate(uint8_t* p) {
	if (RJHandle::HandleIsValid(m_hHandle)) return ::HeapValidate(m_hHandle, 0, p)!=0;
	return false;
}
#endif 
RJHeap::RJHeap() {}
bool RJHeap::Create(size_t lOpts, size_t lIniSize, size_t lMaxSize) {
#ifdef _rjOS_WIN
	if (m_clOS.Create(lOpts, lIniSize, lMaxSize)) return true;
#endif 
	CRJLoggerThread::ErrorOs(EErrorOs::AppErrHeapInvalid, cLoggerLevelCritical);
	return false;
}
RJHeap::~RJHeap()
#pragma warning(suppress: 26447)
 {
#ifdef _rjOS_WIN
	m_clOS.HandleRelease();
#endif 
}
bool RJHeap::Release() {
#ifdef _rjOS_WIN
	m_clOS.HandleRelease();
#endif 
	return true;
}
size_t RJHeap::Free(uint8_t* p) {
	try {
#ifdef _rjOS_WIN
		return m_clOS.Free(p);
#endif 
	}
	catch(...) {}
	return 0;
}
uint8_t* RJHeap::Alloc(size_t s) {
	try {
#ifdef _rjOS_WIN
		return m_clOS.Alloc(CRJDataTools::SizeAlignPower(s, 5));
#endif 
	}
	catch(...) {}
	return nullptr;
}
size_t RJHeap::GetSize(uint8_t* p) {
	try {
#ifdef _rjOS_WIN
		return m_clOS.Size(p);
#endif 
	}
	catch(...) {}
	return 0;
}
bool RJHeap::Validate(uint8_t* p) {
	try {
#ifdef _rjOS_WIN
		return m_clOS.Validate(p);
#endif 
	}
	catch(...) {}
	return false;
}

