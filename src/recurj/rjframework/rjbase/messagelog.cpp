#include "stdafx.h"
#include <recurj/rjframework/libs/base/messages/log.h>
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionmessage.h>
using namespace rjBase;
RJ_ALLOCATOR(CRJMessageLog, Allocator::Region::Message, AllocatorMessage::Log) 
CRJMessageLog* CRJMessageLog::Get(const CRJStringByte& s) {
	return new CRJMessageLog(s);
}
CRJMessageLog::CRJMessageLog(const CRJStringByte& s):
	CRJMessage(eRJMessageRegions::Log, 0) {
	m_clData.Set(s);
}
CRJMessageLog::~CRJMessageLog() {}
bool CRJMessageLog::Log(CRJLogger& clLogger) {
	return clLogger.GetWriter().LogPrivate(m_clData);
}

