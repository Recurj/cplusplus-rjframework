#pragma once
#include <recurj/rjframework/libs/base/strings/native.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/strings/dtime.h>
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/datatools/sharedsize.h>
#include <recurj/rjframework/functors.h>
#include <recurj/rjframework/iname.h>
#include <shared_mutex>
#define FUNC_FORMAT(__func ) va_list args;\
va_start(args, pFormat);\
rc = __func(pFormat, args);\
va_end(args);
namespace rjBase {
	constexpr size_t cLoggerLevelAbsolute=0;
	constexpr size_t cLoggerLevelCritical=1;
	constexpr size_t cLoggerLevelError=2;
	constexpr size_t cLoggerLevelImportant=8;
	constexpr size_t cLoggerLevelErrorOptional=32;
	constexpr size_t cLoggerLevelWarning=256;
	constexpr size_t cLoggerLevelInfo=1024;
	constexpr size_t cLoggerLevelTrace=8* 1024;
	constexpr size_t cLoggerLevelTraceMiddle=16* 1024;
	constexpr size_t cLoggerLevelTraceHigh=32* 1024;
	constexpr size_t cLoggerLevelTraceHighest=64* 1024;
	constexpr size_t cLoggerLevelTraceAll=999999;
	constexpr size_t cLoggerLevelDBError=cLoggerLevelImportant;
	constexpr size_t cLoggerLevelLuaError=10;
	constexpr size_t cLoggerPartOthers=0;
	constexpr size_t cLoggerPartDB=1;
	constexpr size_t cLoggerPartPlugin=2;
	constexpr size_t cLoggerPartComm=3;
	constexpr size_t cLoggerPartSecurity=4;
	constexpr size_t cLoggerPartMemory=5;
	constexpr size_t cLoggerPartTask=6;
	constexpr size_t cLoggerPartConfig=7;
	constexpr size_t cLoggerPartMessage=8;
	constexpr size_t cLoggerPartLua=9;
	constexpr size_t cLoggerPartInfo=10;
	constexpr size_t cLoggerPartFile=11;
	constexpr size_t cLoggerPartOS=12;
	constexpr size_t cLoggerPartDaemon=13;
	constexpr size_t cLoggerPartApplication=14;
	constexpr size_t cLoggerPartFinance=15;
	constexpr size_t cLoggerPartMax=16;
	constexpr size_t cLoggerPartMask=cLoggerPartMax-1;
	class CRJStringByte;
	struct RJBASE_API SRJLoggerLevels {
		SRJLoggerLevels(size_t d=cLoggerLevelDBError);
		SRJLoggerLevels(const SRJLoggerLevels&);
		size_t m_sLevels[cLoggerPartMax];
		inline bool Check(size_t s, size_t d) const {
			return m_sLevels[s& cLoggerPartMask]>=d;
		}
		inline void SetLevel(size_t s, size_t d) {
			m_sLevels[s& cLoggerPartMask]=d;
		}
		inline size_t GetLevel(size_t s) const {
			return m_sLevels[s& cLoggerPartMask];
		}
		static SRJLoggerLevels& SetDebugValues(SRJLoggerLevels& st, size_t lValue) {
			for (size_t i=0; i<cLoggerPartMax; i++) st.m_sLevels[i]=lValue;
			return st;
		}
		static SRJLoggerLevels& SetDebugValuesDefaults(SRJLoggerLevels& st) {
			SetDebugValues(st, cLoggerLevelCritical);
			st.m_sLevels[cLoggerPartDB]=cLoggerLevelDBError;
			return st;
		}
		static bool IsDebugPartsEquals(const SRJLoggerLevels& st1,const SRJLoggerLevels& st2) {
			for (size_t i=0; i<cLoggerPartMax; i++)
			if (st1.m_sLevels[i]!=st2.m_sLevels[i]) return false;
			return true;
		}
	};
	class IRJLogWriterManager {
	public:
		virtual CRJFileName& LogWriterStorage(CRJFileName&)=0;
	};
	class RJBASE_API IRJLoggerWriter {
	public:
		virtual bool LogPrivate(const CRJStringByte&)=0;
		virtual bool LogPublic(const CRJStringByte&)=0;
		virtual bool Start(IRJLogWriterManager&)=0;
		virtual void Stop()=0;
		virtual bool IsReady()=0;
	};
	class RJBASE_API CRJLoggerWriter:public IRJLoggerWriter {
	public:
		bool Check(size_t d) const {
			return m_sLevel>=d;
		}
		CRJLoggerWriter& SetLogLevel(size_t d) {
			m_sLevel=d;
			return *this;
		}
	protected:
		size_t m_sLevel=cLoggerLevelImportant;
	};
	class RJBASE_API CRJLoggerWriterNull:public CRJLoggerWriter {
	public:
		virtual~CRJLoggerWriterNull() {}
		bool LogPublic(const CRJStringByte&) override {
			return true;
		}
		bool LogPrivate(const CRJStringByte&) override {
			return true;
		}
		bool Start(IRJLogWriterManager&) override {
			return true;
		}
		void Stop() override {}
		bool IsReady() override {
			return true;
		}
	};
	class RJBASE_API CRJNamedEmpty:public IRJNamed {
	public:
		CRJStringByteVar& GetName(CRJStringByteVar& s) override;
	};
	class RJBASE_API CRJLogger:public CRJStringByteExpanded {
	public:
		CRJLogger()noexcept;
		CRJLogger(IRJNamed&);
		virtual~CRJLogger();
		size_t GetLevel(size_t) const;
		bool Check(size_t, size_t) const;
		bool Message(const CRJStringByte&);
		bool Message(const CRJStringUnicode&);
		bool Int64(const CRJStringByte&, int64_t);
		bool Int64(const CRJStringUnicode&, int64_t);
		bool Size(const CRJStringByte&, size_t);
		bool Size(const CRJStringUnicode&, size_t);
		bool Text(const CRJStringByte&,const CRJStringByte&);
		bool Text(const CRJStringByte&,const CRJStringUnicode&);
		bool Text(const CRJStringUnicode&,const CRJStringUnicode&);
		bool Text(const CRJStringUnicode&,const CRJStringByte&);
		bool Label(const CRJStringByte&,const char*);
		bool Dump(const CRJStringByte&,const uint8_t*, size_t);
		bool Dump(const CRJStringUnicode&,const uint8_t*, size_t);
		bool DumpBlock(const uint8_t*, size_t);
		CRJLogger& Header();
		CRJLogger& Header(time_t);
		CRJLogger& FormatOpen(const CRJStringByte&);
		CRJLogger& FormatOpen(const CRJStringUnicode&);
		CRJLogger& FormatOpen(const char*);
		bool FormatClose();
		bool Format(_Printf_format_string_ const char*,...);
		bool FormatArgs(const char*, va_list);
		inline void SetWriterOff() {
			m_pWriter=& CRJLogger::s_WriterDefault;
		}
		inline void SetWriter(CRJLoggerWriter& clWriter) {
			m_pWriter=& clWriter;
		}
		inline CRJLoggerWriter& GetWriter() {
			return * m_pWriter;
		}
		inline void SetLogLevels(const SRJLoggerLevels& d) {
			m_stLevels=d;
		}
		inline const SRJLoggerLevels& GetLogLevels() const {
			return m_stLevels;
		}
		static const CRJStringByte s_Separator1;
		static const CRJStringByte s_Separator2;
		static const CRJStringByte s_NoMemory;
		static const CRJStringByte s_NoMessageRoute;
		static const CRJStringByte s_logger;
		static CRJLoggerWriterNull s_WriterDefault;
		static CRJNamedEmpty s_NamedEmpty;
	protected:
		CRJStringByte& DumpLine(CRJStringByteDefault&,const uint8_t*, size_t, size_t);
	protected:
		IRJNamed& m_clNamed;
		CRJLoggerWriter* m_pWriter;
		CRJDTimeSequence m_clTimeSeq;
		SRJLoggerLevels m_stLevels;
	};
	class RJBASE_API CRJLoggerFormat {
	public:
		CRJLoggerFormat(CRJLogger& clLogger,const CRJStringUnicode& s):
			m_clLogger(clLogger) {
			clLogger.FormatOpen(s);
		}
		CRJLoggerFormat(CRJLogger& clLogger,const CRJStringByte& s):
			m_clLogger(clLogger) {
			clLogger.FormatOpen(s);
		}
		CRJLoggerFormat(CRJLogger& clLogger,const char* s):
			m_clLogger(clLogger) {
			clLogger.FormatOpen(s);
		}
		~CRJLoggerFormat() {
			m_clLogger.FormatClose();
		}
		CRJLogger& m_clLogger;
	};
	using IRJLogger=IRJFunctor<CRJLogger&>;
	template<typename TFunc>
	using CRJLoggerFunctor=CRJFunctor<IRJLogger, TFunc, CRJLogger&>;
}

