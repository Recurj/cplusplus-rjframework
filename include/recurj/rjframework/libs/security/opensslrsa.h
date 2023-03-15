#pragma once
#include <recurj/rjframework/libs/security/opensslbio.h>
namespace rjSecurity {
	class RJSECURITY_API CRJSecurityRSA:public CRJSecurityHandle {
		friend class CRJSecurityPublicKeyEx;
	public:
		CRJSecurityRSA(size_t);
		CRJSecurityRSA(CRJSecurityBIO&,bool);
		virtual~CRJSecurityRSA();
		bool pSaveToFile(FILE*,bool);
	protected:
		CRJSecurityRSA();
		void SSLRSAAlloc(CRJSecurityBIO&,bool);
		void SSLRSAAlloc(size_t);
		void SSLHandleFree() override;
	};
}

