#include "stdafx.h"
#include <recurj/rjframework/libs/net/tasks/nettasklistener.h>
#include <recurj/rjframework/libs/net/messages/netmessagesock.h>
using namespace rjNet;
CRJStringByteVar& CRJNetTaskListener::GetName(CRJStringByteVar& s) {
	return s.SetLabel("NetListener", 11);
}
CRJNetTaskListener::CRJNetTaskListener():m_clSock(SocketHolder())
{}
CRJNetTaskListener::~CRJNetTaskListener() {}
void CRJNetTaskListener::OnNetEventConnect() {

}
void CRJNetTaskListener::OnNetEventAccept() {
	CRJNetSocketTCPAccepted clSock(*this);
	if (m_clSock.SocketTCPAccept(clSock,true)) TaskMsgNewAndSend<CRJMessageSocket, int64_t, CRJSocket&>(GetAcceptor(), eRJMessagesRegionNet::Accepted, clSock);
}
void CRJNetTaskListener::OnNetEventRead() {}
void CRJNetTaskListener::OnNetEventClose() {}
void CRJNetTaskListener::OnNetEventSend() {}
void CRJNetTaskListener::OnNetEventError(int64_t lErr, CRJNet::eSocketActions lAction) {
	m_clSock.OnNetEventError(*this,lErr, lAction);
}
bool CRJNetTaskListener::TaskRunReady() {
	m_clListener.Reset();
	m_clSock.SocketTCPAlloc().SocketSetOptionBlockMode(false);
	return CRJNetTask::TaskRunReady() &&
		m_clSock.SocketEventPrepareListen() &&
		m_clSock.GetEvent().HandleIsReady() &&
		m_clListener.Insert(m_clSock.GetEvent(), (RJHandler)&CRJNetTaskListener::onSocket);
}
void CRJNetTaskListener::TaskRunFailed() {
	CRJLoggerThread::ErrorSize("Couldn't start listener. Is TCP/IP port free?", m_clSource.m_sPort, cLoggerPartTask);
	CRJNetTask::TaskRunFailed();
}
void CRJNetTaskListener::TaskRunBody() {
	if (!PrepareAcceptor()) CRJLoggerThread::ErrorSize("Couldn't start task for accepted sockets", m_clSource.m_sPort, cLoggerPartTask);
	else {
		CRJLoggerThread::Size(cLoggerPartTask, cLoggerLevelAbsolute,"Listen port", m_clSource.m_sPort);
		CRJNetTask::TaskRunBody();
	}
}
void CRJNetTaskListener::TaskQueueReady()
{
	if (m_clSock.SocketIsValid()) listen();
	CRJNetTask::TaskQueueReady();
}

void CRJNetTaskListener::TaskRunDone(bool bSeparated) {
	ReleaseAcceptor();
	m_clSock.SocketRelease();
	m_clListener.Reset();
	CRJNetTask::TaskRunDone(bSeparated);
}
bool CRJNetTaskListener::onSocket(size_t) {
	m_clSock.HappenedEvents(*this,0);
	return true;
}
void CRJNetTaskListener::listen()
{
	if (m_clSock.SocketBind(m_clSource.m_sPort, m_clSource.m_clHost)) {
		m_clSock.SocketTCPListen();
	}
}
void CRJNetTaskListener::errorNoAcceptor() {
	CRJLoggerThread::ErrorSize("Couldn't create task for accepted sockets", m_clSource.m_sPort, cLoggerPartTask);
}