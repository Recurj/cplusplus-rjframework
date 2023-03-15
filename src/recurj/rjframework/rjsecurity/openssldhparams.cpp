#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/security/openssl.h>
#include <recurj/rjframework/libs/security/openssldhparams.h>
#include <recurj/rjframework/libs/security/opensslbio.h>
#define THIS_DH ((DH *)m_hSSLHandle)
using namespace rjSecurity;
CRJSecurityDHParams::CRJSecurityDHParams() {}
CRJSecurityDHParams::~CRJSecurityDHParams() {}
void CRJSecurityDHParams::SSLHandleFree() {
	try {
		if (SSLHandleIsValid()) {
			DH_free(THIS_DH);
		}
	}
	catch(...) {}
	CRJSecurityHandle::SSLHandleFree();
}
bool CRJSecurityDHParams::LoadFromFile(const CRJStringByte& clPath, size_t lSize) {
	try {
		CRJStringByteDefault fn(clPath);
		fn.CatLabel("DH", 2).CatSize(lSize).CatLabel(".pem", 4);
		if (fn.MakeCLabel().OK()) {
			CRJSecurityBIO clBIO(BIO_new_file((char*) fn.GetHeadForRead(),"r"));
			if (clBIO.SSLHandleIsValid()) {
				return SSLHandleAttach(PEM_read_bio_DHparams((BIO*) clBIO.m_hSSLHandle, NULL, NULL, NULL));
			}
		}
	}
	catch(...) {}
	return false;
}

