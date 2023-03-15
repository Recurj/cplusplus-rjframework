#include "stdafx.h"
#include <recurj/rjframework/libs/base/logger/loggerthread.h>
#include <recurj/rjframework/libs/base/strings/byte/error.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
using namespace rjBase;
#ifdef _rjOS_FAMILY_UNIX
pthread_key_t CRJLoggerThread::tlsIndex;
int CRJLoggerThread::tlsResult=-1;
#endif 
#ifdef _rjOS_WIN
uint32_t CRJLoggerThread::tlsIndex=TLS_OUT_OF_INDEXES;
#endif 
 bool CRJLoggerThread::Prepare() {
#ifdef _rjOS_FAMILY_UNIX
	CRJLoggerThread::tlsResult=pthread_key_create(& CRJLoggerThread::tlsIndex,nullptr);
#endif 
#ifdef _rjOS_WIN
	CRJLoggerThread::tlsIndex=::TlsAlloc();
#endif 
	return CRJLoggerThread::IsValid();
}
bool CRJLoggerThread::IsValid() {
#ifdef _rjOS_FAMILY_UNIX
	return CRJLoggerThread::tlsResult==0;
#endif 
#ifdef _rjOS_WIN
	return CRJLoggerThread::tlsIndex!=TLS_OUT_OF_INDEXES;
#endif 
}
bool CRJLoggerThread::Set(CRJLogger& clLogger) {
#ifdef _rjOS_FAMILY_UNIX
	if (pthread_setspecific(CRJLoggerThread::tlsIndex,& clLogger)) return false;
#endif 
#ifdef _rjOS_WIN
	if (!::TlsSetValue(CRJLoggerThread::tlsIndex,& clLogger)) return false;
#endif 
	return true;
}
CRJLogger* CRJLoggerThread::GetLogger() {
#ifdef _rjOS_FAMILY_UNIX
	return (CRJLogger*) pthread_getspecific(CRJLoggerThread::tlsIndex);
#endif 
#ifdef _rjOS_WIN
	return (CRJLogger*)::TlsGetValue(CRJLoggerThread::tlsIndex);
#endif 
}
void CRJLoggerThread::Do(IRJLogger& clFunc) {
	CRJLogger* pLogger=GetLogger();
	if (pLogger) clFunc(* pLogger);
}
void CRJLoggerThread::Check(size_t lPart, size_t lLevel, IRJLogger& clFunc) {
	CRJLogger* pLogger=GetLogger();
	if (pLogger!=nullptr&&
		pLogger->Check(lPart, lLevel)) clFunc(* pLogger);
}
bool CRJLoggerThread::May(size_t lPart, size_t lLevel) {
	CRJLogger* pLogger=GetLogger();
	return pLogger!=nullptr&&
		pLogger->Check(lPart, lLevel);
}
size_t CRJLoggerThread::GetLevel(size_t lPart) {
	CRJLogger* pLogger=GetLogger();
	return pLogger!=nullptr?
		pLogger->GetLevel(lPart):
		0;
}
void CRJLoggerThread::ErrorApp(int64_t lErr, size_t region, size_t sLevel) {
	auto call=[lErr](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Message(err.Error(lErr));
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(region, sLevel, cb);
}
void CRJLoggerThread::ErrorApp(int64_t lErr,const CRJStringByte& s, size_t region, size_t sLevel) {
	auto call=[& s, lErr](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Text(err.Error(lErr), s);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(region, sLevel, cb);
}
void CRJLoggerThread::ErrorApp(int64_t lErr,const CRJStringUnicode& s, size_t region, size_t sLevel) {
	auto call=[& s, lErr](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Text(err.Error(lErr), s);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(region, sLevel, cb);
}
bool CRJLoggerThread::NoMemory(int64_t lErr, size_t region, size_t sLevel) {
	ErrorApp(lErr, CRJLogger::s_NoMemory, region, sLevel);
	return false;
}
bool CRJLoggerThread::NoMessageRoute(int64_t lErr, size_t sLevel) {
	ErrorApp(lErr, CRJLogger::s_NoMessageRoute, cLoggerPartTask, sLevel);
	return false;
}
void CRJLoggerThread::ErrorOs(int64_t lErr, size_t sLevel) {
	ErrorOs(lErr, CRJDataTools::GetLastError(), sLevel);
}
void CRJLoggerThread::ErrorOs(int64_t lErr, int64_t lOsErr, size_t sLevel) {
	auto call=[lErr, lOsErr](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Message(err.OS(lErr, lOsErr));
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartOS, sLevel, cb);
}
void CRJLoggerThread::ErrorOs(int64_t lErr,const CRJStringByte& clName, size_t sLevel) {
	auto call=[lErr,& clName](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Text(err.OS(lErr, CRJDataTools::GetLastError()), clName);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartOS, sLevel, cb);
}
void CRJLoggerThread::ErrorOs(int64_t lErr,const CRJStringUnicode& clName, size_t sLevel) {
	auto call=[lErr,& clName](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Text(err.OS(lErr, CRJDataTools::GetLastError()), clName);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartOS, sLevel, cb);
}
void CRJLoggerThread::ErrorFile(int64_t lErr,const CRJFileName& fn, size_t sLevel) {
	auto call=[& fn, lErr](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Text(err.Error(lErr), fn);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartFile, sLevel, cb);
}
void CRJLoggerThread::Process(const CRJFileName& fn) {
	auto call=[& fn](CRJLogger& clLogger)->void {
		clLogger.Text("Process", fn);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Do(cb);
}
void CRJLoggerThread::Message(size_t lPart, size_t lLevel,const CRJStringByte& s) {
	auto call=[& s](CRJLogger& clLog)->void {
		clLog.Message(s);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(lPart, lLevel, cb);
}
void CRJLoggerThread::Message(size_t lPart, size_t lLevel,const CRJStringUnicode& s) {
	auto call=[& s](CRJLogger& clLog)->void {
		clLog.Message(s);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(lPart, lLevel, cb);
}
void CRJLoggerThread::Int64(size_t lPart, size_t lLevel,const CRJStringByte& clMsg, int64_t v) {
	auto call=[& clMsg, v](CRJLogger& clLog)->void {
		clLog.Int64(clMsg, v);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(lPart, lLevel, cb);
}
void CRJLoggerThread::Int64(size_t lPart, size_t lLevel,const CRJStringUnicode& clMsg, int64_t v) {
	auto call=[& clMsg, v](CRJLogger& clLog)->void {
		clLog.Int64(clMsg, v);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(lPart, lLevel, cb);
}
void CRJLoggerThread::Size(size_t lPart, size_t lLevel,const CRJStringByte& clMsg, size_t v) {
	auto call=[& clMsg, v](CRJLogger& clLog)->void {
		clLog.Size(clMsg, v);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(lPart, lLevel, cb);
}
void CRJLoggerThread::Size(size_t lPart, size_t lLevel,const CRJStringUnicode& clMsg, size_t v) {
	auto call=[& clMsg, v](CRJLogger& clLog)->void {
		clLog.Size(clMsg, v);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(lPart, lLevel, cb);
}
void CRJLoggerThread::Text(size_t lPart, size_t lLevel,const CRJStringByte& clMsg,const CRJStringByte& clText) {
	auto call=[& clMsg,& clText](CRJLogger& clLog)->void {
		clLog.Text(clMsg, clText);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(lPart, lLevel, cb);
}
void CRJLoggerThread::Text(size_t lPart, size_t lLevel,const CRJStringByte& clMsg,const CRJStringUnicode& clText) {
	auto call=[& clMsg,& clText](CRJLogger& clLog)->void {
		clLog.Text(clMsg, clText);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(lPart, lLevel, cb);
}
void CRJLoggerThread::Text(size_t lPart, size_t lLevel,const CRJStringUnicode& clMsg,const CRJStringByte& clText) {
	auto call=[& clMsg,& clText](CRJLogger& clLog)->void {
		clLog.Text(clMsg, clText);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(lPart, lLevel, cb);
}
void CRJLoggerThread::Text(size_t lPart, size_t lLevel,const CRJStringUnicode& clMsg,const CRJStringUnicode& clText) {
	auto call=[& clMsg,& clText](CRJLogger& clLog)->void {
		clLog.Text(clMsg, clText);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(lPart, lLevel, cb);
}
void CRJLoggerThread::Dump(size_t lPart, size_t lLevel,const CRJStringByte& clMsg,const uint8_t* pData, size_t lSize) {
	auto call=[& clMsg, pData, lSize](CRJLogger& clLog)->void {
		clLog.Dump(clMsg, pData, lSize);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(lPart, lLevel, cb);
}
void CRJLoggerThread::Dump(size_t lPart, size_t lLevel,const CRJStringUnicode& clMsg,const uint8_t* pData, size_t lSize) {
	auto call=[& clMsg, pData, lSize](CRJLogger& clLog)->void {
		clLog.Dump(clMsg, pData, lSize);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(lPart, lLevel, cb);
}
void CRJLoggerThread::ErrorMessage(const CRJStringByte& s, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Message(lPart, lLevel, err.Error(s));
}
void CRJLoggerThread::ErrorInt64(const CRJStringByte& h, int64_t v, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Int64(lPart, lLevel, err.Error(h), v);
}
void CRJLoggerThread::ErrorSize(const CRJStringByte& s, size_t v, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Size(lPart, lLevel, err.Error(s), v);
}
void CRJLoggerThread::ErrorText(const CRJStringByte& h,const CRJStringByte& s, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Text(lPart, lLevel, err.Error(h), s);
}
void CRJLoggerThread::ErrorText(const CRJStringByte& p,const CRJStringUnicode& s, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Text(lPart, lLevel, err.Error(p), s);
}
void CRJLoggerThread::WarningMessage(const CRJStringByte& s, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Message(lPart, lLevel, err.Warning(s));
}
void CRJLoggerThread::WarningInt64(const CRJStringByte& p, int64_t v, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Int64(lPart, lLevel, err.Warning(p), v);
}
void CRJLoggerThread::WarningSize(const CRJStringByte& p, size_t v, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Size(lPart, lLevel, err.Warning(p), v);
}
void CRJLoggerThread::WarningText(const CRJStringByte& p,const CRJStringByte& s, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Text(lPart, lLevel, err.Warning(p), s);
}
void CRJLoggerThread::WarningText(const CRJStringByte& p,const CRJStringUnicode& s, size_t lPart, size_t lLevel) {
	CRJStringError err;
	Text(lPart, lLevel, err.Warning(p), s);
}

