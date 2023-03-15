#include "stdafx.h"
#include <recurj/rjframework/libs/base/memory/allocatorfixed.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
using namespace rjBase;
CRJAllocatorFixed::CRJAllocatorFixed(size_t reg, size_t ind, size_t s) :
	CRJAllocatorPolicy(reg, ind),
	m_pFirstFree(nullptr),
	m_lElemSize(s) {}
CRJAllocatorFixed::~CRJAllocatorFixed() {}
uint8_t* CRJAllocatorFixed::Alloc(size_t s) {
	if (s != m_lElemSize) {
#ifdef _DEBUG
		CRJDataTools::DebugBreak();
#endif 
		return nullptr;
	}
	std::lock_guard<std::mutex>lg(m_clMutex);
	SRJAllocatorList* p;
	s += sizeof(SRJAllocatorList);
	if (m_pFirstFree) {
		p = m_pFirstFree;
		m_pFirstFree = p->pNext;
		tsFree2Use(s);
	}
	else {
		p = (SRJAllocatorList*)m_pAllocator->Alloc(s);
		if (!p) return tsNotAllocated();
		tsInUse(s);
		p->pHeap = this;
	}
	p->pNext = nullptr;
	p++;
	_MEMCLR_BYTE_POINTER(p, m_lElemSize);
	return (uint8_t*)p;
}
size_t CRJAllocatorFixed::Free(uint8_t* q) {
	if (!q) return 0;
	std::lock_guard<std::mutex>lg(m_clMutex);
	SRJAllocatorList* p = (SRJAllocatorList*)q;
	p--;
	if (p->pHeap == this) {
		p->pNext = m_pFirstFree;
		m_pFirstFree = p;
		tsUse2Free(m_lElemSize + sizeof(SRJAllocatorList));
		return m_lElemSize;
	}
	else {
#ifdef _DEBUG
		CRJDataTools::DebugBreak();
#endif 
	}
	return 0;
}
size_t CRJAllocatorFixed::PreAllocate(size_t s) {
	size_t rc = 0;
	SRJAllocatorList* p, * q = nullptr;
	for (size_t i = 0; i < s; i++) {
		p = (SRJAllocatorList*)Alloc(m_lElemSize);
		if (p) {
			p--;
			p->pNext = q;
			q = p;
			rc++;
			tsInUse(m_lElemSize);
		}
	}
	while (q) {
		p = q;
		q = q->pNext;
		Free((uint8_t*)(p + 1));
	}
	return rc;
}
size_t CRJAllocatorFixed::GetSize(uint8_t* q) {
	if (q) {
		std::lock_guard<std::mutex>lg(m_clMutex);
		SRJAllocatorList* p = (SRJAllocatorList*)q;
		p--;
		if (p->pHeap == this) return m_lElemSize;
	}
#ifdef _DEBUG
	CRJDataTools::DebugBreak();
#endif 
	return 0;
}
bool CRJAllocatorFixed::Validate(uint8_t*) {
	return true;
}
void CRJAllocatorFixed::preRelease() {
	m_pFirstFree = nullptr;
}

bool CRJAllocatorFixed::SetElemSize(size_t s) {
	if (!IsInUse()) {
		m_lElemSize = s;
		return true;
	}
	return false;
}
CRJAllocatorFixedNative::CRJAllocatorFixedNative(size_t reg, size_t ind, size_t lElemSize, size_t lOpts, size_t lIniSize, size_t lMaxSize) :
	CRJAllocatorFixed(reg, ind, lElemSize) {
	if (m_clOSAllocator.Create(lOpts, lIniSize, lMaxSize)) {
		m_pAllocator = &m_clOSAllocator;
	}
}
CRJAllocatorFixedNative::~CRJAllocatorFixedNative() {
	Detach();
	m_clOSAllocator.Release();
}

