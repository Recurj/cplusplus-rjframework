#include "stdafx.h"
#include <recurj/rjframework/libs/base/logger/writers/task.h>
#include <recurj/rjframework/libs/base/messages/log.h>
#include <recurj/rjframework/libs/base/messages/cmd.h>
#include <recurj/rjframework/libs/base/tasks/tasklogger.h>
#include <recurj/rjframework/libs/base/application/application.h>

using namespace rjBase;
CRJLogWriterTask::CRJLogWriterTask(CRJLogWriterFile& w):
	m_clTask(w) {}
CRJLogWriterTask::~CRJLogWriterTask() {}
bool CRJLogWriterTask::LogPrivate(const CRJStringByte&) {
	return false;
}
bool CRJLogWriterTask::LogPublic(const CRJStringByte& s) {
	return TaskMsgGetAndSend<CRJMessageLog,const CRJStringByte&>(m_clTask, s);
}
bool CRJLogWriterTask::Start(IRJLogWriterManager& mgr) {
	return m_clTask.TaskLogGetWriter().Start(mgr)&&
		m_clTask.TaskStart()&&
		m_clTask.TaskPing();
}
void CRJLogWriterTask::Stop() {
	//if (TaskMsgGetAndSend<CRJMessageCmd, size_t>(m_clTask, eRJMessageRegions::Base, eRJMessagesRegionBase::Quit)) m_clTask.TaskWait();
	//else {
		m_clTask.TaskStop();
		m_clTask.TaskLogGetWriter().Stop();
	//}
}
void CRJLogWriterTask::QueueReset()
{
	m_clTask.QueueReset();
}
