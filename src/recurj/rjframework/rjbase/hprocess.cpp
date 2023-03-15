#include "stdafx.h"
#include <recurj/rjframework/libs/base/os/hprocess.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/datetime/dtimebinary.h>
using namespace rjBase;
HUWLProcess::HUWLProcess() {
	ProcessReset();
}
HUWLProcess::~HUWLProcess() {
	ProcessClose();
}
#ifdef _rjOS_FAMILY_UNIX
 void HUWLProcess::ProcessReset() {
	m_pProcess=0;
	m_lOSResult=0;
	m_lExitCode=0L;
}
void HUWLProcess::ProcessClose() {
	m_pProcess=0;
}
bool HUWLProcess::ProcessRun(CRJStringVar& clArg,int flags) {
	try {
		ProcessClose();
		ProcessReset();
		m_pProcess=fork();
		if (m_pProcess>0) return true;
		if (m_pProcess==0) {
			char* pPtr;
			char* argv[uProcessMaxArgs+1];
			int argc=1;
			const char* pExec=(const char*) clArg.GetHeadForRead();
			char* pArg=(char*) strrchr(pExec,'/');
			if (pArg!=NULL) pArg++;
			else pArg=(char*) pExec;
			argv[0]=pArg;
			pArg=strtok_r(pArg," ",& pPtr);
			while (pArg!=NULL) {
				argv[argc]=pArg;
				argc++;
				if (argc>=uProcessMaxArgs)break;
				pArg=strtok_r(NULL," ",& pPtr);
			}
			argv[argc]=NULL;
			execv(pExec, argv);
			exit(-1);
		}
		else {
			m_lOSResult=CRJDataTools::GetLastError();
			CRJLoggerThread::ErrorOs("ProcessRun", cLoggerLevelCritical);
		}
	}
	catch(...) {}
	return false;
}
bool HUWLProcess::ProcessIsRunning() {
	try {
		if (m_pProcess>0) {
			if (kill(m_pProcess, 0)==-1&&
				errno==ESRCH) return false;
			int status;
			pid_t p=waitpid(m_pProcess,& status, WNOHANG);
			if (!p) return true;
			else if (p==m_pProcess) return WIFCONTINUED(status)!=0;
		}
	}
	catch(...) {}
	return false;
}
bool HUWLProcess::ProcessWait(int64_t d) {
	try {
		if (m_pProcess>0) {
			int status;
			pid_t p;
			sigset_t mask;
			sigset_t orig_mask;
			struct timespec timeout;
			sigemptyset(& mask);
			sigaddset(& mask, SIGCHLD);
			if (sigprocmask(SIG_BLOCK,& mask,& orig_mask)<0) {
				CRJLoggerThread::ErrorOs("sigprocmask", cLoggerLevelCritical);
				return false;
			}
			CRJDTimeTools::BuildTimeOut(timeout, d& 0x7FFFFFFF);
			do {
				if (sigtimedwait(& mask, NULL,& timeout)<0) {
					if (errno==EINTR)continue;
					else if (errno==EAGAIN) return false;
					else {
						CRJLoggerThread::ErrorOs("sigtimedwait", cLoggerLevelCritical);
						return false;
					}
				}
				break;
			}
			while (1);
			if (waitpid(m_pProcess,& status, 0)==m_pProcess) {
				if (WIFEXITED(status)) m_lExitCode=status;
				else m_lExitCode=uProcessTerminatedNotNormal;
				return OnProcessDone();
			}
		}
	}
	catch(...) {}
	return false;
}
bool HUWLProcess::ProcessTerminate(uint32_t rc) {
	try {
		if (m_pProcess>0) {
			int status;
			kill(m_pProcess, SIGTERM);
			return waitpid(m_pProcess,& status, WNOHANG)==m_pProcess;
		}
	}
	catch(...) {}
	return false;
}
bool HUWLProcess::OnProcessDone() {
	ProcessClose();
	return true;
}
bool HUWLProcess::Shutdown(const CRJString& clTitle, int64_t lDelay,bool bForce,bool bReboot) {
	try {}
	catch(...) {}
	return false;
}
#endif 
#ifdef _rjOS_WIN
 void HUWLProcess::ProcessReset() {
	_MEMCLR_BYTE(m_stStartup);
	_MEMCLR_BYTE(m_stProcess);
	m_stStartup.cb=sizeof(m_stStartup);
	m_lOSResult=0;
	m_lExitCode=0L;
}
void HUWLProcess::ProcessClose() {
	try {
		::InterlockedExchange((long*)& m_stProcess.dwThreadId, 0);
		HandleRelease();
		m_stProcess.hProcess=nullptr;
		if (m_stProcess.hThread) {
			::CloseHandle(m_stProcess.hThread);
			m_stProcess.hThread=nullptr;
		}
	}
	catch(...) {}
}
bool HUWLProcess::ProcessRun(CRJStringVar& clArg, uint32_t flags) {
	try {
		ProcessClose();
		ProcessReset();
#pragma warning(suppress: 6335)
		if (clArg.MakeCLabel()&&
			::CreateProcessW(nullptr, clArg.GetHeadForUpdate(),nullptr,nullptr,false, flags,nullptr,nullptr,& m_stStartup,& m_stProcess)) {
			HandleAttach(m_stProcess.hProcess);
			return true;
		}
		else m_lOSResult=CRJDataTools::GetLastError();
	}
	catch(...) {}
	return false;
}
bool HUWLProcess::ProcessIsRunning() {
	return ::WaitForSingleObject(m_stProcess.hProcess, 0)==WAIT_TIMEOUT;
}
bool HUWLProcess::ProcessWait(int64_t d) {
	try {
		if (::WaitForSingleObject(m_stProcess.hProcess, RJEvent::GetTimeout(d))==WAIT_OBJECT_0) {
			return OnProcessDone();
		}
	}
	catch(...) {}
	return false;
}
bool HUWLProcess::ProcessTerminate(uint32_t rc) {
	try {
		if (!m_stProcess.hProcess) return true;
		if (TerminateProcess(m_stProcess.hProcess, rc)) {
			return true;
		}
		else m_lOSResult=(int64_t)::GetLastError();
	}
	catch(...) {}
	return false;
}
bool HUWLProcess::OnProcessDone() {
	DWORD dw;
	GetExitCodeProcess(m_stProcess.hProcess,& dw);
	m_lExitCode=(int64_t) dw;
	ProcessClose();
	return true;
}
bool HUWLProcess::Shutdown(const CRJString& clTitle, int64_t lDelay,bool bForce,bool bReboot) {
	try {
		HOS hToken;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES| TOKEN_QUERY,& hToken)) {
			TOKEN_PRIVILEGES tkp;
			CRJStringUnicodeDefault str;
			str.Set(clTitle);
			::LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME,& tkp.Privileges[0].Luid);
			tkp.PrivilegeCount=1;
			tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
			::AdjustTokenPrivileges(hToken,false,& tkp, 0,(PTOKEN_PRIVILEGES)nullptr, 0);
#pragma warning(suppress: 28159)
			if (::InitiateSystemShutdownExW(nullptr, str.GetHeadForUpdate(),(uint32_t) lDelay, bForce, bReboot, SHTDN_REASON_MINOR_MAINTENANCE)) return true;
		}
	}
	catch(...) {}
	return false;
}
#endif 

