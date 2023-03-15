#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/security/openssl.h>
#include <recurj/rjframework/libs/security/opensslconnection.h>
#include <recurj/rjframework/libs/security/opensslfactory.h>
#include <recurj/rjframework/libs/security/opensslx509.h>
#include <recurj/rjframework/libs/security/opensslapplication.h>

#define THIS_SSL ((SSL *)m_hSSLHandle)
using namespace rjSecurity;
CRJSecurityConnectionApplData::CRJSecurityConnectionApplData(size_t s) {
	if (s>0) {
		m_pBuffer=(uint8_t*) CRJSecurityApplication::m_clHeap.Alloc(s);
		if (m_pBuffer) SetBuffer(m_pBuffer, s);
	}
	else m_pBuffer=nullptr;
}
CRJSecurityConnectionApplData::~CRJSecurityConnectionApplData() {
	CRJSecurityApplication::m_clHeap.Free(m_pBuffer);
}
CRJStringByte CRJSecurityConnection::s_clSSLConnectionErrorTooMuchData("ERROR::TOO MANY DATA");
CRJStringByte CRJSecurityConnection::s_clSSLConnectionErrorNoCertificate("ERROR::CLIENT CERTIFICATE IS ABSENT");
CRJSecurityConnection::CRJSecurityConnection(size_t s):
	m_clApplData(s) {}
CRJSecurityConnection::~CRJSecurityConnection() {}
void CRJSecurityConnection::SSLHandleFree() {
	SSLConnectionReset();
	try {
		if (SSLHandleIsValid()) {
			SSL_free(THIS_SSL);
			m_clRead.CRJSecurityHandle::SSLHandleFree();
			m_clSend.CRJSecurityHandle::SSLHandleFree();
		}
	}
	catch(...) {}
	CRJSecurityHandle::SSLHandleFree();
}
bool CRJSecurityConnection::SSLConnectionReset() {
	try {
		SSLConnectionClose();
		m_clApplData.ResetCollector();
		if (SSLHandleIsValid()) {
			if (SSL_clear(THIS_SSL)>0) {
				m_clRead.SSLBIOReset();
				m_clSend.SSLBIOReset();
				return true;
			}
			else return SSLHandleError();
		}
	}
	catch(...) {}
	return false;
}
bool CRJSecurityConnection::SSLConnectionClose() {
	try {
		if (SSLHandleIsValid()) {
			if (!(SSL_get_shutdown(THIS_SSL)& SSL_SENT_SHUTDOWN)) SSL_shutdown(THIS_SSL);
			return SSLConnectionCheckNetData();
		}
	}
	catch(...) {}
	return false;
}
bool CRJSecurityConnection::SSLConnectionRecv(CRJStringByteFlex& stRecv) {
	try {
		if (m_clRead.SSLBIOAddDataSafed(stRecv)) {
			if (SSL_is_init_finished(THIS_SSL)) return SSLConnectionCheckApplData();
			return SSLConnectionHandshake();
		}
		else return SSLHandleError();
	}
	catch(...) {}
	return false;
}
bool CRJSecurityConnection::SSLConnectionSend(CRJStringByteFlex& stSend) {
	try {
		if (SSL_write(THIS_SSL, stSend.GetHeadForRead(), (int32_t)stSend.GetLength())>0) return SSLConnectionCheckNetData();
		else return SSLHandleError();
	}
	catch(...) {}
	return false;
}
bool CRJSecurityConnection::SSLConnectionInited() {
	try {
		return (SSL_is_init_finished(THIS_SSL))?
			true:false;
	}
	catch(...) {}
	return false;
}
bool CRJSecurityConnection::SSLConnectionHandshake() {
	switch (SSL_get_error(THIS_SSL, SSLHandshakeStart())) {
	case SSL_ERROR_NONE: return OnSSLHandshakeDone();
	case SSL_ERROR_WANT_WRITE:
	case SSL_ERROR_WANT_READ: return SSLConnectionCheckNetData();
	}
	return SSLHandleError();
}
bool CRJSecurityConnection::OnSSLHandshakeDone() {
	if (SSLConnectionReady()) return SSLConnectionCheckApplData();
	return false;
}
bool CRJSecurityConnection::SSLConnectionReady() {
	return true;
}
bool CRJSecurityConnection::SSLConnectionCheckNetData() {
	CRJStringByteFlex stSend;
	if (m_clSend.SSLBIOHasDataSafed(stSend)) {
		bool rc=OnSSLDataNetwork(stSend);
		m_clSend.SSLBIOReset();
		return rc;
	}
	return true;
}
bool CRJSecurityConnection::SSLConnectionCheckApplData() {
	CRJStringByteFlex stData;
	bool rc=false;
	SSLConnectionCheckNetData();
	while (SSLConnectionRead(rc)) {
		OnSSLDataApplication(stData.Set(m_clApplData.GetStorage()));
		m_clApplData.Processed(stData.GetLength()).Normalize();
	}
	return rc;
}
bool CRJSecurityConnection::SSLConnectionRead(bool& bOK) {
	CRJStringByteVar& storage=m_clApplData.GetStorage();
	int32_t lRead, lVolume=(int32_t) storage.GetCapacity();
	uint8_t* bData=const_cast<uint8_t*>(storage.GetTail());
	bOK=true;
	if (!lVolume) {
#ifdef _DEBUG
		CRJDataTools::DebugBreak();
#endif 
		OnSSLHandleError(CRJSecurityConnection::s_clSSLConnectionErrorTooMuchData);
		m_clApplData.ResetCollector();
		lVolume=(int32_t) storage.GetCapacity();
		bData=const_cast<uint8_t*>(storage.GetTail());
	};
	lRead=SSL_read(THIS_SSL, bData, lVolume);
	if (lRead>0) {
		storage.ExpandedOutside((size_t) lRead);
		return true;
	}
	bOK=SSLHandleError();
	return false;
}
bool CRJSecurityConnection::SSLHandleError() {
	if (!CRJSecurityHandle::SSLHandleError()) return false;
	return SSLConnectionCheckNetData();
}
void CRJSecurityConnection::SSLClearThreadErrors() {
	ERR_clear_error();
}
CRJSecurityConnectionClient::CRJSecurityConnectionClient(size_t s):
	CRJSecurityConnection(s) {
	SSLConnectionAlloc();
}
void CRJSecurityConnectionClient::SSLConnectionAlloc() {
	try {
		if (m_clApplData.IsReady()) {
			if (m_clCTX.SSLHandleAttach(SSL_CTX_new(TLS_client_method()))&&
				SSLHandleAttach(SSL_new((SSL_CTX*) m_clCTX.m_hSSLHandle))) {
				SSL_set_bio(THIS_SSL,(BIO*) m_clRead.m_hSSLHandle,(BIO*) m_clSend.m_hSSLHandle);
				SSL_set_connect_state(THIS_SSL);
				return ;
			}
		}
	}
	catch(...) {}
	SSLHandleError();
	SSLHandleFree();
}
CRJSecurityConnectionClient::~CRJSecurityConnectionClient() {
	SSLHandleFree();
	m_clCTX.SSLHandleFree();
}
int CRJSecurityConnectionClient::SSLHandshakeStart() {
	return SSL_do_handshake(THIS_SSL);
}
CRJSecurityConnectionServer::CRJSecurityConnectionServer(CRJSecurityFactory& clFactory, size_t s):
	CRJSecurityConnection(s),
m_clFactory(clFactory) {
	SSLConnectionAlloc();
}
CRJSecurityConnectionServer::~CRJSecurityConnectionServer() {
	SSLHandleFree();
}
int CRJSecurityConnectionServer::SSLHandshakeStart() {
	return SSL_accept(THIS_SSL);
}
bool CRJSecurityConnectionServer::OnSSLHandshakeDone() {
	return SSLConnectionPostCheck()&&
		CRJSecurityConnection::OnSSLHandshakeDone();
}
void CRJSecurityConnectionServer::SSLConnectionAlloc() {
	try {
		if (m_clApplData.IsReady()) {
			if (SSLHandleAttach(SSL_new((SSL_CTX*) m_clFactory.m_hSSLHandle))) {
				SSL_set_bio(THIS_SSL,(BIO*) m_clRead.m_hSSLHandle,(BIO*) m_clSend.m_hSSLHandle);
				SSL_set_hostflags(THIS_SSL, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
				SSL_set_verify(THIS_SSL, SSL_VERIFY_PEER, NULL);
				if (m_clFactory.SSLFactoryVerifyMustBe()) {
					m_clFactory.m_clFQDN.MakeCLabel()&&
						SSL_set1_host(THIS_SSL,(const char*) m_clFactory.m_clFQDN.GetHeadForRead());
				}
				SSL_set_accept_state(THIS_SSL);
				return ;
			}
		}
	}
	catch(...) {}
	SSLHandleError();
	SSLHandleFree();
}
bool CRJSecurityConnectionServer::SSLConnectionPostCheck() {
	CRJSecurityX509 clCert(SSL_get_peer_certificate(THIS_SSL));
	if (clCert.SSLHandleIsValid()) return true;
	if (m_clFactory.SSLFactoryVerifyMustBe()) {
		OnSSLHandleError(CRJSecurityConnection::s_clSSLConnectionErrorNoCertificate);
		return false;
	}
	return true;
}

