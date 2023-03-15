#pragma once
#include <pod/data/septadapodata.h>
#include <cdata/security/septadaopensslhandle.h>
class SEPTADASECURITY_API CSeptadaOpenSSLX509Name:public CRJSecurityHandle {
public:
	CSeptadaOpenSSLX509Name(openSSLHandle);
	virtual~CSeptadaOpenSSLX509Name();
	bool GetValue(CRJStringByte&);
};

