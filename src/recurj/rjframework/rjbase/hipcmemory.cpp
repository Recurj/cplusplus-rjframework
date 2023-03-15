#include "stdafx.h"
#include <recurj/rjframework/libs/base/os/hipcmemory.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/errorcodes.h>
using namespace rjBase;
HUWLIPCMemory::HUWLIPCMemory(const CRJString& s, size_t sSize):
	m_lSize(sSize) {
	m_clName.Set(HUWLIPCMemory::s_prefixSharedMemory).Cat(s).MakeCLabel();
}
HUWLIPCMemory::~HUWLIPCMemory() {
#ifdef _rjOS_FAMILY_UNIX
	if (m_hHandle>0) {
		HandleRelease();
		DeleteFile();
	}
#endif 
}
#ifdef _rjOS_FAMILY_UNIX
CRJStringUnicode HUWLIPCMemory::s_prefixSharedMemory("/tmp/memory");
void HUWLIPCMemory::DeleteFile() {
	CRJFileName clFileName;
	clFileName.Set(m_clName).Remove();
}
bool HUWLIPCMemory::SharedMemoryCreate() {
	try {
		if (latchOpenChecked()) {
			DeleteFile();
			m_hHandle=open(m_clName, O_CREAT| O_RDWR, S_IRUSR| S_IWUSR| S_IRGRP| S_IWGRP);
			if (m_hHandle<0) CRJLoggerThread::ErrorOs(EErrorOs::AppErrSharedMemoryCreate, m_clName);
			else {
				char ch='\0';
				lseek(m_hHandle, m_lSize-1, SEEK_SET);
				write(m_hHandle,& ch, 1);
				if (lseek(m_hHandle, 0, SEEK_END)<m_lSize) CRJLoggerThread::ErrorOs(EErrorOs::AppErrSharedMemoryCreate, m_clName);
				return true;
			}
		}
		else DuplicatedCreate();
	}
	catch(...) {}
	return false;
}
bool HUWLIPCMemory::SharedMemoryOpen(bool) {
	try {
		if (latchOpenChecked()) {
			m_hHandle=open(m_clName, O_RDWR, S_IRUSR| S_IWUSR| S_IRGRP| S_IWGRP);
			if (m_hHandle<0) CRJLoggerThread::ErrorOs(EErrorOs::AppErrSharedMemoryOpen, m_clName);
			else if (lseek(m_hHandle, 0, SEEK_END)<m_lSize) CRJLoggerThread::ErrorOs(EErrorOs::AppErrSharedMemoryOpen, m_clName);
			return true;
		}
		else DuplicatedCreate();
	}
	catch(...) {}
	return false;
}
uint8_t* HUWLIPCMemory::SharedMemoryAttach() {
	void* pShared=mmap(0, m_lSize, PROT_READ| PROT_WRITE, MAP_PRIVATE| MAP_SHARED, m_hHandle, 0);
	if (pShared==MAP_FAILED) {
		CRJLoggerThread::ErrorOs(EErrorOs::AppErrSharedMemoryAttach, m_clName);
		return nullptr;
	}
	return (uint8_t*) pShared;
}
void HUWLIPCMemory::SharedMemoryRelease(uint8_t* p) {
	if (!p) return ;
	try {
		::munmap(p, m_lSize);
	}
	catch(...) {}
}
#endif 
#ifdef _rjOS_WIN
CRJStringUnicode HUWLIPCMemory::s_prefixSharedMemory(L"Local\\Memory");
bool HUWLIPCMemory::SharedMemoryCreate() {
	CRJHandleCreator clCreator(*this);
	try {
		if (!clCreator.SetResult(HandleAttach(::CreateFileMappingW(HOS_INVALID, NULL,//	                default security
		PAGE_READWRITE,//	                read/write access
		(DWORD) 0,//	                maximum object size (high-order DWORD)
		(DWORD) m_lSize,//	                maximum object size (low-order DWORD)
		m_clName)))) CRJLoggerThread::ErrorOs(EErrorOs::AppErrSharedMemoryCreate, m_clName);
	}
	catch(...) {}
	return clCreator.GetResult();
}
bool HUWLIPCMemory::SharedMemoryOpen(bool bOwn) {
	CRJHandleCreator clCreator(*this);
	try {
		clCreator.SetResult(HandleAttach(::OpenFileMappingW(FILE_MAP_ALL_ACCESS, bOwn, m_clName)));
	}
	catch(...) {}
	return clCreator.GetResult();
}
void HUWLIPCMemory::SharedMemoryRelease(uint8_t* p) {
	if (!p) return ;
	try {
		::UnmapViewOfFile(p);
	}
	catch(...) {}
}
uint8_t* HUWLIPCMemory::SharedMemoryAttach() {
	try {
		return (uint8_t*)::MapViewOfFile(m_hHandle, FILE_MAP_ALL_ACCESS, 0, 0, m_lSize);
	}
	catch(...) {}
	return nullptr;
}
#endif 
 bool HUWLIPCMemory::Read(uint8_t* pBuf, size_t lSize) {
	CUOSIPCMemoryAccess clAccess(*this);
	if (!clAccess) CRJLoggerThread::ErrorOs(EErrorOs::AppErrSharedMemoryRead, m_clName);
	else {
		_MEMMOVE_BYTE_POINTER(pBuf, clAccess.m_pAccess, lSize);
		return true;
	}
	return false;
}
bool HUWLIPCMemory::Write(uint8_t* pBuf, size_t lSize) {
	CUOSIPCMemoryAccess clAccess(*this);
	if (!clAccess) CRJLoggerThread::ErrorOs(EErrorOs::AppErrSharedMemoryWrite, m_clName);
	else {
		_MEMMOVE_BYTE_POINTER(clAccess.m_pAccess, pBuf, lSize);
		return true;
	}
	return false;
}
void HUWLIPCMemory::DuplicatedCreate() {
	auto call=[this](CRJLogger& clLogger)->void {
		clLogger.Text(m_clName,"Try create once more");
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartOS, cLoggerLevelCritical, cb);
}
CUOSIPCMemoryAccess::CUOSIPCMemoryAccess(HUWLIPCMemory& clMemory):
	m_clMemory(clMemory) {
	m_pAccess=m_clMemory.SharedMemoryAttach();
}
CUOSIPCMemoryAccess::~CUOSIPCMemoryAccess() {
	m_clMemory.SharedMemoryRelease(m_pAccess);
}
CUOSIPCMemoryHolder::CUOSIPCMemoryHolder(const CRJString& s, size_t l):
	HUWLIPCMemory(s, l) {
	SharedMemoryCreate();
}
CUOSIPCMemoryHolder::~CUOSIPCMemoryHolder() {}
CUOSIPCMemoryCustomer::CUOSIPCMemoryCustomer(const CRJString& s, size_t l):
	HUWLIPCMemory(s, l) {
	SharedMemoryOpen(false);
}
CUOSIPCMemoryCustomer::~CUOSIPCMemoryCustomer() {}

