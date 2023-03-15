#pragma once
#include <recurj/rjframework/libs/base/strings/native.h>
#include <recurj/rjframework/libs/base/logger/logger.h>
namespace rjBase {
	class CRJLogger;
	class RJBASE_API CRJLoggerThread {
	public:
		static bool Prepare();
		static bool IsValid();
		static bool NoMemory(int64_t, size_t, size_t lLevel=cLoggerLevelCritical);
		static bool NoMessageRoute(int64_t, size_t lLevel=cLoggerLevelCritical);
		static bool Set(CRJLogger&);
		static void ErrorApp(int64_t, size_t, size_t lLevel=cLoggerLevelCritical);
		static void ErrorApp(int64_t,const CRJStringByte&, size_t, size_t lLevel=cLoggerLevelCritical);
		static void ErrorApp(int64_t,const CRJStringUnicode&, size_t, size_t lLevel=cLoggerLevelCritical);
		static void ErrorOs(int64_t, size_t lLevel=cLoggerLevelCritical);
		static void ErrorOs(int64_t, int64_t, size_t lLevel=cLoggerLevelCritical);
		static void ErrorOs(int64_t,const CRJStringByte&, size_t lLevel=cLoggerLevelCritical);
		static void ErrorOs(int64_t,const CRJStringUnicode&, size_t lLevel=cLoggerLevelCritical);
		static void ErrorFile(int64_t,const CRJFileName&, size_t lLevel=cLoggerLevelCritical);
		static void Process(const CRJFileName&);
		static void ErrorMessage(const CRJStringByte&, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelCritical);
		static void ErrorInt64(const CRJStringByte&, int64_t, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelCritical);
		static void ErrorSize(const CRJStringByte&, size_t, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelCritical);
		static void ErrorText(const CRJStringByte&,const CRJStringByte&, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelCritical);
		static void ErrorText(const CRJStringByte&,const CRJStringUnicode&, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelCritical);
		static void WarningMessage(const CRJStringByte&, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelWarning);
		static void WarningInt64(const CRJStringByte&, int64_t, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelWarning);
		static void WarningSize(const CRJStringByte&, size_t, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelWarning);
		static void WarningText(const CRJStringByte&,const CRJStringByte&, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelWarning);
		static void WarningText(const CRJStringByte&,const CRJStringUnicode&, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelWarning);
		static void Message(size_t, size_t,const CRJStringByte&);
		static void Message(size_t, size_t,const CRJStringUnicode&);
		static void Int64(size_t, size_t,const CRJStringByte&, int64_t);
		static void Int64(size_t, size_t,const CRJStringUnicode&, int64_t);
		static void Size(size_t, size_t,const CRJStringByte&, size_t);
		static void Size(size_t, size_t,const CRJStringUnicode&, size_t);
		static void Text(size_t, size_t,const CRJStringByte&,const CRJStringByte&);
		static void Text(size_t, size_t,const CRJStringByte&,const CRJStringUnicode&);
		static void Text(size_t, size_t,const CRJStringUnicode&,const CRJStringByte&);
		static void Text(size_t, size_t,const CRJStringUnicode&,const CRJStringUnicode&);
		static void Dump(size_t, size_t,const CRJStringByte&,const uint8_t*, size_t);
		static void Dump(size_t, size_t,const CRJStringUnicode&,const uint8_t*, size_t);
		static void Do(IRJLogger&);
		static void Check(size_t, size_t, IRJLogger&);
		static bool May(size_t, size_t);
		static size_t GetLevel(size_t);
		static CRJLogger* GetLogger();
#ifdef _rjOS_FAMILY_UNIX
		static pthread_key_t tlsIndex;
		static int tlsResult;
#endif 
#ifdef _rjOS_WIN
		static uint32_t tlsIndex;
#endif 
	};
}

