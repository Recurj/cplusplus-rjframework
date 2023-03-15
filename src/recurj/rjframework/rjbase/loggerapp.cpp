#include "stdafx.h"
#include <recurj/rjframework/libs/base/logger/loggerapp.h>
#include <recurj/rjframework/libs/base/strings/byte/error.h>
#include <recurj/rjframework/libs/base/application/application.h>
using namespace rjBase;
void CRJLoggerApp::Check(size_t lPart, size_t lLevel, IRJLogger& clFunc) {
	if (CRJApplication::Current().AppLogger().Check(lPart, lLevel)) clFunc(CRJApplication::Current().AppLogger());
}
bool CRJLoggerApp::May(size_t lPart, size_t lLevel) {
	return CRJApplication::Current().AppLogger().Check(lPart, lLevel);
}
void CRJLoggerApp::ErrorApp(int64_t lErr, size_t sPart, size_t sLevel) {
	auto call=[lErr](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Message(err.Error(lErr));
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(sPart, sLevel, cb);
}
void CRJLoggerApp::ErrorApp(int64_t lErr,const CRJStringByte& s, size_t sPart, size_t sLevel) {
	auto call=[& s, lErr](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Text(err.Error(lErr), s);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(sPart, sLevel, cb);
}
void CRJLoggerApp::ErrorApp(const CRJStringByte& s, size_t sPart, size_t sLevel) {
	auto call=[& s](CRJLogger& clLogger)->void {
		clLogger.Message(s);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(sPart, sLevel, cb);
}
void CRJLoggerApp::ErrorApp(const CRJStringUnicode& s, size_t sPart, size_t sLevel) {
	auto call=[& s](CRJLogger& clLogger)->void {
		clLogger.Message(s);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(sPart, sLevel, cb);
}
void CRJLoggerApp::ErrorApp(int64_t lErr,const CRJStringUnicode& s, size_t sPart, size_t sLevel) {
	auto call=[& s, lErr](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Text(err.Error(lErr), s);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(sPart, sLevel, cb);
}
bool CRJLoggerApp::NoMemory(int64_t lErr, size_t sPart, size_t sLevel) {
	ErrorApp(lErr, CRJLogger::s_NoMemory, sPart, sLevel);
	return false;
}
bool CRJLoggerApp::NoMessageRoute(int64_t lErr, size_t sLevel) {
	ErrorApp(lErr, CRJLogger::s_NoMessageRoute, cLoggerPartTask, sLevel);
	return false;
}
void CRJLoggerApp::ErrorOs(int64_t lErr, size_t sLevel) {
	ErrorOs(lErr, CRJDataTools::GetLastError(), sLevel);
}
void CRJLoggerApp::ErrorOs(int64_t lErr, int64_t lOsErr, size_t sLevel) {
	auto call=[lErr, lOsErr](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Message(err.OS(lErr, lOsErr));
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(cLoggerPartOS, sLevel, cb);
}
void CRJLoggerApp::ErrorOs(int64_t lErr,const CRJStringByte& clName, size_t sLevel) {
	auto call=[lErr,& clName](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Text(err.OS(lErr, CRJDataTools::GetLastError()), clName);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(cLoggerPartOS, sLevel, cb);
}
void CRJLoggerApp::ErrorOs(int64_t lErr,const CRJStringUnicode& clName, size_t sLevel) {
	auto call=[lErr,& clName](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Text(err.OS(lErr, CRJDataTools::GetLastError()), clName);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(cLoggerPartOS, sLevel, cb);
}
void CRJLoggerApp::ErrorFile(int64_t lErr,const CRJFileName& fn, size_t sLevel) {
	auto call=[& fn, lErr](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Text(err.Error(lErr), fn);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(cLoggerPartFile, sLevel, cb);
}
void CRJLoggerApp::Process(const CRJFileName& fn) {
	auto call=[& fn](CRJLogger& clLogger)->void {
		clLogger.Text("Process", fn);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	cb(CRJApplication::Current().m_clAppLogger);
}
void CRJLoggerApp::Message(const CRJStringByte& s,size_t lPart, size_t lLevel) {
	auto call=[& s](CRJLogger& clLog)->void {
		clLog.Message(s);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(lPart, lLevel, cb);
}
void CRJLoggerApp::Message(const CRJStringUnicode& s, size_t lPart, size_t lLevel) {
	auto call=[& s](CRJLogger& clLog)->void {
		clLog.Message(s);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(lPart, lLevel, cb);
}
void CRJLoggerApp::Int64(const CRJStringByte& clMsg, int64_t v, size_t lPart, size_t lLevel) {
	auto call=[& clMsg, v](CRJLogger& clLog)->void {
		clLog.Int64(clMsg, v);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(lPart, lLevel, cb);
}
void CRJLoggerApp::Int64(const CRJStringUnicode& clMsg, int64_t v, size_t lPart, size_t lLevel) {
	auto call=[& clMsg, v](CRJLogger& clLog)->void {
		clLog.Int64(clMsg, v);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(lPart, lLevel, cb);
}
void CRJLoggerApp::Size(const CRJStringByte& clMsg, size_t v, size_t lPart, size_t lLevel) {
	auto call=[& clMsg, v](CRJLogger& clLog)->void {
		clLog.Size(clMsg, v);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(lPart, lLevel, cb);
}
void CRJLoggerApp::Size(const CRJStringUnicode& clMsg, size_t v, size_t lPart, size_t lLevel) {
	auto call=[& clMsg, v](CRJLogger& clLog)->void {
		clLog.Size(clMsg, v);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(lPart, lLevel, cb);
}
void CRJLoggerApp::Text(const CRJStringByte& clMsg,const CRJStringByte& clText, size_t lPart, size_t lLevel) {
	auto call=[& clMsg,& clText](CRJLogger& clLog)->void {
		clLog.Text(clMsg, clText);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(lPart, lLevel, cb);
}
void CRJLoggerApp::Text(const CRJStringByte& clMsg,const CRJStringUnicode& clText, size_t lPart, size_t lLevel) {
	auto call=[& clMsg,& clText](CRJLogger& clLog)->void {
		clLog.Text(clMsg, clText);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(lPart, lLevel, cb);
}
void CRJLoggerApp::Text(const CRJStringUnicode& clMsg,const CRJStringByte& clText, size_t lPart, size_t lLevel) {
	auto call=[& clMsg,& clText](CRJLogger& clLog)->void {
		clLog.Text(clMsg, clText);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(lPart, lLevel, cb);
}
void CRJLoggerApp::Text(const CRJStringUnicode& clMsg,const CRJStringUnicode& clText, size_t lPart, size_t lLevel) {
	auto call=[& clMsg,& clText](CRJLogger& clLog)->void {
		clLog.Text(clMsg, clText);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(lPart, lLevel, cb);
}
void CRJLoggerApp::Dump(const CRJStringByte& clMsg,const uint8_t* pData, size_t lSize, size_t lPart, size_t lLevel) {
	auto call=[& clMsg, pData, lSize](CRJLogger& clLog)->void {
		clLog.Dump(clMsg, pData, lSize);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(lPart, lLevel, cb);
}
void CRJLoggerApp::Dump(const CRJStringUnicode& clMsg,const uint8_t* pData, size_t lSize, size_t lPart, size_t lLevel) {
	auto call=[& clMsg, pData, lSize](CRJLogger& clLog)->void {
		clLog.Dump(clMsg, pData, lSize);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerApp::Check(lPart, lLevel, cb);
}
void CRJLoggerApp::ErrorMessage(const CRJStringByte& p, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Message(err.Error(p),lPart, lLevel);
}
void CRJLoggerApp::ErrorInt64(const CRJStringByte& p, int64_t v, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Int64(err.Error(p), v, lPart, lLevel);
}
void CRJLoggerApp::ErrorSize(const CRJStringByte& p, size_t v, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Size(err.Error(p), v, lPart, lLevel);
}
void CRJLoggerApp::ErrorText(const CRJStringByte& p,const CRJStringByte& s, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Text(err.Error(p), s, lPart, lLevel);
}
void CRJLoggerApp::ErrorText(const CRJStringByte& p,const CRJStringUnicode& s, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Text( err.Error(p), s, lPart, lLevel);
}
void CRJLoggerApp::WarningMessage(const CRJStringByte& p, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Message(err.Warning(p), lPart, lLevel);
}
void CRJLoggerApp::WarningInt64(const CRJStringByte& p, int64_t v, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Int64(err.Warning(p), v, lPart, lLevel);
}
void CRJLoggerApp::WarningSize(const CRJStringByte& p, size_t v, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Size(err.Warning(p), v, lPart, lLevel);
}
void CRJLoggerApp::WarningText(const CRJStringByte& p,const CRJStringByte& s, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Text(err.Warning(p), s, lPart, lLevel);
}
void CRJLoggerApp::WarningText(const CRJStringByte& p,const CRJStringUnicode& s, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Text(err.Warning(p), s, lPart, lLevel);
}

