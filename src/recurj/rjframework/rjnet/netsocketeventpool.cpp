#include "stdafx.h"
#include <recurj/rjframework/libs/net/netsocketeventpool.h>
#include <recurj/rjframework/libs/net/tasks/nettaskpool.h>
using namespace rjNet;
CRJSocketTCPPool::CRJSocketTCPPool(IRJSocketHolder& holder):
	CRJSocketTCPEvent(holder),
	m_pPool(nullptr),
m_sIndex(0),
m_sLifeSock(0) {}
CRJSocketTCPPool::~CRJSocketTCPPool() {}
void CRJSocketTCPPool::OnNetEventConnect()
{}
void CRJSocketTCPPool::OnNetEventAccept()
{}
void CRJSocketTCPPool::OnNetEventError(int64_t lErr, CRJNet::eSocketActions action)
{
	CRJSocketTCPEvent::OnNetEventError(*this, lErr, action);
}

void CRJSocketTCPPool::OnNetEventClose() {
	IncSockLifeId();
	m_pPool->Remove(m_sIndex, GetEvent());
	SocketRelease();
}
void CRJSocketTCPPool::PoolRelease() {
	m_pPool->Remove(m_sIndex, GetEvent());
	SocketRelease();
}
void CRJSocketTCPPool::PoolBind(size_t ind, CRJNetTaskPool& clPool) {
	m_pPool=& clPool;
	m_sIndex=ind;
}
bool CRJSocketTCPPool::PoolReady() {
	return IsConnectedAndReady()&&m_pPool->Insert(m_sIndex, GetEvent());
}
bool CRJSocketTCPPool::PoolAdd(CRJSocket& sock) {
	SocketTake(sock);
	if ((PoolStart())&&(m_pPool->Insert(m_sIndex, GetEvent()))) {
		OnNetEventConnect();
		return true;
	}
	return false;
}
bool CRJSocketTCPPool::PoolStart() {
	SocketEventPrepareAccepted();
	return GetEvent().HandleIsReady();
}
SRJTaskTicket& CRJSocketTCPPool::BuildTicket(SRJTaskTicket& stTicket) {
	stTicket.m_sIndex=m_sIndex;
	stTicket.m_sLifeSeq=GetSockLifeId();
	stTicket.m_pTaskSource=m_pPool;
	return stTicket;
}
void CRJSocketTCPPool::SocketRedirected() {
	m_pPool->Remove(m_sIndex, GetEvent());
}

