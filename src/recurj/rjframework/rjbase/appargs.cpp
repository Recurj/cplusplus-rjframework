#include "stdafx.h"
#include <recurj/rjframework/libs/base/application/application.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/base/logger/loggerapp.h>
#include <recurj/rjframework/libs/base/strings/byte/error.h>
CRJApplicationArgs::CRJApplicationArgs(int a, CRJStringLabel** v):
	argc(a),
argv(v) {}
bool CRJApplicationArgs::NoApplication() {
	CRJLoggerApp::ErrorApp(EErrorApplication::AppErrNoApplication, cLoggerPartConfig);
	return false;
}
bool CRJApplicationArgs::CHead::SkipOption() {
	if (top<args.argc) {
		top++;
		return true;
	}
	return false;
}
bool CRJApplicationArgs::CHead::GetOption(const CRJString& opt, CRJStringByteVar& s) {
	if (top<args.argc) {
		GetValue(s);
		return true;
	}
	CRJLoggerApp::ErrorApp(EErrorApplication::AppErrOptionAbsent, opt, cLoggerPartConfig);
	return false;
}
bool CRJApplicationArgs::CHead::GetOption(const CRJString& opt, CRJStringUnicodeVar& s) {
	if (top<args.argc) {
		GetValue(s);
		return true;
	}
	CRJLoggerApp::ErrorApp(EErrorApplication::AppErrOptionAbsent, opt, cLoggerPartConfig);
	return false;
}
bool CRJApplicationArgs::CHead::GetOption(const CRJString& opt, CRJStringFlex& s) {
	if (top<args.argc) {
		GetValue(s);
		return true;
	}
	CRJLoggerApp::ErrorApp(EErrorApplication::AppErrOptionAbsent, opt, cLoggerPartConfig);
	return false;
}
void CRJApplicationArgs::CHead::GetValue() {
	if (top<args.argc) top++;
}
bool CRJApplicationArgs::Parse() {
	int first=FirstArgument();
	size_t cValue=0;
	if (argc>first) {
		CRJStringLabel* p;
		CRJStringFlex opt;
		size_t cOption=0;
		CHead head(*this, first);
		while (head.top<argc) {
			p=argv[head.top];
			if (p[0]=='-') {
				head.GetValue(opt);
				if (!OnAppCmdOption(++cOption, opt.TruncLeft(1), head)) return false;
			}
			else {
				cValue++;
				if (cValue==1&&
					CRJApplication::HasApplication()&&
					!CRJApplication::Current().AppInit()) return false;
				else if (!OnAppCmdValue(cValue, head)) return false;
			}
		}
	}
	if (!OnAppCmdValues(cValue)) {
		CRJLoggerApp::ErrorApp(EErrorApplication::AppErrCmdValuesCount, cLoggerPartConfig);
		return false;
	}
	return true;
}
bool CRJApplicationArgs::OnAppCmdOption(size_t ind,const CRJString& opt, CHead& head) {
	if (opt==CRJApplication::s_appOptionConfig) return onAppOptionConfig(head);
	else if (opt==CRJApplication::s_appOptionLog) return onAppOptionLog(head);
	else if (opt==CRJApplication::s_appOptionDebug) return onAppOptionDebug(head);
	else if (opt==CRJApplication::s_appOptionWork) return onAppOptionWork(head);
	else if (opt==CRJApplication::s_appOptionInit) return onAppOptionInit();
	return CRJApplication::Current().OnAppCmdOption(ind, opt, head);
}
bool CRJApplicationArgs::onAppOptionConfig(CHead& head) {
	CRJFileName fn;
	if (head.GetOption(CRJApplication::s_appOptionConfig, fn)) {
		if (CRJApplication::HasApplication()) {
			CRJApplication& app=CRJApplication::Current();
			return app.OnAppConfig(app.AppFileName(fn));
		}
		else return NoApplication();
	}
	return false;
}
bool CRJApplicationArgs::onAppOptionLog(CHead& head) {
	CRJFileName fn;
	if (head.GetOption(CRJApplication::s_appOptionLog, fn)) {
		if (CRJApplication::HasApplication()) {
			CRJApplication::Current().AppSetLogPath(fn.BuildPath());
			return true;
		}
		else return NoApplication();
	}
	return false;
}
bool CRJApplicationArgs::onAppOptionWork(CHead& head) {
	CRJFileName fn;
	if (head.GetOption(CRJApplication::s_appOptionLog, fn)) {
		CRJFilePath::SetAppWorking(fn);
		return true;
	}
	return false;
}
bool CRJApplicationArgs::onAppOptionDebug(CHead& head) {
	CRJStringFlex f;
	size_t d;
	if (head.GetOption(CRJApplication::s_appOptionDebug, f)&&
		f.IsValidSize(d)) {
		if (CRJApplication::HasApplication()) {
			CRJApplication::Current().m_clAppLogger.GetWriter().SetLogLevel(d);
			return true;
		}
		else return NoApplication();
	}
	return false;
}
bool CRJApplicationArgs::onAppOptionInit() {
	if (CRJApplication::HasApplication()) return CRJApplication::Current().AppInit();
	else return NoApplication();
}
bool CRJApplicationArgs::OnAppCmdValue(size_t ind, CHead& head) {
	return CRJApplication::Current().OnAppCmdValue(ind, head);
}
bool CRJApplicationArgs::OnAppCmdValues(size_t) {
	return true;
}

