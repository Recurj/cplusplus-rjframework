#include "stdafx.h"
#include <recurj/rjframework/libs/net/messages/netmessagesock.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionmessage.h>
using namespace rjNet;
RJ_ALLOCATOR(CRJMessageSocket, Allocator::Region::Message, AllocatorMessage::NetSocket) CRJMessageSocket::CRJMessageSocket(size_t lMsg, CRJSocket& clSock) :
	CRJMessage(eRJMessageRegions::Net, lMsg), m_clSock(clSock.SocketHolder()) {
	m_clSock.SocketTake(clSock);
}
CRJMessageSocket::~CRJMessageSocket() {}
void CRJMessageSocket::CloseSock() {
	m_clSock.SocketClose();
}

