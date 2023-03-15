#pragma once
#include <recurj/rjframework/libs/security/opensslhandle.h>
#include <recurj/rjframework/libs/base/strings/byte/strings.h>
namespace rjSecurity {
	const int SecurityPasswordIterations=50000;
	class RJSECURITY_API CRJSecurityPassword {
	public:
		CRJSecurityPassword();
		virtual~CRJSecurityPassword();
		bool Build(const CRJStringByteFlex&, CRJStringByteVarAssigned&);
	protected:
		static const char s_iv[16];
		static const uint8_t s_salt[8];
	};
}

