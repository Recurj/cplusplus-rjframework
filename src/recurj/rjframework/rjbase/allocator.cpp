#include "stdafx.h"
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/memory/allocatorpolicy.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/application/application.h>
#include <vector>
using namespace rjBase;
bool CRJAllocatorDefault::Release() {
	return true;
}
uint8_t* CRJAllocatorDefault::Alloc(size_t s) {
	return (uint8_t *)::malloc(s);
}
size_t CRJAllocatorDefault::Free(uint8_t* p) {
	try {
		if (p) {
			size_t s=GetSize(p);
			::free(p);
			return s;
		}
	}
	catch(...) {}
	return 0;
}
size_t CRJAllocatorDefault::GetSize(uint8_t* p) {
	try {
		if (p) return SIZE__MEM_ALLOCED(p);
	}
	catch(...) {}
	return 0;
}
bool CRJAllocatorDefault::Validate(uint8_t*) {
	return true;
}
CRJAllocatorRuntime::CRJAllocatorRuntime(size_t r, size_t t, size_t s):
	m_stInfo(r, t, s),
	m_pAllocator(&CRJApplicationMemory::GetAllocatorDefault()) {
	m_clMode=& CRJAllocatorRuntime::ModeStartup;
}
CRJAllocatorRuntime::~CRJAllocatorRuntime() {}
IRJAllocator& CRJAllocatorRuntime::GetAllocator() {
	return (this->* m_clMode)();
}
bool CRJAllocatorRuntime::Release() {
	return GetAllocator().Release();
}
uint8_t* CRJAllocatorRuntime::Alloc(size_t s) {
	return GetAllocator().Alloc(s);
}
size_t CRJAllocatorRuntime::Free(uint8_t* p) {
	return (p!=nullptr)?
		GetAllocator().Free(p):
		0;
}
size_t CRJAllocatorRuntime::GetSize(uint8_t* p) {
	return GetAllocator().GetSize(p);
}
bool CRJAllocatorRuntime::Validate(uint8_t* p) {
	return GetAllocator().Validate(p);
}
IRJAllocator& CRJAllocatorRuntime::ModeStartup() {
	if (CRJApplication::HasApplication()) {
		IRJAllocator* p=CRJApplication::Current().AppAllocatorRuntime(m_stInfo);
		if (p!=nullptr) m_pAllocator=p;
	}
	m_clMode=& CRJAllocatorRuntime::ModeWork;
	return * m_pAllocator;
}
IRJAllocator& CRJAllocatorRuntime::ModeWork() {
	return * m_pAllocator;
}
size_t CRJAllocatorRuntime::PreAllocate(size_t s, size_t elem) {
	void* p;
	std::vector<Something>arr;
	arr.reserve(s);
	for (size_t i=0; i<s; i++) {
		p=Alloc(elem);
		if (p) arr.push_back(p);
	}
	for (auto pElem:arr) Free((uint8_t*)pElem);
	return arr.size();
}

size_t CRJAllocatorRuntime::GetSizeAligned(size_t p, size_t n) {
	size_t v;
	if (p == 0) p = 1024;
	v = n / p;
	if ((n % p) > 0) v++;
	return v * p;
}
bool CRJAllocatorRuntime::Realloc(size_t s, uint8_t** pq, const uint8_t* pData, size_t l) {
	uint8_t* p = Alloc(s);
	if (p) {
		uint8_t* q = *pq;
		if (pData != nullptr &&
			l > 0) _MEMMOVE_BYTE_POINTER(p, pData, l);
		if (q != nullptr) Free(q);
		*pq = p;
		return true;
	}
	return false;
}
CRJAllocatorPolicy::CRJAllocatorPolicy(size_t reg, size_t ind) :
	CRJAllocator(reg, ind),
	m_pAllocator(&CRJApplicationMemory::GetAllocatorDefault()) {}
CRJAllocatorPolicy::~CRJAllocatorPolicy() {}
void CRJAllocatorPolicy::Detach() {
	m_pAllocator = &CRJApplicationMemory::GetAllocatorDefault();
}
bool CRJAllocatorPolicy::Release() {
	std::lock_guard<std::mutex>lg(m_clMutex);
	preRelease();
	m_pAllocator->Release();
	Reset();
	return true;
}
