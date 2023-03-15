#include "stdafx.h"
#include <recurj/rjframework/libs/net/messages/netmessagesession.h>
#include <recurj/rjframework/libs/base/messages/known.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionmessage.h>
using namespace rjNet;
RJ_ALLOCATOR(CRJMessageNetSession, Allocator::Region::Message, AllocatorMessage::NetSession) CRJMessageNetSession::CRJMessageNetSession(CRJSocket& clSock,const CRJStringByte& s):
	CRJMessage(eRJMessageRegions::Net, eRJMessagesRegionNet::SessionFind),
	m_clSock(clSock.SocketHolder()),m_clParam(s) {
	m_lTrySend=0;
	m_clSock.SocketTake(clSock);
	m_stTicket.m_pTaskSource=nullptr;
}
CRJMessageNetSession::~CRJMessageNetSession() {}
bool CRJMessageNetSession::NextTry() {
	m_lTrySend++;
	return m_lTrySend<=NetSessionTransmitMaxTry;
}

