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
 const size_t SRJDateTime::s_sDays[16]= {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 30, 30, 30, 30
};
const size_t SRJDateTime::s_sQuarters[16]= {
	1, 1, 1, 4, 4, 4, 7, 7, 7, 10, 10, 10, 1, 1, 1, 1
};
SRJDateTime::SRJDateTime():
	m_lYear(0),
m_sMonth(0),
m_sDay(0),
m_sHour(0),
m_sMinute(0),
m_sSecond(0),
m_sYearDay(0),
m_sDayOfWeek(0) {}
SRJDateTime::SRJDateTime(time_t t) {
	Reset();
	Parse(t);
}
SRJDateTime::SRJDateTime(int64_t y, size_t m):
	m_lYear(y),
m_sMonth(m),
m_sDay(1),
m_sHour(0),
m_sMinute(0),
m_sSecond(0),
m_sYearDay(0),
m_sDayOfWeek(0) {}
SRJDateTime::SRJDateTime(int64_t y, size_t m, size_t d):
	m_lYear(y),
m_sMonth(m),
m_sDay(d),
m_sHour(0),
m_sMinute(0),
m_sSecond(0),
m_sYearDay(0),
m_sDayOfWeek(0) {}
SRJDateTime::SRJDateTime(int64_t y, size_t m, size_t d, size_t h, size_t min, size_t sec):
	m_lYear(y),
m_sMonth(m),
m_sDay(d),
m_sHour(h),
m_sMinute(min),
m_sSecond(sec),
m_sYearDay(0),
m_sDayOfWeek(0) {}
void SRJDateTime::Reset() {
	m_lYear=0;
	m_sMonth=1;
	m_sDay=1;
	m_sHour=0;
	m_sMinute=0;
	m_sSecond=0;
	m_sYearDay=0;
	m_sDayOfWeek=0;
}
time_t SRJDateTime::MakeTime() {
	time_t rc=0;
	try {
		struct tm tm;
		tm.tm_isdst=-1;
		tm.tm_year=(int) m_lYear;
		tm.tm_mon=(int) m_sMonth;
		tm.tm_mday=(int) m_sDay;
		tm.tm_hour=(int) m_sHour;
		tm.tm_min=(int) m_sMinute;
		tm.tm_sec=(int) m_sSecond;
		tm.tm_mon--;
		tm.tm_year-=1900;
		rc=::mktime(& tm);
	}
	catch(...) {}
	return rc;
}
SRJDateTime& SRJDateTime::Parse(time_t dt) {
	try {
		struct tm tm;
		if (RJDTIME_LOCALTIME(& tm,& dt)) {
			m_lYear=tm.tm_year;
			m_lYear+=1900;
			m_sMonth=tm.tm_mon;
			m_sMonth++;
			m_sDay=tm.tm_mday;
			m_sHour=tm.tm_hour;
			m_sMinute=tm.tm_min;
			m_sSecond=tm.tm_sec;
			m_sYearDay=tm.tm_yday;
			m_sDayOfWeek=tm.tm_wday;
		}
	}
	catch(...) {}
	return *this;
}
SRJDateTime& SRJDateTime::Parse(const uint8_t* p) {
	try {
		if ((p)&&
			(p[0])&&
			(p[4]=='-')&&
			(p[7]=='-')&&
			(p[10]==' ')&&
			(p[13]==':')&&
			(p[16]==':')) {
			m_lYear=CRJByteConvertor::DecStr2IntSafed(p, 4);
			m_sMonth=CRJByteConvertor::DecPair2IntSafedNoCheck(& p[5]);
			m_sDay=CRJByteConvertor::DecPair2IntSafedNoCheck(& p[8]);
			m_sHour=CRJByteConvertor::DecPair2IntSafedNoCheck(& p[11]);
			m_sMinute=CRJByteConvertor::DecPair2IntSafedNoCheck(& p[14]);
			m_sSecond=CRJByteConvertor::DecPair2IntSafedNoCheck(& p[17]);
		}
	}
	catch(...) {}
	return *this;
}
size_t SRJDateTime::SetDTAppl(uint8_t* pBuf, size_t sSize) const {
	if (sSize>=(cSizeDTime-1)) {
		size_t lOff=CRJByteConvertor::Int64ToBytes(m_lYear, pBuf, sSize-1);
		size_t len=lOff+15;
		if (len<=sSize) {
			const char** q=CRJByteConvertor::s_uPairDigitsToString;
			pBuf+=lOff;
			DTIME_SET(pBuf,'-');
			lOff=(size_t)(m_sMonth& 0x0F);
			DTIME_COPY(pBuf);
			DTIME_SET(pBuf,'-');
			lOff=(size_t)(m_sDay& 0x1F);
			DTIME_COPY(pBuf);
			DTIME_SET(pBuf,' ');
			lOff=(size_t)(m_sHour& 0x3F);
			DTIME_COPY(pBuf);
			DTIME_SET(pBuf,':');
			lOff=(size_t)(m_sMinute& 0x3F);
			DTIME_COPY(pBuf);
			DTIME_SET(pBuf,':');
			lOff=(size_t)(m_sSecond& 0x3F);
			DTIME_COPY(pBuf);
			return len;
		}
	}
	return 0;
}
size_t SRJDateTime::SetDTDate(uint8_t* pBuf, size_t sSize) const {
	if (sSize>=10) {
		size_t lOff=CRJByteConvertor::Int64ToBytes(m_lYear, pBuf, sSize-1);
		size_t len=lOff+6;
		if (len<=sSize) {
			const char** q=CRJByteConvertor::s_uPairDigitsToString;
			pBuf+=lOff;
			DTIME_SET(pBuf,'-');
			lOff=(size_t)(m_sMonth& 0x0F);
			DTIME_COPY(pBuf);
			DTIME_SET(pBuf,'-');
			lOff=(size_t)(m_sDay& 0x1F);
			DTIME_COPY(pBuf);
			DTIME_SET(pBuf,' ');
			return len;
		}
	}
	return 0;
}
size_t SRJDateTime::SetDTTime(uint8_t* pBuf, size_t sSize) const {
	if (sSize>=8) {
		const char** q=CRJByteConvertor::s_uPairDigitsToString;
		size_t lOff=(size_t)(m_sHour& 0x3F);
		DTIME_COPY(pBuf);
		DTIME_SET(pBuf,':');
		lOff=(size_t)(m_sMinute& 0x3F);
		DTIME_COPY(pBuf);
		DTIME_SET(pBuf,':');
		lOff=(size_t)(m_sSecond& 0x3F);
		DTIME_COPY(pBuf);
		return 8;
	}
	return 0;
}
size_t SRJDateTime::SetYYYYMMDDHHMMSS(uint8_t* pBuf, size_t sSize) const {
	if (sSize>=14) {
		size_t lOff=CRJByteConvertor::Int64ToBytes(m_lYear, pBuf, sSize-1);
		size_t len=lOff+10;
		if (len<=sSize) {
			const char** q=CRJByteConvertor::s_uPairDigitsToString;
			pBuf+=lOff;
			lOff=(size_t)(m_sMonth& 0x0F);
			DTIME_COPY(pBuf);
			lOff=(size_t)(m_sDay& 0x1F);
			DTIME_COPY(pBuf);
			lOff=(size_t)(m_sHour& 0x3F);
			DTIME_COPY(pBuf);
			lOff=(size_t)(m_sMinute& 0x3F);
			DTIME_COPY(pBuf);
			lOff=(size_t)(m_sSecond& 0x3F);
			DTIME_COPY(pBuf);
			return len;
		}
	}
	return 0;
}
size_t SRJDateTime::SetYYYYMM(uint8_t* pBuf, size_t sSize) const {
	if (sSize>=6) {
		size_t lOff=CRJByteConvertor::Int64ToBytes(m_lYear, pBuf, sSize-1);
		size_t len=lOff+2;
		if (len<=sSize) {
			const char** q=CRJByteConvertor::s_uPairDigitsToString;
			pBuf+=lOff;
			lOff=(size_t)(m_sMonth& 0x0F);
			DTIME_COPY(pBuf);
			return len;
		}
	}
	return 0;
}
size_t SRJDateTime::SetYYMMDDHHMM(uint8_t* pBuf, size_t sSize) const {
	if (sSize>=10) {
		const char** q=CRJByteConvertor::s_uPairDigitsToString;
		size_t lOff=(size_t)(m_lYear% 100);
		DTIME_COPY(pBuf);
		lOff=(size_t)(m_sMonth& 0x0F);
		DTIME_COPY(pBuf);
		lOff=(size_t)(m_sDay& 0x1F);
		DTIME_COPY(pBuf);
		lOff=(size_t)(m_sHour& 0x3F);
		DTIME_COPY(pBuf);
		lOff=(size_t)(m_sMinute& 0x3F);
		DTIME_COPY(pBuf);
		return 10;
	}
	return 0;
}
size_t SRJDateTime::SetMMDDHHMMSS(uint8_t* pBuf, size_t sSize) const {
	if (sSize>=10) {
		const char** q=CRJByteConvertor::s_uPairDigitsToString;
		size_t lOff=(size_t)(m_sMonth& 0x0F);
		DTIME_COPY(pBuf);
		lOff=(size_t)(m_sDay& 0x1F);
		DTIME_COPY(pBuf);
		lOff=(size_t)(m_sHour& 0x3F);
		DTIME_COPY(pBuf);
		lOff=(size_t)(m_sMinute& 0x3F);
		DTIME_COPY(pBuf);
		lOff=(size_t)(m_sSecond& 0x3F);
		DTIME_COPY(pBuf);
		return 10;
	}
	return 0;
}
bool SRJDateTime::BeforeOrSameMonth(const SRJDateTime& st) const {
	return (m_lYear<st.m_lYear)||
		(st.m_lYear==m_lYear&&
		m_sMonth<=st.m_sMonth);
}
bool SRJDateTime::SameDay(const SRJDateTime& st) const {
	return st.m_lYear==m_lYear&&
		st.m_sMonth==m_sMonth&&
		st.m_sDay==m_sDay;
}
bool SRJDateTime::SameMonth(const SRJDateTime& st) const {
	return st.m_lYear==m_lYear&&
		st.m_sMonth==m_sMonth;
}
void SRJDateTime::operator=(const SRJDateTime& st) {
	m_lYear=st.m_lYear;
	m_sMonth=st.m_sMonth;
	m_sDay=st.m_sDay;
	m_sHour=st.m_sHour;
	m_sMinute=st.m_sMinute;
	m_sSecond=st.m_sSecond;
	m_sYearDay=st.m_sYearDay;
	m_sDayOfWeek=st.m_sDayOfWeek;
}
bool SRJDateTime::operator==(const SRJDateTime& st) const {
	return st.m_lYear==m_lYear&&
		st.m_sMonth==m_sMonth&&
		st.m_sDay==m_sDay&&
		st.m_sHour==m_sHour&&
		st.m_sMinute==m_sMinute&&
		st.m_sSecond==m_sSecond;
}
bool SRJDateTime::operator>(const SRJDateTime& st) const {
	DTIME_MORE(m_lYear);
	DTIME_MORE(m_sMonth);
	DTIME_MORE(m_sDay);
	DTIME_MORE(m_sHour);
	DTIME_MORE(m_sMinute);
	DTIME_MORE(m_sSecond);
	return false;
}
bool SRJDateTime::operator<(const SRJDateTime& st) const {
	DTIME_LESS(m_lYear);
	DTIME_LESS(m_sMonth);
	DTIME_LESS(m_sDay);
	DTIME_LESS(m_sHour);
	DTIME_LESS(m_sMinute);
	DTIME_LESS(m_sSecond);
	return false;
}
SRJDateTime& SRJDateTime::ValidateDay() {
	size_t m=SRJDateTime::GetMonthLastDay(m_lYear, m_sMonth);
	if (m_sDay>m) m_sDay=m;
	return *this;
}
SRJDateTime& SRJDateTime::NextSecond(size_t v) {
	m_sSecond+=v;
	if (m_sSecond>59) {
		if (m_sSecond<120) {
			m_sSecond-=60;
			return NextMinute(1);
		}
		else {
			size_t m=m_sSecond/ 60;
			m_sSecond=m_sSecond% 60;
			return NextMinute(m);
		}
	}
	return *this;
}
SRJDateTime& SRJDateTime::PrevSecond(size_t v) {
	if (m_sSecond>=v) m_sSecond-=v;
	else {
		v-=m_sSecond;
		if (v>59) {
			size_t m=v/ 60;
			size_t r=v% 60;
			m_sSecond=60-r;
			return PrevMinute(m);
		}
		else {
			m_sSecond=60-v;
			return PrevMinute(1);
		}
	}
	return *this;
}
SRJDateTime& SRJDateTime::NextMinute(size_t v) {
	m_sMinute+=v;
	if (m_sMinute>59) {
		if (m_sMinute<120) {
			m_sMinute-=60;
			return NextHour(1);
		}
		else {
			size_t h=m_sMinute/ 60;
			m_sMinute=m_sMinute% 60;
			return NextHour(h);
		}
	}
	return *this;
}
SRJDateTime& SRJDateTime::PrevMinute(size_t v) {
	if (m_sMinute>=v) m_sMinute-=v;
	v-=m_sMinute;
	if (v>59) {
		size_t m=v/ 60;
		size_t r=v% 60;
		m_sMinute=60-r;
		return PrevHour(m);
	}
	m_sMinute=60-v;
	return PrevHour(1);
}
SRJDateTime& SRJDateTime::NextHour(size_t v) {
	m_sHour+=v;
	if (m_sHour>23) {
		if (m_sHour<48) {
			m_sHour-=24;
			return NextDay(1);
		}
		else {
			size_t d=m_sHour/ 24;
			m_sHour=m_sHour% 24;
			return NextDay(d);
		}
	}
	return *this;
}
SRJDateTime& SRJDateTime::PrevHour(size_t v) {
	if (m_sHour>=v) m_sHour-=v;
	v-=m_sHour;
	if (v>23) {
		size_t m=v/ 24;
		size_t r=v% 24;
		m_sHour=24-r;
		return PrevDay(m);
	}
	m_sHour=24-v;
	return PrevDay(1);
}
SRJDateTime& SRJDateTime::NextDay(size_t d) {
	if (d==1) {
		m_sDay++;
		if (m_sDay>SRJDateTime::GetMonthLastDay(m_lYear, m_sMonth)) {
			m_sDay=1;
			return NextMonth();
		}
	}
	else {
		time_t t=MakeTime();
		t+=(CRJDTimeClockAround* d);
		Parse(t);
		while (d>0) {
			d--;
			NextDay(1);
		}
	}
	return *this;
}
SRJDateTime& SRJDateTime::PrevDay(size_t v) {
	while (v>0) {
		if (m_sDay>v) {
			m_sDay-=v;
			break;
		}
		else {
			v-=m_sDay;
			if (m_sMonth>1) m_sMonth--;
			else {
				m_sMonth=12;
				m_lYear--;
			}
			m_sDay=SRJDateTime::GetMonthLastDay(m_lYear, m_sMonth);
		}
	}
	return *this;
}
SRJDateTime& SRJDateTime::NextMonth(size_t v) {
	m_sMonth+=v;
	if (m_sMonth<=12) return ValidateDay();
	int64_t y;
	if (m_sMonth<24) {
		y=1;
		m_sMonth-=12;
	}
	else {
		y=(int64_t) m_sMonth/ 12;
		m_sMonth=m_sMonth% 12;
	}
	return NextYear(y);
}
SRJDateTime& SRJDateTime::PrevMonth(size_t v) {
	while (v>0) {
		if (m_sMonth>v) m_sMonth-=v;
		else {
			v-=m_sMonth;
			m_sMonth=12;
			m_lYear--;
		}
	}
	return *this;
}
SRJDateTime& SRJDateTime::PrevMonthValidate(size_t v) {
	return PrevMonth(v).ValidateDay();
}
SRJDateTime& SRJDateTime::NextYear(int64_t v) {
	m_lYear+=v;
	size_t m=SRJDateTime::GetMonthLastDay(m_lYear, m_sMonth);
	if (m_sDay>m) m_sDay=m;
	return *this;
}
SRJDateTime& SRJDateTime::TruncSeconds() {
	m_sSecond=0;
	return *this;
}
SRJDateTime& SRJDateTime::TruncMinutes() {
	m_sSecond=0;
	m_sMinute=0;
	return *this;
}
SRJDateTime& SRJDateTime::TruncHours() {
	m_sSecond=0;
	m_sMinute=0;
	m_sHour=0;
	return *this;
}
SRJDateTime& SRJDateTime::TruncDays() {
	m_sSecond=0;
	m_sMinute=0;
	m_sHour=0;
	m_sDay=1;
	return *this;
}
SRJDateTime& SRJDateTime::TruncMonths() {
	m_sSecond=0;
	m_sMinute=0;
	m_sHour=0;
	m_sDay=1;
	m_sMonth=1;
	return *this;
}
SRJDateTime& SRJDateTime::AlignMinute(size_t v) {
	if (v>1) m_sMinute=(m_sMinute/ v)* v;
	return TruncSeconds();
}
SRJDateTime& SRJDateTime::AlignHour(size_t v) {
	if (v>1) m_sHour=(m_sHour/ v)* v;
	return TruncMinutes();
}
SRJDateTime& SRJDateTime::AlignDay(size_t v) {
	if (v>1) {
		m_sDay=(m_sDay/ v)* v;
		if (m_sDay<1) m_sDay=1;
	}
	return TruncHours();
}
SRJDateTime& SRJDateTime::AlignMonth(size_t v) {
	if (v>1) {
		m_sMonth=(m_sMonth/ v)* v;
		if (m_sMonth<1) m_sMonth=1;
	}
	return TruncDays();
}
SRJDateTime& SRJDateTime::CurrentWeek() {
	if (!m_sDayOfWeek) m_sDayOfWeek=6;
	else m_sDayOfWeek--;
	return PrevDay(m_sDayOfWeek);
}
SRJDateTime& SRJDateTime::CurrentQuarter() {
	m_sMonth=SRJDateTime::s_sQuarters[m_sMonth-1];
	return *this;
}
SRJDateTime& SRJDateTime::Range(SRJDateTime& stRight, size_t id) {
	switch (id) {
	case CRJDTimeRangeLastYear:(*this)=stRight;
		m_lYear--;
		break;
	case CRJDTimeRangeLastMinute:(*this)=stRight;
		return PrevMinute(1);
	case CRJDTimeRangeLastMinute5:(*this)=stRight;
		return PrevMinute(5);
	case CRJDTimeRangeLastMinute10:(*this)=stRight;
		return PrevMinute(10);
	case CRJDTimeRangeLastMinute15:(*this)=stRight;
		return PrevMinute(15);
	case CRJDTimeRangeLastMinute30:(*this)=stRight;
		return PrevMinute(30);
	case CRJDTimeRangeLastHour:(*this)=stRight;
		return PrevHour(1);
	case CRJDTimeRangeLastHour2:(*this)=stRight;
		return PrevHour(2);
	case CRJDTimeRangeLastHour8:(*this)=stRight;
		return PrevHour(8);
	case CRJDTimeRangeLastHour12:(*this)=stRight;
		return PrevHour(12);
	case CRJDTimeRangeLastSutki:(*this)=stRight;
		return PrevDay(1);
	case CRJDTimeRangeLastSutki3:(*this)=stRight;
		return PrevDay(3);
	case CRJDTimeRangeLastWeek:(*this)=stRight;
		return PrevDay(7);
	case CRJDTimeRangeLastDecade:(*this)=stRight;
		return PrevDay(10);
	case CRJDTimeRangeLastWeek2:(*this)=stRight;
		return PrevDay(14);
	case CRJDTimeRangeCurrentDay:(*this)=stRight;
		return TruncHours();
	case CRJDTimeRangeCurrentWeek:(*this)=stRight;
		return CurrentWeek();
	case CRJDTimeRangeCurrentMounth:(*this)=stRight;
		return TruncDays();
	case CRJDTimeRangeCurrentQuarter:(*this)=stRight;
		return CurrentQuarter();
	case CRJDTimeRangePrevDay:(*this)=stRight.TruncHours();
		stRight.PrevSecond();
		return PrevDay(1);
	case CRJDTimeRangePrevWeek:(*this)=stRight.TruncHours().CurrentWeek();
		stRight.PrevSecond();
		return PrevDay(7);
	case CRJDTimeRangePrevMonth:(*this)=stRight.TruncDays();
		stRight.PrevSecond();
		return PrevMonth(1);
	case CRJDTimeRangePrevQuarter:(*this)=stRight.TruncDays().CurrentQuarter();
		stRight.PrevSecond();
		return PrevMonth(3);
	default:
		(*this)=stRight;
		return PrevMinute(10);
	}
	return *this;
}
size_t SRJDateTime::GetMonthLastDay(int64_t y, size_t m) {
	size_t rc=SRJDateTime::s_sDays[m& 0xF];
	if (m==1) {
		if (y& 3);
		else if (y% 100) rc++;
		else if (y% 400);
		else rc++;
	}
	return rc;
}
void SRJDateTime::NextMonth(int64_t& y, size_t& m) {
	m++;
	if (m>12) {
		m=1;
		y++;
	}
}
void SRJDateTime::PrevMonth(int64_t& y, size_t& m) {
	m--;
	if (m<=0) {
		m=12;
		y--;
	}
}

