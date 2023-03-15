#pragma once
#include <recurj/rjframework/libs/security/opensslhandle.h>
namespace rjSecurity {
	class RJSECURITY_API CRJSecurityRSAContext:public CRJSecurityHandle {
	public:
		CRJSecurityRSAContext();
		virtual~CRJSecurityRSAContext();
		openSSLHandle Create(size_t);
	protected:
		void SSLHandleFree() override;
		void SSLRSAContextAlloc();
	};
}

