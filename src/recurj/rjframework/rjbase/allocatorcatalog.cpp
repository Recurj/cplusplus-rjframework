#include "stdafx.h"
#include <recurj/rjframework/libs/base/memory/allocatorcatalog.h>
using namespace rjBase;

CRJAllocatorCatalog::CRJAllocatorCatalog(size_t reg, size_t ind):
	CRJAllocatorPolicy(reg, ind) {}
CRJAllocatorCatalog::~CRJAllocatorCatalog() {
	m_clMap.clear();
}
uint8_t* CRJAllocatorCatalog::Alloc(size_t s) {
	std::lock_guard<std::mutex>lg(m_clMutex);
	auto it = m_clMap.find(s);
	size_t v=s+sizeof(SRJAllocatorList);
	SRJAllocatorList* p;
	if (it != m_clMap.end() && it->second != nullptr) {
		p = it->second;
		it->second = p->pNext;
		tsFree2Use(v);
	}
	else {
		p=(SRJAllocatorList*) m_pAllocator->Alloc(v);
		if (!p) return tsNotAllocated();
	    tsInUse(v);
		p->sSize = s;
		p->pHeap = this;
	}
	p->pNext = nullptr;
	p++;
	_MEMCLR_BYTE_POINTER(p, s);
	return (uint8_t*)p;
}
size_t CRJAllocatorCatalog::Free(uint8_t* q) {
	if (!q) return 0;
	std::lock_guard<std::mutex>lg(m_clMutex);
	SRJAllocatorList* p = (SRJAllocatorList*)q;
	p--;
	if (p->pHeap == this) {
		size_t s = p->sSize;
		auto it = m_clMap.find(s);
		if (it != m_clMap.end()) {
			p->pNext = it->second;
			it->second = p;
			tsUse2Free(s + sizeof(SRJAllocatorList));
		}
		else if (addElem(p, s)) tsUse2Free(s + sizeof(SRJAllocatorList));
		else {
			tsNotUsed(s + sizeof(SRJAllocatorList));
			m_pAllocator->Free((uint8_t*)p);
		}
		return s;
	}
	else {
#ifdef _DEBUG
		CRJDataTools::DebugBreak();
#endif 
	}
	return 0;
}

bool CRJAllocatorCatalog::addElem(SRJAllocatorList*p, size_t s)
{
	try {
		p->pNext = nullptr;
		m_clMap.emplace(s, p);
		return true;
	}
	catch (...) {}
	return false;
}
size_t CRJAllocatorCatalog::GetSize(uint8_t* q) {
	if (q) {
		std::lock_guard<std::mutex>lg(m_clMutex);
		uint8_t* p=(uint8_t*) q;
		p-=sizeof(size_t);
		return *((size_t*) p);
	}
	return 0;
}
bool CRJAllocatorCatalog::Validate(uint8_t*) {
	return true;
}
void CRJAllocatorCatalog::preRelease() {
	m_clMap.clear();
}

CRJAllocatorCatalogNative::CRJAllocatorCatalogNative(size_t reg, size_t ind, size_t lOpts, size_t lIniSize, size_t lMaxSize):
	CRJAllocatorCatalog(reg, ind) {
	if (m_clOSAllocator.Create(lOpts, lIniSize, lMaxSize)) {
		m_pAllocator=& m_clOSAllocator;
	}
}
CRJAllocatorCatalogNative::~CRJAllocatorCatalogNative() {
	Detach();
	m_clOSAllocator.Release();
}

