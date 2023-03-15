#pragma once
#include <recurj/rjframework/libs/base/application/memory.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <recurj/rjframework/libs/base/logger/writers/fileapp.h>
#include <recurj/rjframework/libs/base/logger/writers/task.h>
namespace rjBase {
	enum EApplicationOptions:size_t {
		appOptionName=0, appOptionParametersRoot, appOptionDBMutex,
	};
	class RJBASE_API CRJApplicationArgs {
	public:
		CRJApplicationArgs(int, CRJStringLabel**);
		class RJBASE_API CHead {
		public:
			CHead(CRJApplicationArgs& a,int t):args(a),
			top(t) {}
			bool SkipOption();
			bool GetOption(const CRJString&, CRJStringByteVar&);
			bool GetOption(const CRJString&, CRJStringUnicodeVar&);
			bool GetOption(const CRJString&, CRJStringFlex&);
			void GetValue();
			inline CRJStringByteVar& GetValue(CRJStringByteVar& s) {return s.SetLabel(args.argv[top++]);}
			inline CRJStringUnicodeVar& GetValue(CRJStringUnicodeVar& s) {return s.SetLabel(args.argv[top++]);}
			inline CRJStringFlex& GetValue(CRJStringFlex& s) {return s.Set(args.argv[top++]);}
			CRJApplicationArgs& args;
			int top;
		};
		virtual int FirstArgument() {
			return 1;
		}
		virtual bool OnAppCmdOption(size_t,const CRJString&, CHead&);
		virtual bool OnAppCmdValue(size_t, CHead&);
		virtual bool OnAppCmdValues(size_t);
		virtual bool Parse();
		virtual bool NoApplication();
		inline CRJStringLabel* GetProgram() {return argv&&FirstArgument()==1?argv[0]:nullptr;}
		inline int Count() const {return argc;}
		inline CRJStringLabel** Arguments() const {	return argv;}
		inline CRJStringLabel* GetArg(size_t ind) const {return argv[ind];}
		inline CRJStringByteVar& GetArg(size_t ind, CRJStringByteVar& s) const {return s.SetLabel(argv[ind]);}
		inline CRJStringUnicodeVar& GetArg(size_t ind, CRJStringUnicodeVar& s) const {
			return s.SetLabel(argv[ind]);
		}
	protected:
		virtual bool onAppOptionConfig(CHead&);
		virtual bool onAppOptionLog(CHead&);
		virtual bool onAppOptionWork(CHead&);
		virtual bool onAppOptionDebug(CHead&);
		virtual bool onAppOptionInit();
	protected:
		int argc;
		CRJStringLabel** argv;
	};
	class CRJLoggerWriter;
	class RJBASE_API CRJApplication:public CRJApplicationMemory, public IRJLogWriterManager {
	public:
		friend class CRJApplicationArgs;
		friend class CRJLoggerApp;
		CRJApplication(CRJLoggerWriter&);
		CRJFileName& LogWriterStorage(CRJFileName&) override;
		virtual~CRJApplication();
		virtual bool AppStart(CRJApplicationArgs&);
		virtual void AppBreak();
		virtual CRJStringByte& AppGetOption(size_t, CRJStringByteFlex&);
		CRJFileName& AppFileName(CRJFileName&);
		inline CRJApplication& GetApplicationItSelf() {return *this;}
		inline time_t AppGetDTStart() const {return m_lAppDTStart;}
		inline const CRJStringUnicode& AppGetLogPath() const {return m_clLogPath;}
		inline CRJFileName& AppGetPath(CRJFileName& fn) {return m_clAppExe.GetPath(fn);}
		inline CRJFileName& AppGetName(CRJFileName& fn) {return m_clAppExe.GetName(fn);}
		inline CRJLogger& AppLogger() {return m_clAppLogger;}
		void AppSetLogLevels(const SRJLoggerLevels&);
		void AppSetLogPath(const CRJFileName&);
		void AppSetLog(size_t,const CRJFileName&);
#ifdef _rjOS_WIN
		CRJFileName& AppCurrentUserPath(CRJFileName&);
#endif 
		static void AppCtrlC();
		static bool HasApplication() {
			return CRJApplication::s_pApplication!=nullptr;
		}
		static CRJApplication& Current() {
			return * CRJApplication::s_pApplication;
		}
		static void AppExit(int32_t);
		static CRJApplication* s_pApplication;
		static const CRJStringByte s_NoMemory;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJApplication)
	protected:
		virtual bool AppPrepareLog();
		virtual void OnAppLogStart();
		virtual void OnAppLogDone();
		virtual void AppNoLogFile(int);
		virtual bool OnAppConfig(CRJFileName&);
		virtual bool OnAppCmdOption(size_t, const CRJString&, CRJApplicationArgs::CHead&);
		virtual bool OnAppCmdValue(size_t, CRJApplicationArgs::CHead&);
		virtual bool AppInit();
		CRJFileName& AppSetConfigDefault(CRJFileName&);
		void SetAppLoggerWriter(CRJLoggerWriter&);
		void AppLogStarted();
		void AppLogDone();
	public:
		static CRJString s_utf8;
		static CRJString s_appStarted;
		static CRJString s_appDone;
		static CRJString s_appDuration;
		static CRJString s_appLogError;
		static CRJString s_appConfigExt;
		static CRJString s_appOptionConfig;
		static CRJString s_appOptionLog;
		static CRJString s_appOptionDebug;
		static CRJString s_appOptionInit;
		static CRJString s_appOptionWork;
		static std::shared_mutex s_appDebugMutex;
		CRJFileName m_clAppExe;
		CRJLogger m_clAppLogger;
		CRJStringAllocated m_clLogPath;
		time_t m_lAppDTStart;
	};
	template<typename T>
	class CAppBuilderLoggerFile {
	public:
		CAppBuilderLoggerFile() {
			pApp=new T(m_clLog);
		}
		~CAppBuilderLoggerFile() {
			RemoveApp();
		}
		T& Get() {
			return * pApp;
		}
		operator bool() const {
			return pApp!=nullptr;
		}
		operator T&() {
			return * pApp;
		}
	protected:
		void RemoveApp() {
			try {
				if (pApp) delete pApp;
			}
			catch(...) {}
		}
		T* pApp=nullptr;
		CRJLogWriterFileApp m_clLog;
	};
	template<typename T>
	class CAppBuilderLoggerTask {
	public:
		CAppBuilderLoggerTask():
			m_clLogWriter(m_clLogStorage) {
			pApp=new T(m_clLogWriter);
		}
		~CAppBuilderLoggerTask() {
			RemoveApp();
			m_clLogWriter.QueueReset();
		}
		T& Get() {
			return * pApp;
		}
		operator bool() const {
			return pApp!=nullptr;
		}
		operator T&() {
			return * pApp;
		}
	protected:
		void RemoveApp() {
			try {
				if (pApp)delete pApp;
			}
			catch(...) {}
		}
		T* pApp=nullptr;
		CRJLogWriterFileApp m_clLogStorage;
		CRJLogWriterTask m_clLogWriter;
	};
}

