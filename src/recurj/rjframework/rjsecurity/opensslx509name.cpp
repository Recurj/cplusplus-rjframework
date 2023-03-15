#include "stdafx.h"
#include <cdata/security/septadaopenssl.h>
#include <cdata/security/septadaopensslx509name.h>
#define THIS_X509_NAME ((X509_NAME *)m_hSSLHandle)
CSeptadaOpenSSLX509Name::CSeptadaOpenSSLX509Name(openSSLHandle h):
	CRJSecurityHandle(h) {}
CSeptadaOpenSSLX509Name::~CSeptadaOpenSSLX509Name() {}
bool CSeptadaOpenSSLX509Name::GetValue(CRJStringByte& clStr) {
	int lLen=X509_NAME_get_text_by_NID(THIS_X509_NAME, NID_commonName, clStr.GetSafedValue(), clStr.GetSize()-1);
	if (lLen>0) {
		clStr.SetLength(lLen);
		return true;
	}
	return false;
}

