#include "stdafx.h"
#include <recurj/rjframework/libs/base/tasks/taskqueuepool.h>
using namespace rjBase;
CRJTaskQueuePool::CRJTaskQueuePool() {
	Reset();
}
CRJTaskQueuePool::~CRJTaskQueuePool() {}
bool CRJTaskQueuePool::Insert(size_t sIndex, RJEvent& ev) {
	size_t sLast=m_clListener.Count();
	if (m_clListener.Insert(ev,(RJHandler)& CRJTaskQueuePool::OnWaitEventElement)) {
		m_sMapBusy[sLast]=sIndex;
		return true;
	}
	return false;
}
bool CRJTaskQueuePool::Remove(size_t sIndex, RJEvent& ev) {
	size_t ind=m_sMapBusy[sIndex];
	if (ind<cRJOSPollMax) {
		m_sMapBusy[sIndex]=cRJOSPollMax;
		if (m_clListener.RemoveByIndex(ind, ev, m_sMapBusy)) {
			m_sMapFree[sIndex]=m_sFree;
			m_sFree=sIndex;
			return true;
		}
	}
	return false;
}
void CRJTaskQueuePool::Reset() {
	size_t j, i=0;
	while (i<CRJOSPollElemCount) {
		j=i+1;
		m_sMapBusy[i]=cRJOSPollMax;
		m_sMapFree[i]=j;
		i=j;
	}
	m_sFree=0;
}

