#pragma once
#include <pod/data/septadapodata.h>
#include <cdata/security/septadaopensslhandle.h>
class SEPTADASECURITY_API CSeptadaOpenSSLX509Extension:public CRJSecurityHandle {
	friend class CSeptadaOpenSSLConnection;
public:
	CSeptadaOpenSSLX509Extension();
	virtual~CSeptadaOpenSSLX509Extension();
	bool ValidateFQDN(CRJStringByte&);
};

