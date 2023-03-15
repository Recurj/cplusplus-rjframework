#include "stdafx.h"
#include <recurj/rjframework/libs/base/os/hevent.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/tasks/task.h>
using namespace rjBase;
#ifdef _rjOS_WIN
DWORD RJEvent::GetTimeout(size_t d) {
	if (d!=cHandleWaitEndless) return (DWORD)(d& 0x7FFFFFFF);
	return INFINITE;
}
#endif 
 void CRJEventHandler::OnEventHandlerError(int64_t) {
	CRJTask::ThreadYield();
}
void CRJEventHandler::OnEventHandlerTimeout() {}
bool CRJEventHandler::HandlerDefault(size_t) {
	return true;
};
RJEvent::RJEvent() {
	notReady();
}
RJEvent::~RJEvent() {}
void RJEvent::notReady() {
	m_clSet=& RJEvent::npFuncNotReady;
	m_clReset=& RJEvent::npFuncNotReady;
	m_clPulse=& RJEvent::npFuncNotReady;
}
#ifdef _rjOS_LINUX
 bool RJEvent::EventCreate() {
	CRJHandleCreator clCreator(*this);
	try {
		if (latchOpenChecked()) clCreator.SetResult(HandleAttach(::eventfd(0, EFD_CLOEXEC)));
	}
	catch(...) {}
	return clCreator.GetResult();
}
bool RJEvent::npFuncSet() {
	uint64_t u=1;
	return write(m_hHandle,& u,sizeof(uint64_t))==0;
}
bool RJEvent::npFuncReset() {
	uint64_t u;
	return read(m_hHandle,& u,sizeof(uint64_t))==0;
}
bool RJEvent::npFuncPulse() {
	return npFuncReset()&&
		npFuncSet();
}
bool RJEvent::WaitMe(size_t dw) {
	try {
		if (RJHandle::HandleIsValid(m_hHandle)) {
			pollfd pollfds[1];
			pollfds[0].fd=m_hHandle;
			pollfds[0].events=POLLIN;
			return poll(pollfds, 1, dw)>0;
		}
	}
	catch(...) {}
	return false;
}
#endif 
#ifdef _rjOS_WIN
 bool RJEvent::EventCreate() {
	CRJHandleCreator clCreator(*this);
	try {
		clCreator.SetResult(HandleAttach(::CreateEvent(nullptr,true,false,nullptr)));
	}
	catch(...) {}
	return clCreator.GetResult();
}
bool RJEvent::npFuncSet() {
	return ::SetEvent(m_hHandle)& 1;
}
bool RJEvent::npFuncReset() {
	return ::ResetEvent(m_hHandle)& 1;
}
bool RJEvent::npFuncPulse() {
	return ::PulseEvent(m_hHandle)& 1;
}
bool RJEvent::WaitMe(size_t dw) {
	try {
		if (RJHandle::HandleIsValid(m_hHandle)) return (::WaitForSingleObject(m_hHandle, RJEvent::GetTimeout(dw))==WAIT_OBJECT_0);
	}
	catch(...) {}
	return false;
}
#endif 
 bool RJEvent::EventSet() {
	return (this->* m_clSet)();
}
bool RJEvent::EventReset() {
	return (this->* m_clReset)();
}
bool RJEvent::EventPulse() {
	return (this->* m_clPulse)();
}
bool RJEvent::npFuncNotReady() {
	return false;
}
bool RJEvent::HandleDestroy() {
	notReady();
	return RJHandle::HandleDestroy();
}
void RJEvent::HandleAttached() {
	m_clSet=& RJEvent::npFuncSet;
	m_clReset=& RJEvent::npFuncReset;
	m_clPulse=& RJEvent::npFuncPulse;
#ifdef _DEBUG
	RJHandle::HandleAttached();
#endif 
}

