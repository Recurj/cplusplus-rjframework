#pragma once
#include <recurj/rjframework/libs/base/strings/native.h>
#include <recurj/rjframework/libs/base/os/hevent.h>
#include <recurj/rjframework/libs/base/os/hhandle.h>
namespace rjBase {
	const int64_t uProcessTerminatedNotNormal=-1;
	const size_t uProcessMaxArgs=32;
	class RJBASE_API HUWLProcess:public RJHandle {
	public:
		HUWLProcess();
		virtual~HUWLProcess();
#ifdef _rjOS_FAMILY_UNIX
		bool ProcessRun(CRJStringVar&,int fl=0);
#endif 
#ifdef _rjOS_WIN
		bool ProcessRun(CRJStringVar&, uint32_t fl=CREATE_NO_WINDOW);
#endif 
		bool ProcessIsRunning();
		bool ProcessWait(int64_t=cHandleWaitEndless);
		bool ProcessTerminate(uint32_t);
		void ProcessClose();
		void ProcessReset();
		inline int64_t ProcessGetRC() const {
			return m_lExitCode;
		}
		inline int64_t ProcessGetOSErr() const {
			return m_lOSResult;
		}
#ifdef _rjOS_FAMILY_UNIX
		inline pid_t GetProcess() const {
			return m_pProcess;
		}
		static pid_t GetIdCurrentProcess() {
			return getpid();
		}
#endif 
#ifdef _rjOS_WIN
		inline DWORD GetProcessId() const {
			return m_stProcess.dwProcessId;
		}
		static DWORD GetIdCurrentProcess() {
			return ::GetCurrentProcessId();
		}
#endif 
		static bool Shutdown(const CRJString&, int64_t lDelay=30,bool bForce=true,bool bReboot=true);
	protected:
		bool OnProcessDone();
	protected:
		int64_t m_lOSResult;
		int64_t m_lExitCode;
#ifdef _rjOS_FAMILY_UNIX
		pid_t m_pProcess;
#endif 
#ifdef _rjOS_WIN
		STARTUPINFOW m_stStartup;
		PROCESS_INFORMATION m_stProcess;
#endif 
		_CLASS_COPY_AND_ASSIGN_DELETE(HUWLProcess)
	};
}

