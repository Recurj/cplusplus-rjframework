#include "stdafx.h"
#include <recurj/rjframework/libs/net/netsocketeventaccepted.h>
using namespace rjNet;
CRJSocketTCPAccepted::CRJSocketTCPAccepted(IRJSocketHolder&holder):
	CRJSocketTCPEventSend(holder)
{}
CRJSocketTCPAccepted::CRJSocketTCPAccepted(CRJSocket& clSock) :
	CRJSocketTCPEventSend(clSock.SocketHolder())
{
	SocketTake(clSock);
}
CRJSocketTCPAccepted::~CRJSocketTCPAccepted() 
{}

