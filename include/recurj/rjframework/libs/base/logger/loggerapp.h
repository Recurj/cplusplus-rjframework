#pragma once
#include <recurj/rjframework/libs/base/strings/native.h>
#include <recurj/rjframework/libs/base/logger/logger.h>
namespace rjBase {
	class CRJLogger;
	class RJBASE_API CRJLoggerApp {
	public:
		static bool NoMemory(int64_t, size_t, size_t sLevel=cLoggerLevelCritical);
		static bool NoMessageRoute(int64_t, size_t sLevel=cLoggerLevelCritical);
		static void ErrorApp(int64_t, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelCritical);
		static void ErrorApp(int64_t,const CRJStringByte&, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelCritical);
		static void ErrorApp(int64_t,const CRJStringUnicode&, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelCritical);
		static void ErrorApp(const CRJStringByte&, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelCritical);
		static void ErrorApp(const CRJStringUnicode&, size_t sPart=cLoggerPartApplication, size_t sLevel=cLoggerLevelCritical);
		static void ErrorOs(int64_t, int64_t, size_t sLevel=cLoggerLevelCritical);
		static void ErrorOs(int64_t, size_t sLevel=cLoggerLevelCritical);
		static void ErrorOs(int64_t,const CRJStringByte&, size_t sLevel=cLoggerLevelCritical);
		static void ErrorOs(int64_t,const CRJStringUnicode&, size_t sLevel=cLoggerLevelCritical);
		static void ErrorFile(int64_t,const CRJFileName&, size_t sLevel=cLoggerLevelCritical);
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
		static void Message(const CRJStringByte&, size_t sPart = cLoggerPartApplication, size_t sLevel = cLoggerLevelInfo);
		static void Message(const CRJStringUnicode&, size_t sPart = cLoggerPartApplication, size_t sLevel = cLoggerLevelInfo);
		static void Int64(const CRJStringByte&, int64_t, size_t sPart = cLoggerPartApplication, size_t sLevel = cLoggerLevelInfo);
		static void Int64(const CRJStringUnicode&, int64_t, size_t sPart = cLoggerPartApplication, size_t sLevel = cLoggerLevelInfo);
		static void Size(const CRJStringByte&, size_t, size_t sPart = cLoggerPartApplication, size_t sLevel = cLoggerLevelInfo);
		static void Size(const CRJStringUnicode&, size_t, size_t sPart = cLoggerPartApplication, size_t sLevel = cLoggerLevelInfo);
		static void Text(const CRJStringByte&,const CRJStringByte&, size_t sPart = cLoggerPartApplication, size_t sLevel = cLoggerLevelInfo);
		static void Text(const CRJStringByte&,const CRJStringUnicode&, size_t sPart = cLoggerPartApplication, size_t sLevel = cLoggerLevelInfo);
		static void Text(const CRJStringUnicode&,const CRJStringByte&, size_t sPart = cLoggerPartApplication, size_t sLevel = cLoggerLevelInfo);
		static void Text(const CRJStringUnicode&,const CRJStringUnicode&, size_t sPart = cLoggerPartApplication, size_t sLevel = cLoggerLevelInfo);
		static void Dump(const CRJStringByte&,const uint8_t*, size_t, size_t sPart = cLoggerPartApplication, size_t sLevel = cLoggerLevelInfo);
		static void Dump(const CRJStringUnicode&,const uint8_t*, size_t, size_t sPart = cLoggerPartApplication, size_t sLevel = cLoggerLevelInfo);
		static void Check(size_t, size_t, IRJLogger&);
		static bool May(size_t, size_t);
	};
}

