#pragma once
#include <recurj/rjframework/datalength.h>
#include <recurj/rjframework/libs/base/datetime/dtimebinary.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
namespace rjBase {
	class RJBASE_API CRJStringDTime:public CRJStringByteVarBuffer<cSizeDTime> {
	public:
		CRJStringDTime();
		CRJStringDTime(time_t);
		CRJStringDTime(const CRJStringDTime&);
		void operator=(const CRJStringDTime&);
		void SetUnixEpoch();
		bool IsValid(size_t&);
		bool ParseDateTime(SRJDateTime&);
		CRJStringDTime& SetDTApplCurrent();
		CRJStringDTime& SetDTAppl(time_t);
		CRJStringDTime& SetDTAppl(const SRJDateTime&);
		uint8_t* CopyToBytes(uint8_t*) const;
		void CopyFromBytes(const uint8_t*);
		CRJStringDTime(CRJStringDTime&&)=delete;
		CRJStringDTime&operator=(CRJStringDTime&&)=delete;
		static void Move(uint8_t*,const uint8_t*);
		static bool IsSame(const uint8_t*,const uint8_t*);
	};
	class RJBASE_API CRJStringDTimeCurrent:public CRJStringDTime {
	public:
		CRJStringDTimeCurrent();
		inline time_t GetDTime() const {
			return m_clDT.m_tDTime;
		}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringDTimeCurrent)
	protected:
		CRJDTimeCurrent m_clDT;
	};
	class RJBASE_API CRJStringDTimeSequence:public CRJStringDTime {
	public:
		CRJStringDTimeSequence();
		virtual~CRJStringDTimeSequence();
		size_t Tick();
		size_t Tick(time_t);
		size_t GetSequenceAndNext();
		inline time_t GetDTime() const {
			return m_clDT.GetDTime();
		}
		inline size_t GetSequence() const {
			return m_clDT.GetSequence();
		}
		inline const SRJDTimeSequence& GetDTSequence() const {
			return m_clDT.m_stSequence;
		}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringDTimeSequence)
	protected:
		CRJDTimeSequence m_clDT;
	};
}

