#pragma once
#include <recurj/rjframework/libs/security/opensslhandle.h>
#include <recurj/rjframework/libs/base/strings/byte/strings.h>
namespace rjSecurity {
	class RJSECURITY_API CRJSecurityBIO:public CRJSecurityHandle {
		friend class CRJSecurityConnection;
		friend class CRJSecurityDHParams;
	public:
		CRJSecurityBIO(openSSLHandle);
		virtual~CRJSecurityBIO();
		int64_t pGetDataLen();
		int64_t pRead(CRJStringByteVar&);
	protected:
		void SSLHandleFree() override;
	private:
		void SSLBIOReset();
		bool SSLBIOAddDataSafed(CRJStringByteFlex&);
		bool SSLBIOHasDataSafed(CRJStringByteFlex&);
	};
	class RJSECURITY_API CRJSecurityBIOMemory:public CRJSecurityBIO {
	public:
		CRJSecurityBIOMemory();
		CRJSecurityBIOMemory(const CRJStringByteFlex&);
		virtual~CRJSecurityBIOMemory();
	protected:
		void SSLBIOMemoryAlloc();
		void SSLBIOMemoryAlloc(const CRJStringByteFlex&);
	};
}

