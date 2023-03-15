#include "stdafx.h"
#include <recurj/rjframework/libs/base/os/hevent.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/errorcodes.h>
using namespace rjBase;
RJHandle::RJHandle() {
	HandleClear();
};
RJHandle::~RJHandle() {
	HandleRelease();
}
void RJHandle::HandleClear() {
	m_hHandle=HOS_INVALID;
}
bool RJHandle::HandleRelease() {
	if (RJHandle::HandleIsValid(m_hHandle)) {
		if (HandleDestroy()) {
			HandleClear();
			return true;
		}
	}
	return false;
}
RJHandle& RJHandle::HandleOSError() {
	CRJLoggerThread::ErrorOs(EErrorOs::AppErrHandleInvalid);
	return *this;
}
bool RJHandle::HandleAttach(HOS v) {
	if (HandleIsValid(v)) {
		HandleRelease();
		m_hHandle=v;
		HandleAttached();
		return true;
	}
	else HandleOSError();
	return false;
}
void RJHandle::HandleAttached() {}
#ifdef _rjOS_FAMILY_UNIX
 bool RJHandle::HandleDestroy() {
	try {
		return ::close(m_hHandle)==0;
	}
	catch(...) {}
	return false;
}
bool RJHandle::HandleIsValid(const HOS v) {
	return v!=HOS_INVALID;
}
#endif 
#ifdef _rjOS_WIN
 bool RJHandle::HandleDestroy() {
	try {
		return ::CloseHandle(m_hHandle)& 1;
	}
	catch(...) {}
	return false;
}
bool RJHandle::HandleIsValid(const HOS v) {
	return v!=HOS_INVALID&&
		v!=nullptr;
}
#endif 

