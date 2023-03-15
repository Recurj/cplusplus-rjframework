#include "stdafx.h"
#include <recurj/rjframework/libs/net/tasks/nettaskaccepted.h>
using namespace rjNet;
CRJStringByteVar& CRJNetTaskAccepted::GetName(CRJStringByteVar& s) {
	return s.SetLabel("NetAcceptor", 11);
}
void CRJNetTaskAccepted::Take(CRJSocket& sock)
{
	m_clSock.SocketTake(sock);
}
CRJNetTaskAccepted::CRJNetTaskAccepted(const SRJSocketProcessMetrics& st) :
	m_clSock(SocketHolder()),m_sPause(cHandleWaitEndless) {
	m_clSock.GetNetSend().SetMetrics(st);
}
CRJNetTaskAccepted::CRJNetTaskAccepted(CRJSocket& clSock,const SRJSocketProcessMetrics& st):
	m_clSock(clSock),
m_sPause(cHandleWaitEndless) {
	m_clSock.GetNetSend().SetMetrics(st);
}
CRJNetTaskAccepted::~CRJNetTaskAccepted() {}
bool CRJNetTaskAccepted::TaskRunReady() {
	m_clListener.Reset();
	m_clSock.SocketSetOptionBlockMode(false);
	return CRJNetTask::TaskRunReady()&&
		m_clSock.SocketEventPrepareAccepted()&&
		m_clSock.GetEvent().HandleIsReady()&&
		m_clListener.Insert(m_clSock.GetEvent(),(RJHandler)& CRJNetTaskAccepted::onSocket);
}
void CRJNetTaskAccepted::TaskRunBody() {
	SetTaskQueueWait(m_sPause);
	CRJNetTask::TaskRunBody();
}
void CRJNetTaskAccepted::TaskRunDone(bool bSeparated) {
	m_clListener.Reset();
	m_clSock.SocketRelease();
	CRJNetTask::TaskRunDone(bSeparated);
}
void CRJNetTaskAccepted::OnEventHandlerTimeout() {
	if (m_clSock.SocketIsConnected()) Ping();
	else OnNetEventClose();
}
void CRJNetTaskAccepted::OnNetEventConnect() {

}
void CRJNetTaskAccepted::OnNetEventAccept() {

}
void CRJNetTaskAccepted::OnNetEventRead() {
	OnReceive();
	if (!m_clSock.SocketIsConnected()) OnNetEventClose();
}

void CRJNetTaskAccepted::OnNetEventClose() {
	Disconnected();
	TaskBreak();
}
void CRJNetTaskAccepted::OnNetEventSend() {
	m_clSock.OnNetEventSend();
}
void CRJNetTaskAccepted::OnNetEventError(int64_t lErr, CRJNet::eSocketActions lAction) {
	m_clSock.OnNetEventError(*this,lErr, lAction);
}
bool CRJNetTaskAccepted::onSocket(size_t) {
	m_clSock.HappenedEvents(*this, 0);
	return true;
}
