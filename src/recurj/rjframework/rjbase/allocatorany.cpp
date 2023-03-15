#include "stdafx.h"
#include <recurj/rjframework/libs/base/memory/allocatorany.h>
using namespace rjBase;
CRJAllocatorAny::CRJAllocatorAny(size_t reg, size_t ind):
	CRJAllocatorPolicy(reg, ind) {}
CRJAllocatorAny::~CRJAllocatorAny() {}
uint8_t* CRJAllocatorAny::Alloc(size_t s) {
	std::lock_guard<std::mutex>lg(m_clMutex);
	uint8_t* p=m_pAllocator->Alloc(s);
	if (!p) return tsNotAllocated();
	tsInUse(s);
	return p;
}
size_t CRJAllocatorAny::Free(uint8_t* q) {
	std::lock_guard<std::mutex>lg(m_clMutex);
	size_t s=m_pAllocator->Free(q);
	tsNotUsed(s);
	return s;
}
size_t CRJAllocatorAny::GetSize(uint8_t* q) {
	std::lock_guard<std::mutex>lg(m_clMutex);
	return m_pAllocator->GetSize(q);
}
bool CRJAllocatorAny::Validate(uint8_t* q) {
	std::lock_guard<std::mutex>lg(m_clMutex);
	return m_pAllocator->Validate(q);
}
void CRJAllocatorAny::preRelease() {
}

CRJAllocatorAnyNative::CRJAllocatorAnyNative(size_t reg, size_t ind, size_t lOpts, size_t lIniSize, size_t lMaxSize):
	CRJAllocatorAny(reg, ind) {
	if (m_clOSAllocator.Create(lOpts, lIniSize, lMaxSize)) {
		m_pAllocator=& m_clOSAllocator;
	}
}
CRJAllocatorAnyNative::~CRJAllocatorAnyNative() {
	Detach();
	m_clOSAllocator.Release();
}

