#include "stdafx.h"
#include <cdata/data/dtime/septadadtime.h>
#include <cdata/security/septadaopensslserver.h>
#include <cdata/security/septadaopenssl.h>
#include <cdata/security/septadaopensslx509.h>
CSeptadaOpenSSLServer::CSeptadaOpenSSLServer(CSeptadaOpenSSLFactoryServer& clFactory):
	CSeptadaOpenSSLConnection(clFactory) {
	m_clSockEvent.EventCreate();
	SSLConnectionServerAlloc();
}
CSeptadaOpenSSLServer::~CSeptadaOpenSSLServer() {
	m_clSocket.AtomSocketResetEvent(m_clSockEvent);
}
void CSeptadaOpenSSLServer::SSLConnectionServerAlloc() {
	try {
		if (SSLHandleAttach(SSL_new((SSL_CTX*) m_clFactory.SSLHandleGet()))) {
			THIS_SSL->pUserData=this;
			SSL_set_accept_state(THIS_SSL);
			SSLConnectionSetBio();
		}
	}
	catch(...) {}
}
bool CSeptadaOpenSSLServer::SSLConnectionServerAccept() {
	try {
		if (SSLHandleIsValid()) {
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CSeptadaOpenSSLServer::DoSendNetworkData(const CRJValue& st) {
	CRJValue stWork=st;
	return m_clSocket.AtomSocketSend(stWork, m_stSockSendMetrics);
}
bool CSeptadaOpenSSLServer::SSLConnectionDoHandshake() {
	switch (SSL_get_error(THIS_SSL, SSL_accept(THIS_SSL))) {
	case SSL_ERROR_NONE: return SSLConnectionHandshakeDone();
	case SSL_ERROR_WANT_WRITE:
	case SSL_ERROR_WANT_READ: return SSLConnectionCheckNetOutData();
	}
	return SSLHandleError();
}
bool CSeptadaOpenSSLServer::SSLConnectionHandshakeDone() {
	if (SSLConnectionPostCheck()) return CSeptadaOpenSSLConnection::SSLConnectionHandshakeDone();
	return false;
}
void CSeptadaOpenSSLServer::SSLConnectionClose() {
	CSeptadaOpenSSLConnection::SSLConnectionClose();
	m_clSocket.AtomSocketResetEvent(m_clSockEvent);
	m_clSocket.AtomSocketRelease();
}
bool CSeptadaOpenSSLServer::SSLConnectionPostCheck() {
	CSeptadaOpenSSLX509 clCert(SSL_get_peer_certificate(THIS_SSL));
	if (clCert.SSLHandleIsValid()) return clCert.ValidateFQDNSafed(((CSeptadaOpenSSLFactoryServer&) m_clFactory).SSLFactoryGetFQDN());
	if (((CSeptadaOpenSSLFactoryServer&) m_clFactory).SSLFactoryVerifyMustBe()) {
		OnSSLHandleError(CSeptadaOpenSSLConnection::s_clSSLConnectionErrorNoCertificate);
		return false;
	}
	return true;
}

