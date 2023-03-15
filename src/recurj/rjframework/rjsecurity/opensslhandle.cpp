#include "stdafx.h"
#include <recurj/rjframework/libs/security/opensslhandle.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/security/openssl.h>
#include <recurj/rjframework/libs/base/logger/loggerthread.h>
using namespace rjSecurity;
CRJStringByte CRJSecurityHandle::s_clSSLHandleWordError("OpenSSL ERROR");
CRJStringByte CRJSecurityHandle::s_clSSLHandleWordDNS("DNS");
CRJStringByte CRJSecurityHandle::s_clSSLHandleWordSubjectAltName("subjectAltName");
CRJSecurityHandle::CRJSecurityHandle() {
	m_hSSLHandle=nullptr;
}
CRJSecurityHandle::CRJSecurityHandle(openSSLHandle h) {
	m_hSSLHandle=h;
}
CRJSecurityHandle::~CRJSecurityHandle() {}
bool CRJSecurityHandle::SSLHandleAttach(openSSLHandle h) {
	m_hSSLHandle=h;
	return SSLHandleIsValid();
}
void CRJSecurityHandle::SSLHandleFree() {
	m_hSSLHandle=nullptr;
}
void CRJSecurityHandle::OnSSLHandleError(const CRJStringByte& s) {
	CRJLoggerThread::Message(cLoggerPartSecurity, cLoggerLevelTrace, s);
}
bool CRJSecurityHandle::SSLHandleError() {
	int32_t lErrCount=0;
	try {
		CRJStringByteDefault str;
		int32_t lCount=theSSLHandleMaxErrorInStep;
		uint32_t lErr=ERR_get_error();
		while ((lErr>0)&&
			(lCount>0)) {
			if ((lErr!=SSL_ERROR_WANT_READ)&&
				(lErr!=SSL_ERROR_WANT_WRITE)) {
				ERR_error_string_n(lErr,(char*) str.GetHeadForRead(),(size_t) str.GetSize()-1);
				str.CalcCLength();
				OnSSLHandleError(str);
				lCount--;
				lErrCount++;
			}
			lErr=ERR_get_error();
		}
	}
	catch(...) {}
	return lErrCount==0;
}

