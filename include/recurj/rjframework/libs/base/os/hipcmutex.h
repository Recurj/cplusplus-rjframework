#pragma once
#include <recurj/rjframework/libs/base/strings/native.h>
#include <recurj/rjframework/libs/base/os/hhandle.h>
namespace rjBase {
#ifdef _rjOS_FAMILY_UNIX
	class RJBASE_API HUWLIPCMutex:public CRJLatch
#endif 
#ifdef _rjOS_WIN
	class RJBASE_API HUWLIPCMutex:public RJHandle
#endif 
	{
		friend class CUOSMutexLock;
	public:
		HUWLIPCMutex();
		virtual~HUWLIPCMutex();
		bool MutexCreate(bool bOwn=false);
		bool MutexOpen(bool bOwn=false);
		bool MutexEnter(int64_t);
		bool MutexLeave();
		void MutexRelease();
		static CRJStringUnicode s_prefixMutex;
	protected:
		void DuplicatedCreate();
		void mtxClose();
		bool mtxRelease();
		void setName(const CRJStringByte&);
	protected:
		CRJStringDefault m_clName;
		size_t m_lHooked;
#ifdef _rjOS_FAMILY_UNIX
		sem_t* m_pSemaphore;
#endif 
		_CLASS_COPY_AND_ASSIGN_DELETE(HUWLIPCMutex)
	};
	class RJBASE_API CUOSIPCMutexHolder:public HUWLIPCMutex {
	public:
		CUOSIPCMutexHolder();
		virtual~CUOSIPCMutexHolder();
		bool Open(const CRJStringByte&);
	};
	class RJBASE_API CUOSIPCMutexCustomer:public HUWLIPCMutex {
	public:
		CUOSIPCMutexCustomer();
		virtual~CUOSIPCMutexCustomer();
		bool Open(const CRJStringByte&);
	};
}

