#include "stdafx.h"
#include <recurj/rjframework/libs/base/os/hipcmutex.h>
#include <recurj/rjframework/libs/base/datetime/dtimebinary.h>
#include <recurj/rjframework/libs/base/os/hevent.h>
#include <recurj/rjframework/errorcodes.h>
using namespace rjBase;
HUWLIPCMutex::HUWLIPCMutex() {
	m_lHooked=0;
#ifdef _rjOS_FAMILY_UNIX
	m_pSemaphore=SEM_FAILED;
#endif 
}
HUWLIPCMutex::~HUWLIPCMutex() {
	MutexRelease();
}
void HUWLIPCMutex::setName(const CRJStringByte& s) {
	m_clName.Set(HUWLIPCMutex::s_prefixMutex).Cat(s).MakeCLabel();
}
bool HUWLIPCMutex::MutexLeave() {
	if (!m_lHooked) return true;
	m_lHooked--;
	return mtxRelease();
}
void HUWLIPCMutex::MutexRelease() {
	while (m_lHooked>0) {
		mtxRelease();
		m_lHooked--;
	}
	mtxClose();
}
void HUWLIPCMutex::DuplicatedCreate() {
	auto call=[this](CRJLogger& clLogger)->void {
		clLogger.Text(m_clName,"Try create once more");
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartOS, cLoggerLevelCritical, cb);
}
#ifdef _rjOS_FAMILY_UNIX
CRJStringUnicode HUWLIPCMutex::s_prefixMutex("tmp/mutex");
bool HUWLIPCMutex::MutexCreate(bool bOwn) {
	try {
		if (latchOpenChecked()) {
			int v;
			if (bOwn) {
				m_lHooked++;
				v=0;
			}
			else v=1;
			m_pSemaphore=sem_open(m_clName, O_CREAT, O_RDWR, v);
			if (m_pSemaphore==SEM_FAILED) CRJLoggerThread::ErrorOs(EErrorOs::AppErrMutexCreate, m_clName);
		}
		else DuplicatedCreate();
	}
	catch(...) {}
	return false;
}
bool HUWLIPCMutex::MutexOpen(bool bOwn) {
	try {
		if (latchOpenChecked()) {
			int v;
			if (bOwn) {
				m_lHooked++;
				v=0;
			}
			else v=1;
			m_pSemaphore=sem_open(m_clName, 0, O_RDWR, v);
			if (m_pSemaphore==SEM_FAILED) CRJLoggerThread::ErrorOs(EErrorOs::AppErrMutexOpen, m_clName);
		}
		else DuplicatedCreate();
	}
	catch(...) {}
	return false;
}
bool HUWLIPCMutex::MutexEnter(int64_t d) {
	try {
		if (m_pSemaphore!=SEM_FAILED) {
			struct timespec timeout;
			CRJDTimeTools::BuildTimeOut(timeout, d& 0x7FFFFFFFFFFFFFFF);
			if (sem_timedwait(m_pSemaphore,& timeout)==0) return true;
			CRJLoggerThread::ErrorOs(EErrorOs::AppErrMutexEnter, m_clName);
		}
	}
	catch(...) {}
	return false;
}
bool HUWLIPCMutex::mtxRelease() {
	if (m_pSemaphore==SEM_FAILED) return true;
	try {
		if (sem_post(m_pSemaphore)==0) return true;
		else CRJLoggerThread::ErrorOs(EErrorOs::AppErrMutexRelease, m_clName);
	}
	catch(...) {}
	return false;
}
void HUWLIPCMutex::mtxClose() {
	if (m_pSemaphore==SEM_FAILED) return ;
	try {
		if (sem_close(m_pSemaphore)==0) m_pSemaphore=SEM_FAILED;
		else CRJLoggerThread::ErrorOs(EErrorOs::AppErrMutexClose, m_clName);
	}
	catch(...) {}
}
#endif 
#ifdef _rjOS_WIN
CRJStringUnicode HUWLIPCMutex::s_prefixMutex(L"Local\\Mutex");
bool HUWLIPCMutex::MutexCreate(bool bOwn) {
	CRJHandleCreator clCreator(*this);
	try {
		if (clCreator.SetResult(HandleAttach(::CreateMutexW(nullptr, bOwn, m_clName)))) {
			if (bOwn) m_lHooked++;
		}
		else CRJLoggerThread::ErrorOs(EErrorOs::AppErrMutexCreate, m_clName);
	}
	catch(...) {}
	return clCreator.GetResult();
}
bool HUWLIPCMutex::MutexOpen(bool bOwn) {
	CRJHandleCreator clCreator(*this);
	try {
		if (clCreator.SetResult(HandleAttach(::OpenMutexW(MUTEX_ALL_ACCESS, bOwn, m_clName)))) {
			if (bOwn) m_lHooked++;
		}
		else CRJLoggerThread::ErrorOs(EErrorOs::AppErrMutexOpen, m_clName);
	}
	catch(...) {}
	return clCreator.GetResult();
}
bool HUWLIPCMutex::MutexEnter(int64_t dw) {
	try {
		switch (::WaitForSingleObject(m_hHandle, RJEvent::GetTimeout(dw))) {
		case WAIT_OBJECT_0:m_lHooked++;
			return true;
		case WAIT_FAILED:CRJLoggerThread::ErrorOs(EErrorOs::AppErrMutexEnter, m_clName);
		}
	}
	catch(...) {}
	return false;
}
bool HUWLIPCMutex::mtxRelease() {
	try {
		if (::ReleaseMutex(m_hHandle)) return true;
		CRJLoggerThread::ErrorOs(EErrorOs::AppErrMutexRelease, m_clName);
		return false;
	}
	catch(...) {}
	return false;
}
void HUWLIPCMutex::mtxClose() {
	HandleRelease();
}
#endif 
CUOSIPCMutexHolder::CUOSIPCMutexHolder() {}
CUOSIPCMutexHolder::~CUOSIPCMutexHolder() {}
bool CUOSIPCMutexHolder::Open(const CRJStringByte& s) {
	setName(s);
	return MutexCreate(false);
}
CUOSIPCMutexCustomer::CUOSIPCMutexCustomer() {}
CUOSIPCMutexCustomer::~CUOSIPCMutexCustomer() {}
bool CUOSIPCMutexCustomer::Open(const CRJStringByte& s) {
	setName(s);
	return MutexOpen(false);
}

