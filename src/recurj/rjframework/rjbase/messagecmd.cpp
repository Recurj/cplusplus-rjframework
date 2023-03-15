#include "stdafx.h"
#include <recurj/rjframework/libs/base/messages/cmd.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionmessage.h>
using namespace rjBase;
RJ_ALLOCATOR(CRJMessageCmd, Allocator::Region::Message, AllocatorMessage::Cmd)
CRJMessageCmd* CRJMessageCmd::Get(size_t r, size_t c) {
	return new CRJMessageCmd(r, c);
}
CRJMessageCmd::CRJMessageCmd(size_t r, size_t c):
	CRJMessage(r, c) {}
CRJMessageCmd::~CRJMessageCmd() {}

