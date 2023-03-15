#include "stdafx.h"
#include <recurj/rjframework/libs/base/tasks/tasklogger.h>
#include <recurj/rjframework/libs/base/application/application.h>
using namespace rjBase;
CRJStringByteVar& CRJTaskLogger::GetName(CRJStringByteVar& s) {
	return s.Set(CRJLogger::s_logger);
}
CRJTaskLogger::CRJTaskLogger(CRJLoggerWriter& clLoggerWriter):
	CRJTaskQueueBasic(cTaskTypeLog),
m_sQueueLimit(cLoggerTaskQueueLimit) {
	m_clTaskLog.SetWriter(clLoggerWriter);
}
CRJTaskLogger::~CRJTaskLogger() {}
void CRJTaskLogger::TaskRunDone(bool bSeparated) {
	m_clTaskLog.GetWriter().Stop();
	CRJTaskQueueBasic::TaskRunDone(bSeparated);
}
CRJMessage* CRJTaskLogger::OnTaskQueueMsg(CRJMessage& clMsg) {
	switch (clMsg.GetRegion()) {
	case eRJMessageRegions::Log: return Out(clMsg);
		break;
	}
	return CRJTaskQueueBasic::OnTaskQueueMsg(clMsg);
}
CRJMessage* CRJTaskLogger::Out(CRJMessage& clMsg) {
	clMsg.Log(m_clTaskLog);
	return & clMsg;
}
CRJMessage* CRJTaskLogger::SendNormal(CRJMessage& clMsg) const {
	return CRJTaskQueue::SendRestricted(clMsg, m_sQueueLimit.Get());
}
void CRJTaskLogger::SetQueueLimit(size_t s) {
	m_sQueueLimit.Set(s);
}

