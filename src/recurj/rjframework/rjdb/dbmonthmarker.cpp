#include "stdafx.h"
#include <recurj/rjframework/libs/db/odbc/dbmonthmarker.h>
using namespace rjDb;
CRJDBMonthMarker::CRJDBMonthMarker() {
	m_sLength=6;
	Reset();
}
void CRJDBMonthMarker::Reset() {
	*((int64_t*) m_szBuffer)=0x2020202020202020;
	m_szBuffer[6]=0x00;
}
void CRJDBMonthMarker::Set(int64_t lYear, int64_t lMonth) {
	CRJStringByteDefault s;
	const uint8_t* p=s.GetHeadForRead();
	s.SetInt64PaddingLeft(lYear, 4).CatInt64Fmt(lMonth, 2);
	*((uint32_t*) m_szBuffer)=*((uint32_t*) p);
	*((uint16_t*)& m_szBuffer[4])=*((uint16_t*)(p+4));
}
bool CRJDBMonthMarker::CheckDTime(const uint8_t* pDTime) {
	if ((((uint16_t*)& m_szBuffer[4])[0]!=((uint16_t*)& pDTime[5])[0])||
		(((uint32_t*) m_szBuffer)[0]!=((uint32_t*) pDTime)[0])) {
		((uint32_t*) m_szBuffer)[0]=((uint32_t*) pDTime)[0];
		((uint16_t*)& m_szBuffer[4])[0]=((uint16_t*)& pDTime[5])[0];
		return true;
	}
	return false;
}
bool CRJDBMonthMarker::CheckLabel(const uint8_t* pLabel) {
	if ((((uint16_t*)& m_szBuffer[4])[0]!=((uint16_t*)& pLabel[4])[0])||
		(((uint32_t*) m_szBuffer)[0]!=((uint32_t*) pLabel)[0])) {
		((uint32_t*) m_szBuffer)[0]=((uint32_t*) pLabel)[0];
		((uint16_t*)& m_szBuffer[4])[0]=((uint16_t*)& pLabel[4])[0];
		return true;
	}
	return false;
}

