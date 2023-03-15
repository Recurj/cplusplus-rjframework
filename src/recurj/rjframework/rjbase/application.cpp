#include "stdafx.h"
#include <recurj/rjframework/libs/base/application/application.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#ifdef _rjOS_WIN
#include <shlobj.h>
#endif 
#include <csignal>
#include <iostream>
#include <recurj/rjframework/libs/base/strings/byte/error.h>
#include <recurj/rjframework/libs/base/logger/loggerapp.h>
using namespace rjBase;
#ifdef _rjOS_WIN	
void _cdecl onAppExit() {
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif 
}
BOOL WINAPI CtrlHandlerRoutine(DWORD fdwCtrlType) {
	try {
		switch (fdwCtrlType) {
		case CTRL_C_EVENT:
		case CTRL_CLOSE_EVENT:CRJApplication::AppExit(-2);
			return TRUE;
		case CTRL_BREAK_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT: return FALSE;
		default:
			return FALSE;
		}
	}
	catch (...) {}
	return TRUE;
}
void SETranslator(uint32_t uCode, EXCEPTION_POINTERS*) {
	if (uCode == EXCEPTION_STACK_OVERFLOW) {}
	else {}
}
void HandlerCCtrl(int) {
	CRJApplication::AppExit(-1);
}
#endif
void CRJApplication::SetAppLoggerWriter(CRJLoggerWriter& v) {
	m_clAppLogger.SetWriter(v);
}
void CRJApplication::AppExit(int32_t) {
	try {
		if (CRJApplication::s_pApplication) {
			CRJApplication::s_pApplication->AppBreak();
			CRJApplication::s_pApplication = nullptr;
		}
	}
	catch (...) {}
}

CRJApplication::CRJApplication(CRJLoggerWriter& w) :CRJApplicationMemory() {
	CRJApplication::s_pApplication = this;
	CRJLoggerThread::Prepare();
	m_lAppDTStart = CRJDTimeTools::GetDTime();
	m_clAppLogger.SetWriter(w);
	CRJLoggerThread::Set(m_clAppLogger);
	std::locale::global(std::locale(""));
	auto v = std::locale();
	std::cin.imbue(v);
	std::cout.imbue(v);
	std::cerr.imbue(v);
#ifdef _rjOS_WIN	
	std::atexit(onAppExit);
	::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOALIGNMENTFAULTEXCEPT | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
	::_set_se_translator(SETranslator);
#endif 
}
CRJApplication::~CRJApplication() {
	AppLogDone();
	m_clAppLogger.GetWriter().Stop();
	m_clAppLogger.SetWriterOff();
}
CRJFileName& CRJApplication::LogWriterStorage(CRJFileName& fn) {
	CRJFileName n;
	if (m_clLogPath.IsEmpty()) AppGetPath(fn);
	else fn.Set(m_clLogPath);
	fn.BuildPath().Cat(CRJLogWriterFile::s_appLogPath).Cat(AppGetName(n));
	return fn;
}
void CRJApplication::AppBreak() {
	AppLogDone();
	m_clAppLogger.GetWriter().Stop();
}
void CRJApplication::AppLogStarted() {
	if (m_clAppLogger.Check(cLoggerPartInfo, cLoggerLevelCritical)) {
		CRJStringDTime dt(m_lAppDTStart);
		m_clAppLogger.Text(CRJApplication::s_appStarted, dt);
		OnAppLogStart();
	}
}
void CRJApplication::AppLogDone() {
	if (m_clAppLogger.GetWriter().IsReady() && m_clAppLogger.Check(cLoggerPartInfo, cLoggerLevelCritical)) {
		CRJStringExpanded s;
		time_t t = CRJDTimeTools::GetDTime();
		CRJStringDTime dt(t);
		OnAppLogDone();
		m_clAppLogger.Text(CRJApplication::s_appDone, s.Set(dt).Cat(s_appDuration).CatInt64(t - m_lAppDTStart + 1).CatSingle(')'));
	};
}
void CRJApplication::OnAppLogStart() {
	m_clAppLogger.Text("Version", CRJDataTools::s_szVersion);
}
void CRJApplication::OnAppLogDone() {}
void CRJApplication::AppCtrlC() {
	std::signal(SIGINT, HandlerCCtrl);
	std::signal(SIGBREAK, HandlerCCtrl);
}
bool CRJApplication::AppPrepareLog() {
	return m_clAppLogger.GetWriter().Start(*this);
}
bool CRJApplication::AppStart(CRJApplicationArgs& args) {
	CRJFileName fn;
	m_clAppExe.Set(fn.GetAppExec());
	if (args.Parse()) return AppInit();
	return false;
}
bool CRJApplication::AppInit() {
	if (m_clAppLogger.GetWriter().IsReady()) return true;
	if (AppPrepareLog()) {
		if (!CRJLoggerThread::IsValid()) m_clAppLogger.Message(CRJApplication::s_appLogError);
		AppLogStarted();
		return true;
	}
	AppNoLogFile(-1);
	return false;
}
CRJFileName& CRJApplication::AppSetConfigDefault(CRJFileName& s) {
	CRJFileName n;
	AppGetPath(s).Cat(AppGetName(n)).Cat(CRJApplication::s_appConfigExt);
	return s;
}
CRJFileName& CRJApplication::AppFileName(CRJFileName& fn) {
	CRJFileName p;
	return fn.BuildPathAbsolute(AppGetPath(p));
}
void CRJApplication::AppNoLogFile(int lRC) {
	printf("Could not open log file\n");
	exit(lRC);
}
CRJStringByte& CRJApplication::AppGetOption(size_t, CRJStringByteFlex& v) {
	return v.SetEmpty();
}

bool CRJApplication::OnAppConfig(CRJFileName& fn) {
	CRJLoggerApp::ErrorApp(EErrorApplication::AppErrConfigBad, fn, cLoggerPartConfig);
	return false;
}
bool CRJApplication::OnAppCmdOption(size_t, const CRJString& opt, CRJApplicationArgs::CHead&)
{
	CRJLoggerApp::ErrorApp(EErrorApplication::AppErrOptionUnexpected, opt, cLoggerPartConfig);
	return false;
}
bool CRJApplication::OnAppCmdValue(size_t, CRJApplicationArgs::CHead& head) {
	head.GetValue();
	return true;
}



#ifdef _rjOS_WIN
CRJFileName& CRJApplication::AppCurrentUserPath(CRJFileName& s) {
	WCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {
		s.SetLabel(path);
	}
	else m_clAppExe.GetPath(s);
	return s;
}
#endif 
void CRJApplication::AppSetLogLevels(const SRJLoggerLevels& d) {
	AppLogger().SetLogLevels(d);
}
void CRJApplication::AppSetLog(size_t d, const CRJFileName& fn) {
	AppLogger().GetWriter().SetLogLevel(d);
	m_clLogPath.Set(fn);
}
void CRJApplication::AppSetLogPath(const CRJFileName& fn) {
	m_clLogPath.Set(fn);
}

