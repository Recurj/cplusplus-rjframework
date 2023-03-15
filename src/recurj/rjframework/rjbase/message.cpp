#include "stdafx.h"
#include <recurj/rjframework/libs/base/messages/message.h>
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/base/tasks/task.h>
using namespace rjBase;
void CRJMessage::Done(CRJMessage* p)
{
	if (p != nullptr) {
		freeMessage(p);
	}
}
void CRJMessage::freeMessage(CRJMessage* p) {
	try {
		delete p;
	}
	catch (...) {}
}

CRJMessage* CRJMessage::Get(size_t r, size_t c) {
	return new CRJMessage(r, c);
}
CRJMessage::CRJMessage():
	m_sMsgRegion(0),m_sMsgCmd(0) {}
CRJMessage::CRJMessage(size_t region, size_t cmd):
	m_sMsgRegion(region),m_sMsgCmd(cmd) {}
CRJMessage::CRJMessage(const CRJMessage& msg):
	m_sMsgRegion(msg.m_sMsgRegion),m_sMsgCmd(msg.m_sMsgCmd) {}
CRJMessage::~CRJMessage() {}
bool CRJMessage::Log(CRJLogger& clLogger) {
	return clLogger.GetWriter().LogPrivate(clLogger.Header(CRJDTimeTools::GetDTime()).CatSingle('=').CatInt64(m_sMsgRegion).CatSingle(':').CatInt64(m_sMsgCmd));
}
EMessageserialize CRJMessage::Serialize(CRJByteWriter&) {
	return EMessageserialize::NotSupported;
}
CRJMessage* CRJMessage::Duplicate() const {
	return new CRJMessage(*this);
}
bool CRJMessage::Pack(CRJStringByteVar&) {
	return false;
}
bool CRJMessage::Unpack(const CRJStringByte&) {
	return false;
}
bool CRJMessage::Send(CRJTask& clTask, size_t region, size_t sMsg) {
	CRJMessage* pMsg=nullptr;
	try {
		pMsg=CRJMessage::Get(region, sMsg);
		if (pMsg!=nullptr&&
			clTask.SendNormal(* pMsg)==nullptr) return true;
	}
	catch(...) {}
	CRJMessage::Done(pMsg);
	return false;
}
bool CRJMessage::SendUrgent(CRJTask& clTask, size_t region, size_t sMsg) {
	CRJMessage* pMsg=nullptr;
	try {
		pMsg=CRJMessage::Get(region, sMsg);
		if (pMsg!=nullptr&&
			clTask.SendUrgent(* pMsg)==nullptr) return true;
	}
	catch(...) {}
	CRJMessage::Done(pMsg);
	return false;
}

