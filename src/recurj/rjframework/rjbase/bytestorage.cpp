#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/bytestorage.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
using namespace rjBase;
CRJByteStorage::CRJByteStorage(uint8_t* p, size_t s):
	m_clStorage(p, s),
m_sProcessed(0) {}
CRJByteStorage::CRJByteStorage(CRJStringByteFlex& v):
	m_clStorage(v),
m_sProcessed(0) {}
CRJByteStorage::CRJByteStorage():
	m_sProcessed(0) {}
CRJByteStorage::~CRJByteStorage() {}
void CRJByteStorage::SetBuffer(uint8_t* p, size_t s) {
	m_clStorage.SetStorage(p, s);
	m_sProcessed=0;
}
CRJByteStorage& CRJByteStorage::ResetCollector() {
	m_clStorage.SetEmpty();
	m_sProcessed=0;
	return *this;
}
CRJByteStorage& CRJByteStorage::Normalize() {
	if (m_sProcessed>0) {
		size_t s=m_clStorage.GetLength();
		if (m_sProcessed>=s) ResetCollector();
		else {
			uint8_t* h=m_clStorage.GetHeadForUpdate();
			s-=m_sProcessed;
			m_clStorage.SetLengthSafed(s);
			_MEMMOVE_BYTE_POINTER(h,& h[m_sProcessed], s);
		}
		m_sProcessed=0;
	}
	return *this;
}
CRJByteStorage& CRJByteStorage::Processed(size_t lCount) {
#ifdef _DEBUG
	if (m_clStorage.GetLength()<(m_sProcessed+lCount)) CRJDataTools::DebugBreak();
#endif 
	m_sProcessed+=lCount;
	return *this;
}

