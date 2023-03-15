#include "stdafx.h"
#include <recurj/rjframework/libs/net/tasks/nettaskpool.h>
CRJStringByteVar& CRJNetTaskPool::GetName(CRJStringByteVar& s) {
	return m_clManager.GetParentTask().GetName(s).CatSingle('\\').CatSize(m_sIndex);
}
CRJNetTaskPool::CRJNetTaskPool(IRJSocketPoolManager& clManager, size_t ind):
	CRJTaskQueuePool(),
m_pTaskNext(nullptr),
m_clManager(clManager),
m_sIndex(ind),
m_sAllocated(0) {
	CRJStringByteExpanded s;
	for (size_t i=0; i<CRJOSPollElemCount; i++) m_pElems[i]=nullptr;
}
CRJNetTaskPool::~CRJNetTaskPool() {
	m_clManager.PoolFreeTask(m_pTaskNext);
}
CRJMessage* CRJNetTaskPool::OnTaskQueueMsg(CRJMessage& clMsg) {
	switch (clMsg.GetRegion()) {
	case eRJMessageRegions::Net: return onMsgRegionNet(clMsg);
	}
	return CRJTaskQueuePool::OnTaskQueueMsg(clMsg);
}
CRJMessage* CRJNetTaskPool::onMsgRegionNet(CRJMessage& clMsg) {
	switch (clMsg.GetCommand()) {
	case eRJMessagesRegionNet::Accepted: return OnSockAccepted((CRJMessageSocket&) clMsg);
	}
	return CRJTaskQueuePool::OnTaskQueueMsg(clMsg);
}
bool CRJNetTaskPool::OnWaitEventElement(size_t sIndex) {
	sIndex-=2;
	if (sIndex < m_sAllocated) {
		auto sock = m_pElems[sIndex];
		sock->HappenedEvents(*sock,0);
	}
	return true;
}
void CRJNetTaskPool::OnEventHandlerError(int64_t err) {
	CRJTaskQueuePool::OnEventHandlerError(err);
	m_clListener.Reset();
	CRJTaskQueuePool::TaskQueueSetListener();
	repos();
}
void CRJNetTaskPool::repos() {
	size_t f=0;
	Reset();
	for (size_t i=0; i<m_sAllocated; i++) {
		if (m_pElems[i]->PoolReady()) {
			if (i==m_sFree) m_sFree=m_sMapFree[m_sFree];
			else m_sMapFree[f]=m_sMapFree[i];
		}
		else {
			m_pElems[i]->PoolRelease();
			f=i;
		}
	}
}
void CRJNetTaskPool::remove(size_t p) {
	if (Remove(p, m_pElems[p]->GetEvent())) m_pElems[p]->PoolRelease();
}
CRJMessage* CRJNetTaskPool::OnSockAccepted(CRJMessageSocket& clMsg) {
	if (HasSpace()) {
		if (m_sAllocated<CRJOSPollElemCount&&
			(m_sFree>=m_sAllocated)) {
			m_pElems[m_sFree]=m_clManager.PoolElem(m_sFree);
			if (m_pElems[m_sFree]==nullptr) return & clMsg;
			m_pElems[m_sFree]->PoolBind(m_sFree,*this);
			m_sAllocated++;
		}
		if (m_pElems[m_sFree]->PoolAdd(clMsg.GetSock())) m_sFree=m_sMapFree[m_sFree];
		else remove(m_sFree);
		return & clMsg;
	}
	return Propogate(clMsg);
}
CRJMessage* CRJNetTaskPool::Propogate(CRJMessageSocket& clMsg) {
	try {
		if (!m_pTaskNext) {
			m_pTaskNext=m_clManager.PoolCreateTask(m_sIndex+1);
			if (!m_pTaskNext) return & clMsg;
		}
		return m_pTaskNext->SendNormal(clMsg);
	}
	catch(...) {}
	return & clMsg;
}
bool CRJNetTaskPool::IsValidTaskTicket(const SRJTaskTicket& stPost) {
	return stPost.m_sIndex<m_sAllocated&&
		m_pElems[stPost.m_sIndex]->IsThatSockLife(stPost.m_sLifeSeq);
}

