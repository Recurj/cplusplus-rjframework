#include "stdafx.h"
#include <recurj/rjframework/libs/www/http/defs.h>
using namespace rjWWW;
CHTTP::CMessage::CMessage() {}
CHTTP::CMessage::~CMessage() {}
void CHTTP::CMessage::Restart() {
	SetEmpty();
	m_clVersionRecv.SetEmpty();
	m_clBinaryData.SetEmpty();
}
bool CHTTP::CMessage::Attributes(AProcessor& cb) {
	CRJStringByteFlex v, a, f(*this);
	while (f.WasFoundTopAndMovedHead(0x0a, a)) {
		if (!a.WasFoundTopAndMovedTail(':', v)) return false;
		if (!cb(a, v)) return false;
	}
	if (f.GetLength()>0) {
		return f.WasFoundTopAndMovedTail(':', v)?
			cb(f, v):
			false;
	}
	return true;
}
bool CHTTP::CMessage::SetHandlerResult(CHandler::eResult r, CHTTP::Status s) {
	m_lHandlerResult=r;
	m_lHandlerStatus=s;
	return false;
}

