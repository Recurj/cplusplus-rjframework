#include "stdafx.h"
#include <cdata/data/dtime/septadadtime.h>
#include <cdata/security/septadaopensslclient.h>
#include <cdata/security/septadaopenssl.h>
CSeptadaOpenSSLClient::CSeptadaOpenSSLClient(CSeptadaOpenSSLFactoryClient& clFactory,bool bSSL):
	CSeptadaOpenSSLConnection(clFactory) {
	m_clSockEvent.EventCreate();
	if (bSSL) SSLConnectionClientAlloc();
}
CSeptadaOpenSSLClient::~CSeptadaOpenSSLClient() {
	doReset();
}
bool CSeptadaOpenSSLClient::DoClientConnect(const CRJStringByte& clHost, int32_t lPort, int32_t lDelay, int32_t lRetry, int32_t lWaitConnect,bool bSSL) {
	int32_t lWarning;
	struct sockaddr_in stSockInfo;
	if (!SSLConnectionReset()) {
		CRJLoggerThread::Message(sLoggerPartSecurity, LOGGER_LEVEL_TRACE_LOW,"Error::SSL::Could not init session");
		return false;
	}
	doReset();
	m_stSockSendMetrics.dwWait=(uint32_t)(lDelay/ 2);
	m_stSockSendMetrics.lRetry=lRetry;
	CRJLoggerThread::Text(sLoggerPartSecurity, LOGGER_LEVEL_TRACE_LOW,"SSL::Try connect to", clHost);
	if (!m_clSocket.AtomSocketAlloc()) {
		CRJLoggerThread::Message(sLoggerPartSecurity, LOGGER_LEVEL_TRACE_LOW,"Error::SSL::Could not allocate socket");
		return false;
	}
	lWarning=m_clSocket.AtomSocketSetOptionKeepAlive(true);
	if (lWarning) APPLOGGER.PutTaskOwnLogNumber(sLoggerPartSecurity, LOGGER_LEVEL_TRACE_LOW,"SSL::SetOptionKeepAlive", lWarning);
	lWarning=m_clSocket.AtomSocketSetOptionNonBlockMode(false);
	if (lWarning) APPLOGGER.PutTaskOwnLogNumber(sLoggerPartSecurity, LOGGER_LEVEL_TRACE_LOW,"SSL::SetOptionNonBlockMode", lWarning);
	if (!m_clSocket.AtomSocketFindRemoteHost(lPort, clHost, stSockInfo)) {
		APPLOGGER.PutTaskOwnLogNumber(sLoggerPartSecurity, LOGGER_LEVEL_TRACE_LOW,"Error::SSL::FindRemoteHost", m_clSocket.AtomSocketGetLastError());
		return false;
	};
	if (!m_clSocket.AtomSocketSetEvent(m_clSockEvent, FD_READ| FD_CLOSE| FD_CONNECT)) {
		APPLOGGER.PutTaskOwnLogNumber(sLoggerPartSecurity, LOGGER_LEVEL_TRACE_LOW,"Error::SSL::SocketSetEvent", m_clSocket.AtomSocketGetLastError());
		return false;
	}
	return doReConnect(stSockInfo, lWaitConnect)&&
		(!bSSL||
		doHandshake(lWaitConnect));
}
void CSeptadaOpenSSLClient::SSLConnectionClientAlloc() {
	try {
		if (SSLHandleAttach(SSL_new((SSL_CTX*) m_clFactory.SSLHandleGet()))) {
			THIS_SSL->pUserData=this;
			SSL_set_connect_state(THIS_SSL);
			SSLConnectionSetBio();
		}
		else SSLHandleError();
	}
	catch(...) {}
}
bool CSeptadaOpenSSLClient::SSLConnectionClientConnect() {
	try {
		if (SSLHandleIsValid()) {
			if (SSL_do_handshake(THIS_SSL)<0) SSLHandleError();
			return SSLConnectionCheckNetOutData();
		}
	}
	catch(...) {}
	return false;
}
bool CSeptadaOpenSSLClient::DoSendNetworkData(const CRJValue& st) {
	CRJValue stWork=st;
	return m_clSocket.AtomSocketSend(stWork, m_stSockSendMetrics);
}
bool CSeptadaOpenSSLClient::SSLConnectionDoHandshake() {
	switch (SSL_get_error(THIS_SSL, SSL_connect(THIS_SSL))) {
	case SSL_ERROR_NONE: return SSLConnectionHandshakeDone();
	case SSL_ERROR_WANT_WRITE:
	case SSL_ERROR_WANT_READ: return SSLConnectionCheckNetOutData();
	}
	return SSLHandleError();
}
void CSeptadaOpenSSLClient::SSLConnectionClose() {
	CSeptadaOpenSSLConnection::SSLConnectionClose();
	m_clSocket.AtomSocketResetEvent(m_clSockEvent);
	m_clSocket.AtomSocketRelease();
}
bool CSeptadaOpenSSLClient::SSLClientRead(int32_t v, int32_t lHeader, std::function<bool(const CRJValue&)>clFunc) {
	bool rc;
	SSeptadaCommReadRecord stRecv;
	time_t b=CSeptadaDTime::GetDTime();
	time_t t=b;
	int32_t d=(v>>10)& 0x7FFFFFFF;
	if (d<=1) d=2;
	b+=d;
	while (t<b) {
		if (m_clDataAppl.CommCollectorCheckIncoming(lHeader, stRecv)) {
			rc=clFunc(stRecv.stData);
			m_clDataAppl.StripUsedData(stRecv.lDataTotalLen);
			return rc;
		}
		if (!doWaitEventRead(v)) return false;
		if (m_clSocket.AtomSocketRead(m_clSockRaw,true)) {
			if (!SSLConnectionRecv(m_clSockRaw.GetDataBlock(stRecv.stData))) return false;
			m_clSockRaw.ResetData();
		}
		t=CSeptadaDTime::GetDTime();
	}
	return false;
}
bool CSeptadaOpenSSLClient::SSLClientReadBinary(int32_t v, int32_t lLen, std::function<bool(const CRJValue&)>clFunc) {
	bool rc;
	CRJValue st;
	time_t b=CSeptadaDTime::GetDTime();
	time_t t=b;
	if (v<=1) v=2;
	b+=v;
	v=((v<<10)& 0x7FFFFFFF);
	while (t<b) {
		if (m_clDataAppl.GetDataLen()>=lLen) {
			m_clDataAppl.GetDataBlock(st).lLen=lLen;
			rc=clFunc(st);
			m_clDataAppl.StripUsedData(lLen);
			return rc;
		}
		if (!doWaitEventRead(v)) return false;
		if (m_clSocket.AtomSocketRead(m_clSockRaw,true)) {
			if (!SSLConnectionRecv(m_clSockRaw.GetDataBlock(st))) return false;
			m_clSockRaw.ResetData();
		}
		t=CSeptadaDTime::GetDTime();
	}
	return false;
}
void CSeptadaOpenSSLClient::doReset() {
	m_clSocket.AtomSocketResetEvent(m_clSockEvent);
	m_clSocket.AtomSocketRelease();
	m_clSockRaw.ResetData();
}
bool CSeptadaOpenSSLClient::doHandshake(int32_t t) {
	CRJValue st;
	SSLConnectionClientConnect();
	while (doWaitEventRead(t)) {
		if (!m_clSocket.AtomSocketRead(m_clSockRaw,false)) return onSockErrorNoRead(m_clSocket.AtomSocketGetLastError());
		if (!SSLConnectionRecv(m_clSockRaw.GetDataBlock(st))) return false;
		m_clSockRaw.ResetData();
		if (SSL_is_init_finished(THIS_SSL)) return true;
	}
	return false;
}
bool CSeptadaOpenSSLClient::doReConnect(
struct sockaddr_in& stSockDest, int32_t lWaitConnect) {
	try {
		if (!m_clSocket.AtomSocketConnect(stSockDest,sizeof(stSockDest))) {
			if (m_clSocket.AtomSocketGetLastError()==WSAEWOULDBLOCK) return doWaitEventConnect(lWaitConnect);
		}
		else return true;
	}
	catch(...) {}
	return onSockErrorNoConnect(m_clSocket.AtomSocketGetLastError());
}
bool CSeptadaOpenSSLClient::doWaitEventConnect(int32_t t) {
	try {
		HANDLE htab[2];
		htab[0]=m_clFactory.GetTask().GetTaskBreakEvent().HandleGetChecked();
		htab[1]=m_clSockEvent.HandleGetChecked();
		if (WaitForMultipleObjects(2, htab,false,(uint32_t) t)==(WAIT_OBJECT_0+1)) return doCheckSockEventConnect();
	}
	catch(...) {}
	return false;
}
bool CSeptadaOpenSSLClient::doWaitEventRead(int32_t t) {
	try {
		HANDLE htab[2];
		htab[0]=m_clFactory.GetTask().GetTaskBreakEvent().HandleGetChecked();
		htab[1]=m_clSockEvent.HandleGetChecked();
		if (WaitForMultipleObjects(2, htab,false,(uint32_t) t)==(WAIT_OBJECT_0+1)) return doCheckSockEventRead();
	}
	catch(...) {}
	return false;
}
bool CSeptadaOpenSSLClient::doCheckSockEventRead() {
	WSANETWORKEVENTS nEvents;
	if (m_clSocket.AtomSocketWaitEvents(nEvents, m_clSockEvent)) {
		if (nEvents.lNetworkEvents& FD_READ) {
			if (nEvents.iErrorCode[FD_READ_BIT]) return onSockErrorNoRead(nEvents.iErrorCode[FD_READ_BIT]);
			return true;
		}
		if (nEvents.lNetworkEvents& FD_CLOSE) return false;
		else return true;
	}
	else onSockErrorNoEvent(m_clSocket.AtomSocketGetLastError());
	return false;
}
bool CSeptadaOpenSSLClient::doCheckSockEventConnect() {
	WSANETWORKEVENTS nEvents;
	if (m_clSocket.AtomSocketWaitEvents(nEvents, m_clSockEvent)) {
		if (nEvents.lNetworkEvents& FD_CONNECT) {
			if (nEvents.iErrorCode[FD_CONNECT_BIT]) return onSockErrorNoConnect(nEvents.iErrorCode[FD_CONNECT_BIT]);
			else return true;
		}
		if (nEvents.lNetworkEvents& FD_READ) {
			if (nEvents.iErrorCode[FD_READ_BIT]) return onSockErrorNoRead(nEvents.iErrorCode[FD_READ_BIT]);
			return true;
		}
	}
	else onSockErrorNoEvent(m_clSocket.AtomSocketGetLastError());
	return false;
}
bool CSeptadaOpenSSLClient::onSockErrorNoEvent(int32_t lErr) {
	APPLOGGER.PutTaskOwnLogNumber(sLoggerPartSecurity, LOGGER_LEVEL_TRACE_LOW,"Error::SSL::Raw get event", lErr);
	return false;
}
bool CSeptadaOpenSSLClient::onSockErrorNoRead(int32_t lErr) {
	APPLOGGER.PutTaskOwnLogNumber(sLoggerPartSecurity, LOGGER_LEVEL_TRACE_LOW,"Error::SSL::Raw read", lErr);
	return false;
}
bool CSeptadaOpenSSLClient::onSockErrorNoConnect(int32_t lErr) {
	APPLOGGER.PutTaskOwnLogNumber(sLoggerPartSecurity, LOGGER_LEVEL_TRACE_LOW,"Error::SSL::Raw connect", lErr);
	return false;
}

