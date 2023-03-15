#include "stdafx.h"
#include <recurj/rjframework/libs/net/tasks/nettaskconnector.h>
using namespace rjNet;
CRJStringByteVar& CRJNetTaskConnector::GetName(CRJStringByteVar& s) {
	return s.SetLabel("NetConnector", 12);
}
CRJNetTaskConnector::CRJNetTaskConnector() :
	m_clSock(SocketHolder()),m_sReconnect(cUNetTaskConnectorReconnect),m_bKeepAlive(true), m_bStopped(false){}
CRJNetTaskConnector::~CRJNetTaskConnector() {}
bool CRJNetTaskConnector::TaskRunReady() {
	m_clListener.Reset();
	return CRJNetTask::TaskRunReady() && sockPrepare();
}
void CRJNetTaskConnector::TaskRunFailed() {
	CRJLoggerThread::ErrorSize("Couldn't start connector", m_clDestination.m_sPort, cLoggerPartTask);
	CRJNetTask::TaskRunFailed();
}
void CRJNetTaskConnector::TaskRunDone(bool bSeparated) {
	m_clListener.Reset();
	sockClose();
	CRJNetTask::TaskRunDone(bSeparated);
}
void CRJNetTaskConnector::TaskQueueReady()
{
	if (m_clSock.SocketIsValid()) sockConnect();
	CRJNetTask::TaskQueueReady();
}

void CRJNetTaskConnector::OnEventHandlerTimeout() {
	if (m_clSock.SocketIsConnected()) Ping();
	else if (m_clSock.SocketIsValid()) sockConnect();
	else if (!m_bStopped) sockRestart();
}
void CRJNetTaskConnector::OnNetEventConnect() {
	OnConnect();
}
void CRJNetTaskConnector::OnNetEventAccept() {
}
void CRJNetTaskConnector::OnNetEventRead() {
	OnReceive();
	if (!m_clSock.SocketIsConnected()) OnNetEventClose();
}
void CRJNetTaskConnector::OnNetEventClose() {
	OnDisconnect();
	sockRestart();
}
void CRJNetTaskConnector::OnNetEventSend() {
	m_clSock.OnNetEventSend();
}
void CRJNetTaskConnector::OnNetEventError(int64_t lErr, CRJNet::eSocketActions lAction) {
	if (lAction== CRJNet::eSocketActions::CONNECT) OnNoconnect();
	else m_clSock.OnNetEventError(*this, lErr, lAction);
}
void CRJNetTaskConnector::OnSocketError(int64_t lErr, size_t lAction)
{
	if (lAction == CRJNet::eSocketActions::CONNECT) {
		if (lErr != EX_SOCKET_ERROR_BLOCK) {
			onConnectError(lErr);
		}
	}
}
void CRJNetTaskConnector::OnSocketWarning(int64_t, size_t) {

}
void CRJNetTaskConnector::onConnectError(int64_t) {
	Stop();
}
void CRJNetTaskConnector::Stop() {
	m_clListener.Remove(m_clSock.GetEvent());
	m_bStopped = true;
	sockClose();
}
void CRJNetTaskConnector::Pause(RJEvent& h, CRJSocket& sock) {
	m_clListener.Remove(m_clSock.GetEvent());
	sock.SocketTake(m_clSock);
	m_clSock.SocketRelease();
	m_clListener.Insert(h, (RJHandler)&CRJNetTaskConnector::onSocket);
}
void CRJNetTaskConnector::Play(RJEvent& h) {
	m_clListener.Remove(h);
	m_bStopped = false;
	sockRestart();
}

void CRJNetTaskConnector::Next(CRJSocket& sock) {
	m_clListener.Remove(m_clSock.GetEvent());
	sock.SocketTake(m_clSock);
	if (sockPrepare()) sockConnect();
}
void CRJNetTaskConnector::OnConnecting()
{}
void CRJNetTaskConnector::OnConnect() {
	m_clSock.SocketInfo(cLoggerLevelInfo, CRJSocket::s_clConnected);
}
void CRJNetTaskConnector::OnDisconnect() {
	m_clSock.SocketInfo(cLoggerLevelInfo, CRJSocket::s_clDisconnected);
}
bool CRJNetTaskConnector::onSocket(size_t) {
	if (m_clSock.SocketIsValid()) m_clSock.HappenedEvents(*this,0);
	else sockRestart();
	return true;
}
void CRJNetTaskConnector::logNetTryConnect() {
	CRJStringByteExpanded s;
	s.SetLabel("Try connect to host ").Cat(m_clDestination.m_clHost).CatSingle('[').CatInt64(m_clDestination.m_sPort).CatSingle(']');
	m_clSock.SocketInfo(cLoggerLevelInfo, s);
}

bool CRJNetTaskConnector::sockConnect() {
	if (m_clDestination.IsDefined()) {
		if (m_clSource.m_sPort > 0) m_clSock.SocketBind(m_clSource.m_sPort, m_clSource.m_clHost);
		logNetTryConnect();
		SetTaskQueueWait(m_sReconnect);
		OnConnecting();
		return m_clSock.SocketTCPConnect(m_clDestination.m_sPort, m_clDestination.m_clHost, m_bKeepAlive);
	}
	return false;
}
bool CRJNetTaskConnector::sockPrepare() {
	m_clSock.SocketTCPAlloc().SocketSetOptionBlockMode(false);
	return m_clSock.SocketEventPrepareConnect() && m_clSock.GetEvent().HandleIsReady() &&
		m_clListener.Insert(m_clSock.GetEvent(), (RJHandler)&CRJNetTaskConnector::onSocket);
}
void CRJNetTaskConnector::sockClose() {
	if (m_clSock.SocketIsConnected()) {
		OnDisconnect();
		m_clSock.SocketClose();
	}
	m_clSock.SocketRelease();
}
void CRJNetTaskConnector::sockRestart() {
	m_clListener.Remove(m_clSock.GetEvent());
	m_clSock.SocketRelease();
	if (sockPrepare()) {
		sockConnect();
	}
}