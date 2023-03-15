#include "stdafx.h"
#include <recurj/rjframework/libs/net/messages/netmessagesessiondone.h>
#include <recurj/rjframework/libs/base/messages/known.h>
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionmessage.h>
using namespace rjNet;
RJ_ALLOCATOR(CRJMessageNetSessionDone, Allocator::Region::Message, AllocatorMessage::NetSessionDone) CRJMessageNetSessionDone* CRJMessageNetSessionDone::Get(size_t id) {
	return new CRJMessageNetSessionDone(id);
}
CRJMessageNetSessionDone::CRJMessageNetSessionDone(size_t id):
	CRJMessage(eRJMessageRegions::Net, eRJMessagesRegionNet::SessionDone),
m_lId(id) {}
CRJMessageNetSessionDone::~CRJMessageNetSessionDone() {}

