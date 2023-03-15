#pragma once
#include <thirdparty/openssl/evp.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/modules/modsecurity.h>
using namespace rjBase;
namespace rjSecurity {
	class RJSECURITY_API CRJSecurityHash {
	public:
		CRJSecurityHash();
		virtual~CRJSecurityHash();
		operator EVP_MD_CTX*() {
			return m_pCTX;
		}
	protected:
		EVP_MD_CTX* m_pCTX;
	};
	class RJSECURITY_API CRJSecurityHashNamed:public CRJSecurityHash {
	public:
		CRJSecurityHashNamed(const char*);
		virtual~CRJSecurityHashNamed();
		bool Get(CRJStringByteVar&);
		CRJSecurityHashNamed& Add(const void*, size_t);
	};
	class RJSECURITY_API CRJSecurityHashSha1:public CRJSecurityHashNamed {
	public:
		CRJSecurityHashSha1();
	};
	class RJSECURITY_API CRJSecurityHashSha512:public CRJSecurityHashNamed {
	public:
		CRJSecurityHashSha512();
	};
}

