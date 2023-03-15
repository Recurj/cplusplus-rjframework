#include "stdafx.h"
#include <recurj/rjframework/libs/security/openssl.h>
#include <recurj/rjframework/libs/security/opensslx509.h>
#define THIS_X509 ((X509 *)m_hSSLHandle)
using namespace rjSecurity;
CRJSecurityX509::CRJSecurityX509(openSSLHandle h):
	CRJSecurityHandle(h) {}
CRJSecurityX509::~CRJSecurityX509() {}
void CRJSecurityX509::SSLHandleFree() {
	try {
		if (SSLHandleIsValid()) {
			X509_free(THIS_X509);
		}
	}
	catch(...) {}
	CRJSecurityHandle::SSLHandleFree();
}

