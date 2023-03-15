#pragma once
#include <time.h>
#include <recurj/rjframework/modules/modbase.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
namespace rjBase {
	constexpr time_t CRJDTimeMinute=60;
	constexpr time_t CRJDTimeHour=60* CRJDTimeMinute;
	constexpr time_t CRJDTimeClockAround=24* CRJDTimeHour;
	constexpr time_t CRJDTimeYear=365* CRJDTimeClockAround;
	constexpr size_t CRJDTimeRangeDefault=0;
	constexpr size_t CRJDTimeRangeLastYear=1;
	constexpr size_t CRJDTimeRangeLastMinute=2;
	constexpr size_t CRJDTimeRangeLastMinute5=3;
	constexpr size_t CRJDTimeRangeLastMinute10=4;
	constexpr size_t CRJDTimeRangeLastMinute15=5;
	constexpr size_t CRJDTimeRangeLastMinute30=6;
	constexpr size_t CRJDTimeRangeLastHour=7;
	constexpr size_t CRJDTimeRangeLastHour2=8;
	constexpr size_t CRJDTimeRangeLastHour8=9;
	constexpr size_t CRJDTimeRangeLastHour12=10;
	constexpr size_t CRJDTimeRangeLastSutki=11;
	constexpr size_t CRJDTimeRangeLastSutki3=12;
	constexpr size_t CRJDTimeRangeLastWeek=13;
	constexpr size_t CRJDTimeRangeLastDecade=14;
	constexpr size_t CRJDTimeRangeLastWeek2=15;
	constexpr size_t CRJDTimeRangeCurrentDay=16;
	constexpr size_t CRJDTimeRangeCurrentWeek=17;
	constexpr size_t CRJDTimeRangeCurrentMounth=18;
	constexpr size_t CRJDTimeRangeCurrentQuarter=19;
	constexpr size_t CRJDTimeRangePrevDay=20;
	constexpr size_t CRJDTimeRangePrevWeek=21;
	constexpr size_t CRJDTimeRangePrevMonth=22;
	constexpr size_t CRJDTimeRangePrevQuarter=23;
	class CRJStringByteVar;
	struct RJBASE_API SRJDTimeCalendarCenturies {
		int64_t lFrom, lTo;
		size_t lBase;
	};
	struct RJBASE_API SRJDateTime {
		SRJDateTime();
		SRJDateTime(time_t);
		SRJDateTime(int64_t, size_t);
		SRJDateTime(int64_t, size_t, size_t);
		SRJDateTime(int64_t, size_t, size_t, size_t, size_t, size_t);
		int64_t m_lYear;
		size_t m_sMonth, m_sDay, m_sHour, m_sMinute, m_sSecond, m_sYearDay, m_sDayOfWeek;
		void operator=(const SRJDateTime&);
		bool operator==(const SRJDateTime&) const;
		bool operator>(const SRJDateTime&) const;
		bool operator<(const SRJDateTime&) const;
		bool SameDay(const SRJDateTime&) const;
		bool SameMonth(const SRJDateTime&) const;
		bool BeforeOrSameMonth(const SRJDateTime&) const;
		time_t MakeTime();
		SRJDateTime& Parse(time_t);
		SRJDateTime& Parse(const uint8_t*);
		size_t SetDTAppl(uint8_t*, size_t) const;
		size_t SetDTDate(uint8_t*, size_t) const;
		size_t SetDTTime(uint8_t*, size_t) const;
		size_t SetYYYYMMDDHHMMSS(uint8_t*, size_t) const;
		size_t SetYYYYMM(uint8_t*, size_t) const;
		size_t SetYYMMDDHHMM(uint8_t*, size_t) const;
		size_t SetMMDDHHMMSS(uint8_t*, size_t) const;
		void Reset();
		SRJDateTime& NextSecond(size_t v=1);
		SRJDateTime& PrevSecond(size_t v=1);
		SRJDateTime& NextMinute(size_t v=1);
		SRJDateTime& PrevMinute(size_t v=1);
		SRJDateTime& NextHour(size_t v=1);
		SRJDateTime& PrevHour(size_t v=1);
		SRJDateTime& NextDay(size_t v=1);
		SRJDateTime& PrevDay(size_t v=1);
		SRJDateTime& NextMonth(size_t v=1);
		SRJDateTime& PrevMonth(size_t v=1);
		SRJDateTime& PrevMonthValidate(size_t v=1);
		SRJDateTime& NextYear(int64_t v=1);
		SRJDateTime& AlignMinute(size_t);
		SRJDateTime& AlignHour(size_t);
		SRJDateTime& AlignDay(size_t);
		SRJDateTime& AlignMonth(size_t);
		SRJDateTime& TruncSeconds();
		SRJDateTime& TruncMinutes();
		SRJDateTime& TruncHours();
		SRJDateTime& TruncDays();
		SRJDateTime& TruncMonths();
		SRJDateTime& CurrentWeek();
		SRJDateTime& CurrentQuarter();
		SRJDateTime& ValidateDay();
		SRJDateTime& Range(SRJDateTime&, size_t);
		static size_t GetMonthLastDay(int64_t, size_t);
		static void NextMonth(int64_t&, size_t&);
		static void PrevMonth(int64_t&, size_t&);
		static const size_t s_sDays[16];
		static const size_t s_sQuarters[16];
	};
	class RJBASE_API CRJDTimeTools {
	public:
		static time_t GetDTime();
		static size_t SetDTAppl(time_t, uint8_t*, size_t);
		static size_t SetDTAppl(int64_t, size_t, size_t, size_t, size_t, size_t, uint8_t*, size_t);
		static CRJStringByteVar& SetDTAppl(CRJStringByteVar&, time_t);
		static CRJStringByteVar& SetDTAppl(CRJStringByteVar&, int64_t, size_t, size_t, size_t, size_t, size_t);
		static size_t SetDTDate(time_t, uint8_t*, size_t);
		static size_t SetDTDate(int64_t, size_t, size_t, uint8_t*, size_t);
		static CRJStringByteVar& SetDTDate(CRJStringByteVar&, int64_t, size_t, size_t);
		static CRJStringByteVar& SetDTDate(CRJStringByteVar&, time_t);
		static size_t SetDTTime(time_t, uint8_t*, size_t);
		static CRJStringByteVar& SetDTTime(CRJStringByteVar&, time_t);
		static size_t SetYYYYMMDDHHMMSS(time_t, uint8_t*, size_t);
		static CRJStringByteVar& SetYYYYMMDDHHMMSS(CRJStringByteVar&, time_t);
		static size_t SetYYYYMM(time_t, uint8_t*, size_t);
		static size_t SetYYYYMM(int64_t, size_t, uint8_t*, size_t);
		static CRJStringByteVar& SetYYYYMM(CRJStringByteVar&, time_t);
		static CRJStringByteVar& SetYYYYMM(CRJStringByteVar&, int64_t, size_t);
		static size_t SetYYMMDDHHMM(time_t, uint8_t*, size_t);
		static CRJStringByteVar& SetYYMMDDHHMM(CRJStringByteVar&, time_t);
		static size_t SetMMDDHHMMSS(time_t, uint8_t*, size_t);
		static CRJStringByteVar& SetMMDDHHMMSS(CRJStringByteVar&, time_t);
		static CRJStringByteVar& DT2StrRFC822(CRJStringByteVar&, time_t);
		static CRJStringByteVar& DT2StrCookie(CRJStringByteVar&, time_t);
		static size_t DayOfWeek(int64_t, size_t, size_t);
		static CRJStringByteVar& Duration2Str(CRJStringByteVar&, time_t);
		static time_t MakeTime(const uint8_t*);
		static time_t MakeTime(int64_t, size_t, size_t, size_t, size_t, size_t);
		static const char* s_szUTCDay[8];
		static const char* s_szUTCMonth[16];
		static const CRJStringByte s_szUTCDayLong[8];
		static const size_t s_sCalendarMonths[16];
		static const size_t s_sCalendarMonthsLeap[16];
		static const SRJDTimeCalendarCenturies s_stCalendarCenturies[4];
#ifdef _rjOS_FAMILY_UNIX
		static struct timespec& BuildTimeOut(struct timespec&, uint64_t);
#endif 
	protected:
		static int64_t getYearBase(int64_t);
	};
	class RJBASE_API CRJDTimeCurrent {
	public:
		CRJDTimeCurrent();
		CRJDTimeCurrent(int64_t);
		~CRJDTimeCurrent();
		inline CRJDTimeCurrent& SetDTime(time_t t) {
			m_tDTime=t;
			return *this;
		}
		inline time_t GetDTime() const {
			return m_tDTime;
		}
		time_t m_tDTime;
	};
	struct RJBASE_API SRJDTimeSequence {
		time_t tTime;
		size_t sSeq;
		bool operator==(const SRJDTimeSequence&) const;
		void operator=(const SRJDTimeSequence&);
	};
	class RJBASE_API CRJDTimeSequence {
	public:
		CRJDTimeSequence()noexcept;
		CRJDTimeSequence(time_t, size_t);
		bool operator==(const CRJDTimeSequence&) const;
		void operator=(const CRJDTimeSequence&);
		inline void NextSeq() {
			m_stSequence.sSeq++;
		}
		inline time_t GetDTime() const {
			return m_stSequence.tTime;
		}
		inline size_t GetSequence() const {
			return m_stSequence.sSeq;
		}
		inline const SRJDTimeSequence& GetDTSequence() const {
			return m_stSequence;
		}
		bool Tick();
		bool Tick(time_t);
		void Reset();
		SRJDTimeSequence m_stSequence;
	};
}

