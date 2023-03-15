#include "stdafx.h"
#include <recurj/rjframework/libs/base/tasks/taskqueue.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
using namespace rjBase;
CRJTaskQueue::CRJTaskQueue(size_t t):
	CRJTask(t),
m_clQueueWait(cHandleWaitEndless),
m_bNeedBreak(false) {}
CRJTaskQueue::~CRJTaskQueue() {}
void CRJTaskQueue::QueueReset()
{
	m_clQueue.QueueReset();
}

size_t CRJTaskQueue::TaskQueueDefaultWait() {
	return cHandleWaitEndless;
}
size_t CRJTaskQueue::TaskQueueCheckMinWait(size_t d) {
	return d;
}
CRJMessage* CRJTaskQueue::SendNormal(CRJMessage& clMsg) const {
	return m_clQueue.QueueBot(clMsg)? nullptr:& clMsg;
}
bool CRJTaskQueue::SendList(CRJListMessages& clMessages) const {
	return m_clQueue.QueueBot(clMessages);
}
CRJMessage* CRJTaskQueue::SendRestricted(CRJMessage& clMsg, size_t sMax) const {
	return m_clQueue.QueueBotRestricted(clMsg, sMax)?
		nullptr:& clMsg;
}
bool CRJTaskQueue::TaskIsReadyForMsgs(CRJTask& clParent, size_t lLoop, size_t sDelay) {
	while (!m_clQueue.QueueCheck()&&
		lLoop>0) {
		lLoop--;
		if ((clParent.TaskIsBreakOn((uint32_t) sDelay))||
			(TaskIsBreakOn(0))) return false;
	}
	return lLoop>0;
}
bool CRJTaskQueue::TaskPing(size_t c, size_t dw) {
	while (c>0) {
		if (m_clQueue.QueueCheck()) return true;
		CRJTask::ThreadSleep((uint32_t) dw);
		c--;
	}
	return false;
}
bool CRJTaskQueue::TaskQueueSetListener() {
	return m_clListener.Insert(m_clEventNeedBreak,(RJHandler)& CRJTaskQueue::OnWaitEventBreak)&&
		m_clListener.Insert(m_clQueue,(RJHandler)& CRJTaskQueue::OnWaitEventQueue);
}
bool CRJTaskQueue::TaskRunReady() {
	m_clListener.Reset();
	return CRJTask::TaskRunReady()&&
		m_clQueue.QueueStart()&&
		TaskQueueSetListener();
}
void CRJTaskQueue::TaskRunBody() {
	m_bNeedBreak=false;
	m_clQueue.QueueOpenLatch();
	TaskQueueReady();
	while (!m_bNeedBreak) TaskQueueLoop();
	m_clQueue.QueueCloseLatch();
	TaskQueueDone();
}
void CRJTaskQueue::TaskRunDone(bool bSeparated) {
	m_clQueue.QueueStop();
	m_clQueue.QueueClear();
	m_clListener.Reset();
	CRJTask::TaskRunDone(bSeparated);
}
void CRJTaskQueue::TaskQueueReady() {}
void CRJTaskQueue::TaskQueueDone() {}
void CRJTaskQueue::TaskQueueLoop() {
	m_clListener.Wait(*this, m_clQueueWait.Get());
}
void CRJTaskQueue::TaskQueueMessagesBegin(size_t) {}
void CRJTaskQueue::TaskQueueMessagesEnd() {}
CRJMessage* CRJTaskQueue::OnTaskQueueMsg(CRJMessage& clMsg) {
	switch (clMsg.GetRegion()) {
	case eRJMessageRegions::Base: return onMsgRegionBase(clMsg);
		break;
	}
	return & clMsg;
}
CRJMessage* CRJTaskQueue::onMsgRegionBase(CRJMessage& clMsg) {
	switch (clMsg.GetCommand()) {
	case eRJMessagesRegionBase::Quit:TaskBreak();break;
	case eRJMessagesRegionBase::Ping:break;
	}
	return & clMsg;
}
void CRJTaskQueue::OnEventHandlerError(int64_t err) {
	CRJStringByteDefault s;
	CRJLoggerThread::ErrorApp(err, GetName(s), cLoggerPartTask, cLoggerLevelTrace);
	CRJTask::OnEventHandlerError(err);
}
void CRJTaskQueue::OnWaitEventQueue(size_t) {
	TaskQueueProcess(m_clQueue);
}
void CRJTaskQueue::TaskQueueProcess(CRJQueueMessages& queue) {
	CRJQueueMessages::SPop stQueueCache;
	TaskQueueMessagesBegin(queue.QueueMove2Cache(stQueueCache).m_sCount);
	try {
		CRJMessage* pNext,* pList=stQueueCache.m_pTop;
		while (pList) {
			pNext=pList->GetNext();
			pList->ItemReset();
			CRJMessage::Done(OnTaskQueueMsg(* pList));
			TaskQueueMsgProcessed();
			pList=pNext;
		}
	}
	catch(...) {}
	TaskQueueMessagesEnd();
}
void CRJTaskQueue::TaskQueueProcess(CRJListMessages& list) {
	TaskQueueMessagesBegin(list.Count());
	try {
		CRJMessage* pList=list.PopTop();
		while (pList) {
			CRJMessage::Done(OnTaskQueueMsg(* pList));
			TaskQueueMsgProcessed();
			pList=list.PopTop();
		}
	}
	catch(...) {}
	TaskQueueMessagesEnd();
}
void CRJTaskQueue::TaskQueueMsgProcessed() {}
size_t CRJTaskQueue::TaskQueueWaitParameter(const CRJStringByte& p, size_t d) {
	if (p.GetLength()>0) {
		CRJStringByteFlex f(p);
		size_t t;
		uint8_t ch=f[0];
		if (!CRJByteConvertor::IsDecDigit((uint8_t) ch)) {
			if (f.TruncLeft(1).IsValidSize(t)&&
				t>0) {
				switch (ch) {
				case 'D':
				case 'd': return t* 1000* 60* 60* 24;
				case 'H':
				case 'h': return t* 1000* 60* 60;
				case 'M':
				case 'm': return t* 1000* 60;
				case 'S':
				case 's': return t* 1000;
				}
			}
		}
		else if (f.IsValidSize(t)&&
			t>0) return t;
	}
	return d;
}

