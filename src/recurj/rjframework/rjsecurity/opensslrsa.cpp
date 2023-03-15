#include "stdafx.h"
#include <recurj/rjframework/libs/security/openssl.h>
#include <recurj/rjframework/libs/security/opensslrsa.h>
using namespace rjSecurity;
#define THIS_RSA ((RSA *)m_hSSLHandle)
#define THIS_RSA_POINTER ((RSA **)&m_hSSLHandle)
CRJSecurityRSA::CRJSecurityRSA() {}
CRJSecurityRSA::CRJSecurityRSA(size_t bits) {
	SSLRSAAlloc(bits);
}
CRJSecurityRSA::CRJSecurityRSA(CRJSecurityBIO& bio,bool bPublic) {
	SSLRSAAlloc(bio, bPublic);
}
CRJSecurityRSA::~CRJSecurityRSA() {
	SSLHandleFree();
}
bool CRJSecurityRSA::pSaveToFile(FILE* f,bool bPublic) {
	int32_t rc=(bPublic)?
		PEM_write_RSAPublicKey(f, THIS_RSA):
		PEM_write_RSAPrivateKey(f, THIS_RSA,nullptr,nullptr, 0,nullptr,nullptr);
	if (rc!=1) return SSLHandleError();
	return true;
}
void CRJSecurityRSA::SSLHandleFree() {
	try {
		if (SSLHandleIsValid()) {
			RSA_free(THIS_RSA);
		}
	}
	catch(...) {}
	CRJSecurityHandle::SSLHandleFree();
}
void CRJSecurityRSA::SSLRSAAlloc(size_t bits) {
	try {
		SSLHandleAttach(RSA_generate_key((int32_t) bits, RSA_F4,nullptr,nullptr));
		if (!SSLHandleIsValid()) SSLHandleError();
	}
	catch(...) {}
}
void CRJSecurityRSA::SSLRSAAlloc(CRJSecurityBIO& bio,bool bPublic) {
	try {
		if (bPublic) PEM_read_bio_RSA_PUBKEY((BIO*) bio.SSLHandleGet(), THIS_RSA_POINTER, NULL, NULL);
		else PEM_read_bio_RSAPrivateKey((BIO*) bio.SSLHandleGet(), THIS_RSA_POINTER, NULL, NULL);
		if (!SSLHandleIsValid()) SSLHandleError();
	}
	catch(...) {}
}

