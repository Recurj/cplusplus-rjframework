#include "stdafx.h"
#include <recurj/rjframework/libs/base/storage/leaf.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
using namespace rjBase;
RJ_ALLOCATOR(CRJDataStoreLeaf, Allocator::Region::Storage, AllocatorStorage::Region);
CRJDataStoreLeaf::CRJDataStoreLeaf()
{
	_MEMCLR_BYTE_ARRAY(m_stTable);
}
CRJDataStoreLeaf::~CRJDataStoreLeaf() {}
Something CRJDataStoreLeaf::operator[](size_t sIndex) const {
	return m_stTable[sIndex];
}
bool CRJDataStoreLeaf::Set(size_t sIndex, Something p) {
	m_stTable[sIndex]=p;
	return true;
}
void CRJDataStoreLeaf::ShiftRight(size_t sFirst, size_t sCount, Something pInit) {
#ifdef _DEBUG
	if ((sFirst+sCount)>=CRJSizeDatastoreRegion) CRJDataTools::DebugBreak();
#endif 
	* CRJDataStoreLeaf::ShiftRight(& m_stTable[sFirst+sCount], sCount)=pInit;
}
void CRJDataStoreLeaf::ShiftLeft(size_t sLast, size_t sCount, Something pInit) {
#ifdef _DEBUG
	if (sLast<sCount) CRJDataTools::DebugBreak();
#endif 
	* CRJDataStoreLeaf::ShiftLeft(& m_stTable[sLast-sCount], sCount)=pInit;
}
Something CRJDataStoreLeaf::ShiftRightFromLast(size_t sCount, Something pInit) {
	Something rc=m_stTable[CRJSizeDatastoreRegion-1];
	* CRJDataStoreLeaf::ShiftRight(& m_stTable[CRJSizeDatastoreRegion-1], sCount)=pInit;
	return rc;
}
Something CRJDataStoreLeaf::ShiftLeftFromFirst(size_t sCount, Something pInit) {
	Something rc=m_stTable[0];
	* CRJDataStoreLeaf::ShiftLeft(& m_stTable[0], sCount)=pInit;
	return rc;
}
Something* CRJDataStoreLeaf::ShiftRight(Something* q, size_t sCount) {
	if (sCount>0) {
		size_t s=sCount*sizeof(q);
		Something stTable[CRJSizeDatastoreRegion];
		q-=sCount;
		std::memcpy(stTable, q, s);
		std::memcpy(q+1, stTable, s);
	}
	return q;
}
Something* CRJDataStoreLeaf::ShiftLeft(Something* q, size_t sCount) {
	if (sCount>0) {
		size_t s=sCount*sizeof(q);
		Something stTable[CRJSizeDatastoreRegion];
		std::memcpy(stTable, q+1, s);
		std::memcpy(q, stTable, s);
		q+=sCount;
	}
	return q;
}

