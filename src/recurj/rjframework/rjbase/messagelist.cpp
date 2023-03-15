#include "stdafx.h"
#include <recurj/rjframework/libs/base/messages/list.h>
#include <recurj/rjframework/libs/base/messages/queue.h>
using namespace rjBase;

CRJListMessages::CRJListMessages() noexcept {
	Reset();
}
CRJListMessages::~CRJListMessages() {
	Clear();
}
bool CRJListMessages::QueueBot(CRJQueueMessages& q)
{
	return q.QueueBot(*this);
}

void CRJListMessages::freeElem(CRJMessage* p) {
	CRJMessage::freeMessage(p);
}
