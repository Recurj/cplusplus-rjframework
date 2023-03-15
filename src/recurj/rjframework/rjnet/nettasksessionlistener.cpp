#include "stdafx.h"
#include <recurj/rjframework/libs/net/tasks/nettasksessionlistener.h>
#include <recurj/rjframework/libs/net/messages/netmessagesession.h>
#include <recurj/rjframework/libs/base/application/application.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionnet.h>
using namespace rjNet;
RJ_ALLOCATOR(CRJNetTaskSessionListener::CTaskNetPool, Allocator::Region::Net, AllocatorNet::TaskPool) CRJNetTaskSessionListener::CTaskNetPool::CTaskNetPool(CRJNetTaskSessionListener& clTaskParent, size_t ind):
	CRJNetTaskPool(clTaskParent, ind) {}
CRJNetTaskSessionListener::CTaskNetPool::~CTaskNetPool() {}
CRJMessage* CRJNetTaskSessionListener::CTaskNetPool::OnTaskQueueMsg(CRJMessage& clMsg) {
	switch (clMsg.GetRegion()) {
	case eRJMessageRegions::Net: return onMsgRegionNet(clMsg);
	}
	return CRJNetTaskPool::OnTaskQueueMsg(clMsg);
}
CRJMessage* CRJNetTaskSessionListener::CTaskNetPool::onMsgRegionNet(CRJMessage& clMsg) {
	switch (clMsg.GetCommand()) {
	case eRJMessagesRegionNet::SessionFind: return OnFindSession((CRJMessageNetSession&) clMsg);
	case eRJMessagesRegionNet::SessionSend: return OnSendSession((CRJMessageNetSession&) clMsg);
	}
	return CRJNetTaskPool::OnTaskQueueMsg(clMsg);
}
CRJMessage* CRJNetTaskSessionListener::CTaskNetPool::OnFindSession(CRJMessageNetSession& clMsg) {
	CRJNetSocketEventSessionPooled* pElem;
	for (size_t i=0; i<m_sAllocated; i++) {
		pElem=GetCheckedIndex(i);
		if (clMsg.m_stTicket.m_pTaskSource==nullptr||
			pElem->m_stMetric>clMsg.m_stMetric) {
			clMsg.m_stMetric=pElem->m_stMetric;
			pElem->BuildTicket(clMsg.m_stTicket);
		}
	};
	try {
		if (m_pTaskNext) return m_pTaskNext->SendNormal(clMsg);
	}
	catch(...) {}
	if (clMsg.m_stTicket.m_pTaskSource) {
		if (clMsg.m_stTicket.m_pTaskSource==this) return OnSendSession(clMsg);
		else return clMsg.m_stTicket.m_pTaskSource->SendNormal(clMsg.SetCommand(eRJMessagesRegionNet::SessionSend));
	}
	return & clMsg;
}
CRJMessage* CRJNetTaskSessionListener::CTaskNetPool::OnSendSession(CRJMessageNetSession& clMsg) {
	if (IsValidTaskTicket(clMsg.m_stTicket)) {
		CRJNetSocketEventSessionPooled* pElem=GetCheckedIndex(clMsg.m_stTicket.m_sIndex);
		if (!pElem->Transmit(clMsg.m_clSock, clMsg.m_clParam)) {
			if (clMsg.NextTry()) return SendNormal(clMsg);
		}
	}
	return & clMsg;
}
CRJNetTaskSessionListener::CRJNetTaskSessionListener() {
	m_pTaskPool=nullptr;
}
CRJTask* CRJNetTaskSessionListener::PoolCreateTask(size_t ind) {
	return new CTaskNetPool(*this, ind);
}
void CRJNetTaskSessionListener::PoolFreeTask(CRJTask* p) {
	FreeTaskNetPool((CTaskNetPool*) p);
}
CRJSocketTCPPool* CRJNetTaskSessionListener::PoolElem(size_t) {
	return new CRJNetSocketEventSessionPooled(*this);
}
void CRJNetTaskSessionListener::FreeTaskNetPool(CTaskNetPool* p) {
	if (!p) return ;
	try {
		p->TaskStop();
		delete p;
	}
	catch(...) {}
}
void CRJNetTaskSessionListener::ReleaseAcceptor() {
	FreeTaskNetPool(m_pTaskPool);
}
bool CRJNetTaskSessionListener::Process(CRJSocket& clSock,const CRJStringByte& s) {
	return TaskMsgNewAndSend<CRJMessageNetSession, CRJSocket&,const CRJStringByte&>(GetAcceptor(), clSock, s);
}
CRJTask& CRJNetTaskSessionListener::GetAcceptor() {
	return (m_pTaskPool)?
		* m_pTaskPool:CRJTaskDummy::GetTaskDummy();
}
bool CRJNetTaskSessionListener::PrepareAcceptor() {
	m_pTaskPool=(CTaskNetPool*) PoolCreateTask(1);
	if (!m_pTaskPool) errorNoAcceptor();
	else return m_pTaskPool->TaskStart();
	return false;
}

