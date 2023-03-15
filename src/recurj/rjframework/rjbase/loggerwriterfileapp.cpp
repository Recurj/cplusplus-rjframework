#include "stdafx.h"
#include <recurj/rjframework/libs/base/logger/writers/fileapp.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/base/application/application.h>
using namespace rjBase;
const CRJStringByte CRJLogWriterFile::s_appLogPath("logs\\", 5);
const CRJStringByte CRJLogWriterFile::s_appLogExt(".log", 4);
std::mutex CRJLogWriterFile::s_clConsoleCritical;
bool CRJLogWriterFile::Console(const CRJStringByte& s) {
	std::lock_guard<std::mutex>lg(s_clConsoleCritical);
	std::cout<<s<<std::endl;
	return true;
}
CRJLogWriterFile::CRJLogWriterFile() {}
CRJLogWriterFile::~CRJLogWriterFile() {}
bool CRJLogWriterFile::Open(CRJFileName& fn) {
	return m_clFile.Append(fn);
}
bool CRJLogWriterFile::LogPublic(const CRJStringByte&) {
	return false;
}
bool CRJLogWriterFile::LogPrivate(const CRJStringByte& s) {
	if (!IsReady()) return Console(s);
	return m_clFile.Writeln(s)&&
		m_clFile.Flush();
}
bool CRJLogWriterFile::Start(IRJLogWriterManager& mgr) {
	CRJFileName fn;
	mgr.LogWriterStorage(fn).LowerCase().Cat(CRJLogWriterFile::s_appLogExt);
	if (fn.CreateFilePath()) {
		return Open(fn);
	}
	return false;
}
void CRJLogWriterFile::Stop() {
	m_clFile.Close();
}
CRJLogWriterFileApp::CRJLogWriterFileApp() {}
CRJLogWriterFileApp::~CRJLogWriterFileApp() {}
bool CRJLogWriterFileApp::LogPublic(const CRJStringByte& s) {
	std::lock_guard<std::mutex>lg(m_clLogCritical);
	return LogPrivate(s);
}

