#pragma once
#include <recurj/rjframework/libs/security/opensslhandle.h>
namespace rjSecurity {
	class RJSECURITY_API CRJSecurityDHParams:public CRJSecurityHandle {
		friend class CRJSecurityFactory;
	public:
		CRJSecurityDHParams();
		virtual~CRJSecurityDHParams();
	protected:
		void SSLHandleFree() override;
		bool LoadFromFile(const CRJStringByte&, size_t);
	};
}

