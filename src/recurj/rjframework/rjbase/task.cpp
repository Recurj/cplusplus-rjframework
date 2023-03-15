#include "stdafx.h"
#include <recurj/rjframework/libs/base/tasks/task.h>
#include <recurj/rjframework/libs/base/application/application.h>
using namespace rjBase;
CRJTaskDummy CRJTaskDummy::s_taskDummy;
CRJTask::CTaskManager CRJTask::s_TaskManager;
ITaskManager* CRJTask::s_pTaskManager=& CRJTask::s_TaskManager;
CRJStringByteVar& CRJTaskDummy::GetName(CRJStringByteVar& s) {
	return s.SetLabel("DUMMY", 5);
}
void CRJTask::TaskSetManager(ITaskManager& p) {
	CRJTask::s_pTaskManager=& p;
}
ITaskManager& CRJTask::TaskGetManager() {
	return * CRJTask::s_pTaskManager;
}
void CRJTask::TaskCleanup(CRJTask& clTask) {
	CRJTask::TaskGetManager().ITaskManagerCompleted(clTask);
}
void TaskThread(CRJTask& clTask) {
	CRJTask::funcTaskCleanup cleanup=clTask.TaskThreadStart();
	try {
		clTask.TaskRun(true);
	}
	catch(...) {}
	clTask.TaskThreadDone();
	cleanup(clTask);
}
CRJTask::CRJTask(size_t t):
	m_sTaskType(t) {
	m_clEventNeedBreak.EventCreate();
	m_clEventStopped.EventCreate();
}
CRJTask::~CRJTask() {}
int64_t CRJTask::TaskGetStackSize(int64_t st) {
	return st;
}
void CRJTask::TaskBreak() {
	m_clEventNeedBreak.EventSet();
}
void CRJTask::TaskRun(bool bSeparated) {
	if (TaskRunReady()) TaskRunBody();
	else TaskRunFailed();
	TaskRunDone(bSeparated);
}
bool CRJTask::TaskRunReady() {
	return RJHandle::HandleIsValid(m_clEventNeedBreak)&&RJHandle::HandleIsValid(m_clEventStopped);
}
void CRJTask::TaskRunBody() {}
void CRJTask::TaskRunDone(bool) {}
void CRJTask::TaskRunFailed() {
	CRJStringByteDefault s;
	CRJLoggerThread::ErrorText(GetName(s),"Task isn't ready", cLoggerPartTask);
}
CRJTask::funcTaskCleanup CRJTask::TaskGetCleanup() {
	return & TaskCleanup;
}
CRJTask::funcTaskCleanup CRJTask::TaskThreadStart() {
	CRJLoggerThread::Set(m_clTaskLog);
	return TaskGetCleanup();
}
void CRJTask::TaskThreadDone() {
	m_clEventStopped.EventSet();
	m_clRunning.latchClose();
}
bool CRJTask::TaskForeground() {
	try {
		if (!TaskIsRunning()&&
			m_clEventNeedBreak.EventReset()&&
			m_clEventStopped.EventReset()) {
			CRJLoggerThread::Set(m_clTaskLog);
			TaskRun(false);
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJTask::TaskStart() {
	try {
		if (& m_clTaskLog.GetWriter()==& CRJLogger::s_WriterDefault) m_clTaskLog.SetWriter(CRJApplication::Current().AppLogger().GetWriter());
		if (!TaskIsRunning()&&
			m_clEventNeedBreak.EventReset()&&
			m_clEventStopped.EventReset()) {
			if (CRJTask::TaskGetManager().ITaskManagerPrepare(*this)) {
				std::thread t(TaskThread, std::ref(*this));
				t.detach();
				m_clRunning.latchOpen();
				return true;
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJTask::TaskStop(size_t dw) {
	try {
		if (TaskIsRunning()) {
			TaskBreak();
			if (dw==cHandleWaitEndless) dw=cHandleWaitLong;
			TaskWaitTimed(dw);
		}
	}
	catch(...) {}
	return false;
}
bool CRJTask::TaskIsBreakOn(size_t dw) {
	if (dw==cHandleWaitEndless) dw=cHandleWaitLong;
	return m_clEventNeedBreak.WaitMe(dw);
}
bool CRJTask::TaskWait() {
	try {
		return m_clEventStopped.WaitMe(cHandleWaitEndless);
	}
	catch(...) {}
	return false;
}
bool CRJTask::TaskWaitTimed(size_t dw) {
	try {
		size_t i, j=dw/ 1000;
		for (i=0; i<=j; i++) {
			if (m_clEventStopped.WaitMe(1000)) return true;
			TaskBreak();
			CRJTask::ThreadYield();
		}
		j++;
	}
	catch(...) {}
	return false;
}
CRJMessage* CRJTask::SendNormal(CRJMessage& clMsg) const {
	return & clMsg;
}
bool CRJTask::SendList(CRJListMessages&) const {
	return false;
}
CRJMessage* CRJTask::SendUrgent(CRJMessage& clMsg) const {
	return SendNormal(clMsg);
}
bool CRJTask::TaskIsDummy() const {
	return false;
}
bool CRJTask::TaskIsReadyForMsgs(CRJTask&, size_t, size_t) {
	return false;
}
bool CRJTask::TaskIsReadyToUnload() {
	return true;
}
uint32_t CRJTask::ThreadSleep(uint32_t dw) {
	try {
		if (dw>0) std::this_thread::sleep_for(std::chrono::milliseconds(dw));
	}
	catch(...) {}
	return 0;
}
void CRJTask::ThreadYield() {
	std::this_thread::yield();
}
uint64_t CRJTask::ThreadGetId() {
	std::hash<std::thread::id>h;
	return h(std::this_thread::get_id());
}
CRJTask& CRJTask::TaskLogSetWriter(CRJLoggerWriter& writer) {
	m_clTaskLog.SetWriter(writer);
	return *this;
}
CRJTask& CRJTask::TaskLogSetLevels(const SRJLoggerLevels& d) {
	m_clTaskLog.SetLogLevels(d);
	return *this;
}
CRJLoggerWriter& CRJTask::TaskLogGetWriter() {
	return m_clTaskLog.GetWriter();
}

