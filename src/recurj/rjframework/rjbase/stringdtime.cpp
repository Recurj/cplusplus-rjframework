#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/dtime.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
#include <time.h>
using namespace rjBase;
#define FAST_COPY_DTIMESTR(__pDst,__pSrc) *((int64_t *)__pDst)=*((int64_t *)__pSrc);\
 *((int64_t *)&__pDst[8])=*((int64_t *)&__pSrc[8]);\
 *((int32_t *)&__pDst[16])=*((int32_t *)&__pSrc[16]);
#define FAST_CMP_DTIMESTR(__pDst,__pSrc) (*((int64_t *)__pDst)==*((int64_t *)__pSrc)) &&\
(*((int64_t *)&__pDst[8])==*((int64_t *)&__pSrc[8])) &&\
(*((int32_t *)&__pDst[16])==*((int32_t *)&__pSrc[16]))
#define FAST_CLEAR_DTIMESTR(__pDst) *((int64_t *)__pDst)=0;\
 *((int64_t *)&__pDst[8])=0;\
 *((int32_t *)&__pDst[16])=0;
 void CRJStringDTime::Move(uint8_t* pDst,const uint8_t* pSrc) {
	FAST_COPY_DTIMESTR(pDst, pSrc);
}
bool CRJStringDTime::IsSame(const uint8_t* p1,const uint8_t* p2) {
	return FAST_CMP_DTIMESTR(p1, p2);
}
CRJStringDTime::CRJStringDTime() {
	FAST_CLEAR_DTIMESTR(m_szBuffer);
}
CRJStringDTime::CRJStringDTime(time_t t) {
	SetDTAppl(t);
}
CRJStringDTime::CRJStringDTime(const CRJStringDTime& clSrc) {
	*this=clSrc;
}
void CRJStringDTime::operator=(const CRJStringDTime& clSrc) {
	FAST_COPY_DTIMESTR(m_szBuffer, clSrc.GetHeadForReadC());
	m_szBuffer[cSizeDTime-1]=0x00;
	SetLengthSafed(clSrc.GetLength());
}
bool CRJStringDTime::IsValid(size_t& s) {
	CRJChainFinder st;
	if (FindPos('-', st)) {
		s=st.m_sFound;
		return m_sLength==(s+15)&&
			m_szBuffer[s+3]=='-'&&
			m_szBuffer[s+6]==' '&&
			m_szBuffer[s+9]==':'&&
			m_szBuffer[s+13]==':';
	}
	return false;
}
bool CRJStringDTime::ParseDateTime(SRJDateTime& stDay) {
	size_t s;
	if (IsValid(s)) {
		CRJByteConvertor::Bytes2Int64(m_szBuffer, s, stDay.m_lYear);
		stDay.m_sMonth=CRJByteConvertor::PairChars2NumSafed(& m_szBuffer[s+1]);
		stDay.m_sDay=CRJByteConvertor::PairChars2NumSafed(& m_szBuffer[s+4]);
		stDay.m_sHour=CRJByteConvertor::PairChars2NumSafed(& m_szBuffer[s+7]);
		stDay.m_sMinute=CRJByteConvertor::PairChars2NumSafed(& m_szBuffer[s+10]);
		stDay.m_sSecond=CRJByteConvertor::PairChars2NumSafed(& m_szBuffer[s+13]);
		stDay.m_sDayOfWeek=0;
		stDay.m_sYearDay=0;
		return true;
	}
	return false;
}
CRJStringDTime& CRJStringDTime::SetDTApplCurrent() {
	return SetDTAppl(CRJDTimeTools::GetDTime());
}
CRJStringDTime& CRJStringDTime::SetDTAppl(time_t t) {
	FAST_CLEAR_DTIMESTR(m_szBuffer);
	return static_cast<CRJStringDTime&>(SetLengthSafed(CRJDTimeTools::SetDTAppl(t, m_szBuffer,sizeof(m_szBuffer))));
}
CRJStringDTime& CRJStringDTime::SetDTAppl(const SRJDateTime& stDate) {
	FAST_CLEAR_DTIMESTR(m_szBuffer);
	return static_cast<CRJStringDTime&>(SetLengthSafed(stDate.SetDTAppl(m_szBuffer,sizeof(m_szBuffer))));
}
uint8_t* CRJStringDTime::CopyToBytes(uint8_t* pDst) const {
	FAST_COPY_DTIMESTR(pDst, m_szBuffer);
	pDst[cSizeDTime-1]=0x00;
	return pDst;
}
void CRJStringDTime::CopyFromBytes(const uint8_t* pSrc) {
	FAST_COPY_DTIMESTR(m_szBuffer, pSrc);
	SetLengthSafed(cSizeDTime-1);
}
void CRJStringDTime::SetUnixEpoch() {
	SetLabel("1970-01-01 00:00:00", cSizeDTime-1);
}
CRJStringDTimeCurrent::CRJStringDTimeCurrent() {
	SetDTAppl(m_clDT.m_tDTime);
}
CRJStringDTimeSequence::CRJStringDTimeSequence() {
	SetDTAppl(m_clDT.m_stSequence.tTime);
}
CRJStringDTimeSequence::~CRJStringDTimeSequence() {}
size_t CRJStringDTimeSequence::Tick(time_t t) {
	if (m_clDT.Tick(t)) SetDTAppl(m_clDT.m_stSequence.tTime);
	return m_clDT.m_stSequence.sSeq;
}
size_t CRJStringDTimeSequence::Tick() {
	if (m_clDT.Tick()) SetDTAppl(m_clDT.m_stSequence.tTime);
	return m_clDT.m_stSequence.sSeq;
}
size_t CRJStringDTimeSequence::GetSequenceAndNext() {
	size_t rc=m_clDT.m_stSequence.sSeq;
	m_clDT.m_stSequence.sSeq++;
	return rc;
}

