#pragma once
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/security/opensslhandle.h>
namespace rjSecurity {
	class RJSECURITY_API CRJSecurityX509:public CRJSecurityHandle {
		friend class CRJSecurityConnection;
	public:
		CRJSecurityX509(openSSLHandle);
		virtual~CRJSecurityX509();
	protected:
		void SSLHandleFree() override;
	};
}

