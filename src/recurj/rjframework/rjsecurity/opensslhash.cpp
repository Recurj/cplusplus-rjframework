#include "stdafx.h"
#include <recurj/rjframework/libs/security/opensslhash.h>
using namespace rjSecurity;
CRJSecurityHash::CRJSecurityHash() {
	m_pCTX=EVP_MD_CTX_new();
}
CRJSecurityHash::~CRJSecurityHash() {
	try {
		if (m_pCTX) EVP_MD_CTX_free(m_pCTX);
	}
	catch(...) {}
}
CRJSecurityHashNamed::CRJSecurityHashNamed(const char* p) {
	EVP_DigestInit(m_pCTX, EVP_get_digestbyname(p));
}
CRJSecurityHashNamed::~CRJSecurityHashNamed() {}
bool CRJSecurityHashNamed::Get(CRJStringByteVar& s) {
	unsigned int l=(unsigned int) s.GetSize();
	if (EVP_DigestFinal(m_pCTX, s.GetHeadForUpdate(),& l)) return s.SetLength(l).OK();
	return false;
}
CRJSecurityHashNamed& CRJSecurityHashNamed::Add(const void* b, size_t l) {
	EVP_DigestUpdate(m_pCTX, b,(unsigned int) l);
	return *this;
}
CRJSecurityHashSha512::CRJSecurityHashSha512():
	CRJSecurityHashNamed("sha512") {}
CRJSecurityHashSha1::CRJSecurityHashSha1():
	CRJSecurityHashNamed("SHA1") {}

