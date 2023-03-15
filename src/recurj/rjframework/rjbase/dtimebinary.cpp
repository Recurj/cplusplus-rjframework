#include "stdafx.h"
#include <recurj/rjframework/libs/base/datetime/dtimebinary.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
using namespace rjBase;
#define DTIME_SET(_pDst,_ch) {*_pDst=_ch;_pDst++;}
#define DTIME_COPY(_pDst) {(*((uint16_t *)_pDst))=(*((uint16_t *)q[(lOff)]));_pDst+=2;}
#define DTIME_MORE(_a) if (_a>st._a) return true;else if (_a < st._a) return false
#define DTIME_LESS(_a) if (_a<st._a) return true;else if (_a > st._a) return false
 const char* CRJDTimeTools::s_szUTCDay[8]= {
	"Sun","Mon","Tue","Wed","Thu","Fri","Sat","Sun"
};
const char* CRJDTimeTools::s_szUTCMonth[16]= {
	"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec","---","---",
	"---","---",
};
const CRJStringByte CRJDTimeTools::s_szUTCDayLong[8]= {
	"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"
};
const size_t CRJDTimeTools::s_sCalendarMonths[16]= {
	0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5, 0, 0, 0, 0
};
const size_t CRJDTimeTools::s_sCalendarMonthsLeap[16]= {
	6, 2, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5, 0, 0, 0, 0
};
const SRJDTimeCalendarCenturies CRJDTimeTools::s_stCalendarCenturies[4]= {
	{
		2000, 2099, 6
	}
	, {
		2100, 2199, 4
	}
	, {
		2200, 2299, 2
	}
	, {
		2300, 2399, 0
	}
};
#ifdef _rjOS_WIN
time_t CRJDTimeTools::GetDTime() {
	SDTimeWin stDT;
	::GetSystemTimeAsFileTime(&(stDT.ft_struct));
	return (time_t)((time_t)((stDT.ft_scalar-EPOCH_BIAS)/ 10000000LL));
}
#else 
time_t CRJDTimeTools::GetDTime() {
	return time(nullptr);
}
#endif 
size_t CRJDTimeTools::SetDTAppl(time_t dt, uint8_t* pBuf, size_t sSize) {
	SRJDateTime sdt(dt);
	return sdt.SetDTAppl(pBuf, sSize);
}
size_t CRJDTimeTools::SetDTAppl(int64_t y, size_t m, size_t d, size_t h, size_t min, size_t sec, uint8_t* pBuf, size_t sSize) {
	SRJDateTime sdt(y, m, d, h, min, sec);
	return sdt.SetDTAppl(pBuf, sSize);
}
CRJStringByteVar& CRJDTimeTools::SetDTAppl(CRJStringByteVar& s, time_t dt) {
	SRJDateTime sdt(dt);
	return s.SetLength(sdt.SetDTAppl(s.GetHeadForUpdate(), s.GetSize()));
}
CRJStringByteVar& CRJDTimeTools::SetDTAppl(CRJStringByteVar& s, int64_t y, size_t m, size_t d, size_t h, size_t min, size_t sec) {
	SRJDateTime sdt(y, m, d, h, min, sec);
	return s.SetLength(sdt.SetDTAppl(s.GetHeadForUpdate(), s.GetSize()));
}
size_t CRJDTimeTools::SetDTDate(time_t dt, uint8_t* pBuf, size_t sSize) {
	SRJDateTime sdt(dt);
	return sdt.SetDTDate(pBuf, sSize);
}
size_t CRJDTimeTools::SetDTDate(int64_t y, size_t m, size_t d, uint8_t* pBuf, size_t sSize) {
	SRJDateTime sdt(y, m, d);
	return sdt.SetDTDate(pBuf, sSize);
}
CRJStringByteVar& CRJDTimeTools::SetDTDate(CRJStringByteVar& s, int64_t y, size_t m, size_t d) {
	SRJDateTime sdt(y, m, d);
	return s.SetLengthSafed(sdt.SetDTDate(s.GetHeadForUpdate(), s.GetSize()));
}
CRJStringByteVar& CRJDTimeTools::SetDTDate(CRJStringByteVar& s, time_t dt) {
	SRJDateTime sdt(dt);
	return s.SetLengthSafed(sdt.SetDTDate(s.GetHeadForUpdate(), s.GetSize()));
}
size_t CRJDTimeTools::SetDTTime(time_t dt, uint8_t* pBuf, size_t sSize) {
	SRJDateTime sdt(dt);
	return sdt.SetDTTime(pBuf, sSize);
}
CRJStringByteVar& CRJDTimeTools::SetDTTime(CRJStringByteVar& s, time_t dt) {
	SRJDateTime sdt(dt);
	return s.SetLengthSafed(sdt.SetDTTime(s.GetHeadForUpdate(), s.GetSize()));
}
size_t CRJDTimeTools::SetYYYYMMDDHHMMSS(time_t dt, uint8_t* pBuf, size_t sSize) {
	SRJDateTime sdt(dt);
	return sdt.SetYYYYMMDDHHMMSS(pBuf, sSize);
}
CRJStringByteVar& CRJDTimeTools::SetYYYYMMDDHHMMSS(CRJStringByteVar& s, time_t dt) {
	SRJDateTime sdt(dt);
	return s.SetLength(sdt.SetYYYYMMDDHHMMSS(s.GetHeadForUpdate(), s.GetSize()));
}
size_t CRJDTimeTools::SetYYYYMM(time_t dt, uint8_t* pBuf, size_t sSize) {
	SRJDateTime sdt(dt);
	return sdt.SetYYYYMM(pBuf, sSize);
}
size_t CRJDTimeTools::SetYYYYMM(int64_t y, size_t m, uint8_t* pBuf, size_t sSize) {
	SRJDateTime sdt(y, m);
	return sdt.SetYYYYMM(pBuf, sSize);
}
CRJStringByteVar& CRJDTimeTools::SetYYYYMM(CRJStringByteVar& s, time_t dt) {
	SRJDateTime sdt(dt);
	return s.SetLength(sdt.SetYYYYMM(s.GetHeadForUpdate(), s.GetSize()));
}
CRJStringByteVar& CRJDTimeTools::SetYYYYMM(CRJStringByteVar& s, int64_t y, size_t m) {
	SRJDateTime sdt(y, m);
	return s.SetLength(sdt.SetYYYYMM(s.GetHeadForUpdate(), s.GetSize()));
}
size_t CRJDTimeTools::SetYYMMDDHHMM(time_t dt, uint8_t* pBuf, size_t sSize) {
	SRJDateTime sdt(dt);
	return sdt.SetYYMMDDHHMM(pBuf, sSize);
}
CRJStringByteVar& CRJDTimeTools::SetYYMMDDHHMM(CRJStringByteVar& s, time_t dt) {
	SRJDateTime sdt(dt);
	return s.SetLength(sdt.SetYYMMDDHHMM(s.GetHeadForUpdate(), s.GetSize()));
}
size_t CRJDTimeTools::SetMMDDHHMMSS(time_t dt, uint8_t* pBuf, size_t sSize) {
	SRJDateTime sdt(dt);
	return sdt.SetMMDDHHMMSS(pBuf, sSize);
}
CRJStringByteVar& CRJDTimeTools::SetMMDDHHMMSS(CRJStringByteVar& s, time_t t) {
	return s.SetLength(CRJDTimeTools::SetMMDDHHMMSS(t, s.GetHeadForUpdate(), s.GetSize()));
}
CRJStringByteVar& CRJDTimeTools::DT2StrRFC822(CRJStringByteVar& str, time_t dt) {
	try {
		struct tm tm;
		if (RJDTIME_LOCALTIME(& tm,& dt)) {
			return str.SetLabel(CRJDTimeTools::s_szUTCDay[tm.tm_wday& 7], 3).CatSingle(',').CatInt64Fmt(tm.tm_mday, 2).CatSingle(' ').CatLabel(CRJDTimeTools::s_szUTCMonth[tm.tm_mon& 0x0F], 3).CatSingle(' ').CatInt64Fmt((int64_t) tm.tm_year+1900, 4).CatSingle(' ').CatInt64Fmt(tm.tm_hour, 2).CatSingle(' ').CatInt64Fmt(tm.tm_min, 2).CatSingle(' ').CatInt64(tm.tm_sec).CatLabel(" GMT", 4);
		}
	}
	catch(...) {}
	return str.SetEmpty();
}
CRJStringByteVar& CRJDTimeTools::DT2StrCookie(CRJStringByteVar& str, time_t dt) {
	try {
		struct tm tm;
		if (RJDTIME_LOCALTIME(& tm,& dt)) {
			return str.Set(CRJDTimeTools::s_szUTCDayLong[tm.tm_wday& 7]).CatLabel(", ", 2).CatInt64Fmt(tm.tm_mday, 2).CatSingle('-').CatLabel(CRJDTimeTools::s_szUTCMonth[tm.tm_mon& 0x0F], 3).CatSingle('-').CatInt64Fmt(tm.tm_year% 100, 2).CatSingle(' ').CatInt64Fmt(tm.tm_hour, 2).CatSingle(':').CatInt64Fmt(tm.tm_min, 2).CatSingle(':').CatInt64(tm.tm_sec).CatLabel(" GMT", 4);
		}
	}
	catch(...) {}
	return str.SetEmpty();
}
size_t CRJDTimeTools::DayOfWeek(int64_t y, size_t m, size_t d) {
	int64_t a2, a3, a4, a1=CRJDTimeTools::getYearBase(y);
	a2=y% 100;
	a3=(a2>>2)& 0x2F;
	m=(m-1)& 0x0F;
	d&=0x1F;
	if (y& 3) a4=CRJDTimeTools::s_sCalendarMonths[m];
	else if (a2) a4=CRJDTimeTools::s_sCalendarMonthsLeap[m];
	else if (a2% 3) a4=CRJDTimeTools::s_sCalendarMonths[m];
	else a4=CRJDTimeTools::s_sCalendarMonthsLeap[m];
	return (a1+a2+a3+a4+d)% 7;
}
int64_t CRJDTimeTools::getYearBase(int64_t y) {
	int64_t y1=0;
	if (y>CRJDTimeTools::s_stCalendarCenturies[3].lTo) {
		y1=y-CRJDTimeTools::s_stCalendarCenturies[0].lFrom;
		y1/=400;
		y1*=400;
		y=y-y1;
	}
	else if (y<CRJDTimeTools::s_stCalendarCenturies[0].lFrom) {
		y1=CRJDTimeTools::s_stCalendarCenturies[3].lTo-y+1;
		y1/=400;
		y1*=400;
		y=y+y1;
	}
	for (size_t i=0; i<4; i++) {
		if ((CRJDTimeTools::s_stCalendarCenturies[i].lFrom<=y)&&
			(CRJDTimeTools::s_stCalendarCenturies[i].lTo>=y)) return CRJDTimeTools::s_stCalendarCenturies[i].lBase;
	}
	return 0;
}
CRJStringByteVar& CRJDTimeTools::Duration2Str(CRJStringByteVar& str, time_t t) {
	str.SetEmpty();
	try {
		int64_t lMin=0;
		int64_t lHour=0;
		int64_t lSec=t% 60;
		t-=lSec;
		t/=60;
		if (t>0) {
			lMin=(int64_t)(t% 60);
			t-=lMin;
			t/=60;
			if (t>0) {
				lHour=(int64_t)(t% 24);
				t-=lHour;
				t/=24;
			}
		}
		if (t>0) str.SetInt64((int64_t) t);
		str.CatInt64Fmt(lHour, 2).CatInt64Fmt(lMin, 2).CatInt64Fmt(lSec, 2);
	}
	catch(...) {}
	return str;
}
time_t CRJDTimeTools::MakeTime(const uint8_t* p) {
	SRJDateTime st;
	return st.Parse(p).MakeTime();
}
time_t CRJDTimeTools::MakeTime(int64_t y, size_t mo, size_t d, size_t h, size_t m, size_t s) {
	time_t rc=0;
	try {
		struct tm tm;
		tm.tm_isdst=-1;
		tm.tm_year=(int) y;
		tm.tm_mon=(int) mo;
		tm.tm_mday=(int) d;
		tm.tm_hour=(int) h;
		tm.tm_min=(int) m;
		tm.tm_sec=(int) s;
		tm.tm_mon--;
		tm.tm_year-=1900;
		rc=::mktime(& tm);
	}
	catch(...) {}
	return rc;
}
#ifdef _rjOS_FAMILY_UNIX
struct timespec& CRJDTimeTools::BuildTimeOut(
struct timespec& timeout, uint64_t d) {
	timeout.tv_sec=d/ 1000;
	timeout.tv_nsec=(d% 1000);
	if (timeout.tv_nsec>0) timeout.tv_nsec*=1000;
	return timeout;
}
#endif 
CRJDTimeCurrent::CRJDTimeCurrent() {
	m_tDTime=CRJDTimeTools::GetDTime();
}
CRJDTimeCurrent::CRJDTimeCurrent(int64_t lOffset) {
	m_tDTime=CRJDTimeTools::GetDTime()+lOffset;
}
CRJDTimeCurrent::~CRJDTimeCurrent() {}
CRJDTimeSequence::CRJDTimeSequence()noexcept {
	m_stSequence.tTime=CRJDTimeTools::GetDTime();
	m_stSequence.sSeq=1;
}
CRJDTimeSequence::CRJDTimeSequence(time_t t, size_t s) {
	m_stSequence.tTime=t;
	m_stSequence.sSeq=s;
}
bool CRJDTimeSequence::Tick() {
	return Tick(CRJDTimeTools::GetDTime());
}
bool CRJDTimeSequence::Tick(time_t t) {
	if (t>m_stSequence.tTime) {
		m_stSequence.tTime=t;
		m_stSequence.sSeq=1;
		return true;
	}
	else m_stSequence.sSeq++;
	return false;
}
void CRJDTimeSequence::Reset() {
	m_stSequence.tTime=0;
	m_stSequence.sSeq=0;
}
bool CRJDTimeSequence::operator==(const CRJDTimeSequence& st) const {
	return m_stSequence==st.m_stSequence;
}
void CRJDTimeSequence::operator=(const CRJDTimeSequence& st) {
	m_stSequence==st.m_stSequence;
}
bool SRJDTimeSequence::operator==(const SRJDTimeSequence& st) const {
	return st.sSeq==sSeq&&
		st.tTime==tTime;
}
void SRJDTimeSequence::operator=(const SRJDTimeSequence& st) {
	sSeq=st.sSeq;
	tTime=st.tTime;
}

