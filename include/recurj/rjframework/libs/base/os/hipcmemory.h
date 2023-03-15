#pragma once
#include <recurj/rjframework/libs/base/strings/native.h>
#ifdef _rjOS_FAMILY_UNIX
#include <recurj/rjframework/libs/base/strings/filename.h>
#endif 
#include <recurj/rjframework/libs/base/os/hhandle.h>
namespace rjBase {
	class RJBASE_API HUWLIPCMemory:public RJHandle {
	public:
		HUWLIPCMemory(const CRJString&, size_t);
		virtual~HUWLIPCMemory();
		bool SharedMemoryCreate();
		bool SharedMemoryOpen(bool bOwn=false);
		uint8_t* SharedMemoryAttach();
		void SharedMemoryRelease(uint8_t*);
		bool Read(uint8_t*, size_t);
		bool Write(uint8_t*, size_t);
		static CRJStringUnicode s_prefixSharedMemory;
	protected:
		void DuplicatedCreate();
#ifdef _rjOS_FAMILY_UNIX
	protected:
		void DeleteFile();
#endif 
	protected:
		CRJStringDefault m_clName;
		size_t m_lSize=0;
		_CLASS_COPY_AND_ASSIGN_DELETE(HUWLIPCMemory)
	};
	class RJBASE_API CUOSIPCMemoryAccess {
		friend class HUWLIPCMemory;
		CUOSIPCMemoryAccess(HUWLIPCMemory&);
		~CUOSIPCMemoryAccess();
		HUWLIPCMemory& m_clMemory;
		uint8_t* m_pAccess;
		operator bool() {
			return m_pAccess!=nullptr;
		}
	};
	class RJBASE_API CUOSIPCMemoryHolder:public HUWLIPCMemory {
	public:
		CUOSIPCMemoryHolder(const CRJString&, size_t);
		virtual~CUOSIPCMemoryHolder();
	};
	class RJBASE_API CUOSIPCMemoryCustomer:public HUWLIPCMemory {
	public:
		CUOSIPCMemoryCustomer(const CRJString&, size_t);
		virtual~CUOSIPCMemoryCustomer();
	};
}

