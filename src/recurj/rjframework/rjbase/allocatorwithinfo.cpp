#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/application/application.h>
using namespace rjBase;
CRJAllocator::CRJAllocator(size_t sRegion, size_t sIndex):
m_sRegion(sRegion),
m_sIndex(sIndex)
{
	_MEMCLR_BYTE(m_stInfo);
}
CRJAllocator::~CRJAllocator() {
#ifdef _DEBUG
	if (m_stInfo.lUseCount>0) {
		CRJDataTools::DebugBreak();
	}
#endif 
//	UnRegister();
}
void CRJAllocator::Reset() {
	_MEMCLR_BYTE(m_stInfo);
}
void CRJAllocator::tsInUse(size_t s) {
	m_stInfo.lUseCount++;
	m_stInfo.lUseSize+=s;
	if (m_stInfo.lMaxCount < m_stInfo.lUseCount) m_stInfo.lMaxCount = m_stInfo.lUseCount;
	if (m_stInfo.lMaxSize < m_stInfo.lUseSize) m_stInfo.lMaxSize = m_stInfo.lUseSize;

}
void CRJAllocator::tsUse2Free(size_t s) {
	tsNotUsed(s);
	m_stInfo.lFreeCount++;
	m_stInfo.lFreeSize += s;
}
void CRJAllocator::tsNotUsed(size_t s) {
#ifdef _DEBUG
	if (m_stInfo.lUseCount == 0 || m_stInfo.lUseSize < (int64_t) s) CRJDataTools::DebugBreak();
#endif
	m_stInfo.lUseCount--;
	m_stInfo.lUseSize-=s;
}
void CRJAllocator::tsFree2Use(size_t s) {
#ifdef _DEBUG
	if (m_stInfo.lFreeCount == 0 || m_stInfo.lFreeSize < (int64_t)s) CRJDataTools::DebugBreak();
#endif
	m_stInfo.lFreeCount--;
	m_stInfo.lFreeSize-=s;
	tsInUse(s);
}

uint8_t* CRJAllocator::tsNotAllocated() {
	m_stInfo.lErrorCount++;
	return nullptr;
}
