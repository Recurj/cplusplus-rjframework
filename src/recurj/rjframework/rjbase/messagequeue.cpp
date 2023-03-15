#include "stdafx.h"
#include <recurj/rjframework/libs/base/messages/queue.h>
using namespace rjBase;
CRJQueueMessages::CRJQueueMessages()noexcept
{}
CRJQueueMessages::~CRJQueueMessages() {
	QueueStop();
}
bool CRJQueueMessages::QueueStart() {
	std::lock_guard<std::mutex>lg(m_clQueueCritical);
	return npQueueStart();
}
void CRJQueueMessages::QueueStop() {
	std::lock_guard<std::mutex>lg(m_clQueueCritical);
	QueueCloseLatch();
	npQueueStop();
}
bool CRJQueueMessages::QueueCheck() {
	if (IsQueueLatchNotClosed()) {
		std::lock_guard<std::mutex>lg(m_clQueueCritical);
		EventSet();
		return true;
	}
	return false;
}
void CRJQueueMessages::QueueClear() {
	std::lock_guard<std::mutex>lg(m_clQueueCritical);
	npQueueClear();
	EventReset();
}
void CRJQueueMessages::QueueReset() {
	std::lock_guard<std::mutex>lg(m_clQueueCritical);
	m_clQueueList.Reset();
}
bool CRJQueueMessages::QueueBot(CRJMessage& clElem) {
	std::lock_guard<std::mutex>lg(m_clQueueCritical);
	m_clQueueList.Bot(clElem);
	EventSet();
	return true;
}
bool CRJQueueMessages::QueueBotRestricted(CRJMessage& clElem, size_t sMax) {
	std::lock_guard<std::mutex>lg(m_clQueueCritical);
	if (m_clQueueList.Count() < sMax) {
		m_clQueueList.Bot(clElem);
		EventSet();
	}
	return true;
}
bool CRJQueueMessages::QueueBot(CRJListMessages& clList) {
	if (clList.Count() > 0) {
		std::lock_guard<std::mutex>lg(m_clQueueCritical);
		m_clQueueList.Bot(clList);
		EventSet();
	}
	return true;
}
bool CRJQueueMessages::QueueTop(CRJMessage& clElem) {
	std::lock_guard<std::mutex>lg(m_clQueueCritical);
	m_clQueueList.Top(clElem);
	EventSet();
	return true;
}
bool CRJQueueMessages::QueueTop(CRJListMessages& clList) {
	if (clList.Count() > 0) {
		std::lock_guard<std::mutex>lg(m_clQueueCritical);
		m_clQueueList.Top(clList);
		EventSet();
	}
	return true;
}
CRJMessage* CRJQueueMessages::QueuePop() {
	std::lock_guard<std::mutex>lg(m_clQueueCritical);
	CRJMessage* p = m_clQueueList.PopTop();
	if (!m_clQueueList.Count()) EventReset();
	return p;
}
CRJQueueMessages::SPop& CRJQueueMessages::QueueMove2Cache(SPop& st) {
	std::lock_guard<std::mutex>lg(m_clQueueCritical);
	st.m_sCount = m_clQueueList.Count();
	if (st.m_sCount == 0) {
		st.m_pTop = nullptr;
		npOnQueueEmptyList(st);
	}
	else st.m_pTop = m_clQueueList.PopAll();
	EventReset();
	return st;
}
bool CRJQueueMessages::IsQueueEmpty() {
	std::lock_guard<std::mutex>lg(m_clQueueCritical);
	return m_clQueueList.Count() == 0;
}
size_t CRJQueueMessages::GetQueueCounts() {
	std::lock_guard<std::mutex>lg(m_clQueueCritical);
	return m_clQueueList.Count();
}
bool CRJQueueMessages::QueueSet() {
	std::lock_guard<std::mutex>lg(m_clQueueCritical);
	return EventSet();
}
void CRJQueueMessages::npOnQueueEmptyList(SPop&) {
}
bool CRJQueueMessages::npQueueStart() {
	if (RJHandle::HandleIsValid(m_hHandle)) return npFuncReset();
	return EventCreate();
}
void CRJQueueMessages::npQueueStop() {
	npQueueClear();
	HandleRelease();
}
void CRJQueueMessages::npQueueClear() {
	m_clQueueList.Clear();
}
