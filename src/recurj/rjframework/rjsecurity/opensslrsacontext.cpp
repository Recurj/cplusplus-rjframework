#include "stdafx.h"
#include <recurj/rjframework/libs/security/openssl.h>
#include <recurj/rjframework/libs/security/opensslrsacontext.h>
using namespace rjSecurity;
#define THIS_EVP_CTX ((EVP_PKEY_CTX *)m_hSSLHandle)
CRJSecurityRSAContext::CRJSecurityRSAContext() {
	SSLRSAContextAlloc();
}
CRJSecurityRSAContext::~CRJSecurityRSAContext() {
	SSLHandleFree();
}
void CRJSecurityRSAContext::SSLHandleFree() {
	try {
		if (SSLHandleIsValid()) {
			EVP_PKEY_CTX_free(THIS_EVP_CTX);
		}
	}
	catch(...) {}
	CRJSecurityHandle::SSLHandleFree();
}
openSSLHandle CRJSecurityRSAContext::Create(size_t bits) {
	try {
		EVP_PKEY* h=nullptr;
#ifdef _DEBUG
		auto r=EVP_PKEY_keygen_init(THIS_EVP_CTX);
		if (r==1) {
			r=EVP_PKEY_CTX_set_rsa_keygen_bits(THIS_EVP_CTX,(int32_t) bits);
			if (r>0) {
				r=EVP_PKEY_keygen(THIS_EVP_CTX,& h);
				if (r==1) return h;
			}
		}
#else 
		if (EVP_PKEY_keygen_init(THIS_EVP_CTX)==1&&
			(EVP_PKEY_CTX_set_rsa_keygen_bits(THIS_EVP_CTX,(int32_t) bits))>0&&
			EVP_PKEY_keygen(THIS_EVP_CTX,& h)==1) return h;
#endif 
		SSLHandleError();
	}
	catch(...) {}
	return nullptr;
}
void CRJSecurityRSAContext::SSLRSAContextAlloc() {
	try {
		SSLHandleAttach(EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL));
	}
	catch(...) {}
}

