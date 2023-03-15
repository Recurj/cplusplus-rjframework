#pragma once
#include <recurj/rjframework/libs/base/os/hhandle.h>
#include <recurj/rjframework/iname.h>
namespace rjBase {
	const size_t cHandleWaitEndless=(size_t)-1;
	const size_t cHandleWaitLong=0x7FFFFFFFFFFFFFFF;
	class RJBASE_API CRJEventHandler:public IRJNamed {
	public:
		using RJHandler=bool(CRJEventHandler::*)(size_t);
		virtual void OnEventHandlerError(int64_t);
		virtual void OnEventHandlerTimeout();
		bool HandlerDefault(size_t);
	};
	class RJBASE_API RJEvent:public RJHandle {
	public:
		using funcAction=bool(RJEvent::*)();
		RJEvent();
		virtual~RJEvent();
		bool EventCreate();
		bool EventSet();
		bool EventReset();
		bool EventPulse();
		bool WaitMe(size_t);
#ifdef _rjOS_WIN
		static DWORD GetTimeout(size_t);
#endif 
	protected:
		bool HandleDestroy() override;
		void HandleAttached() override;
		bool npFuncNotReady();
		bool npFuncSet();
		bool npFuncReset();
		bool npFuncPulse();
		_CLASS_COPY_AND_ASSIGN_DELETE(RJEvent)
	protected:
		void notReady();
	protected:
		funcAction m_clSet;
		funcAction m_clReset;
		funcAction m_clPulse;
	};
	class RJBASE_API RJEventDuplicated:public RJEvent {
	public:
		RJEventDuplicated() {}
		virtual~RJEventDuplicated() {
			HandleClear();
		}
		bool Duplicate(HOS h) {
			return HandleAttach(h);
		}
		void Close() {
			HandleClear();
		}
	};
}

