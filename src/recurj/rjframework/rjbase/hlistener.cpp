#include "stdafx.h"
#include <recurj/rjframework/libs/base/os/hlistener.h>
#include <recurj/rjframework/libs/base/os/hevent.h>
using namespace rjBase;
CRJEventListener::CRJEventListener():
	m_pElems(nullptr),
m_pEvents(nullptr),
m_pIndexes(nullptr),
m_sMax(0),
m_sCount(0) {}
void CRJEventListener::Bind(size_t m, ListenerMapElem* el, HOS* h) {
	m_sMax=m;
	m_pElems=el;
	m_pEvents=h;
}
void CRJEventListener::Rebuild() {
	size_t i=0;
	ListenerMapElem* pElem=m_pElems;
	HOS* pEvent=m_pEvents;
	while (i<m_sCount) {
		i++;
		* pEvent=* pElem->pevent;
		pElem++;
		pEvent++;
	}
}
bool CRJEventListener::Insert(RJHandle& ev, CRJEventHandler::RJHandler h) {
	if (m_sCount<m_sMax) {
		m_pElems[m_sCount].pevent=& ev;
		m_pElems[m_sCount].handler=h;
		m_pEvents[m_sCount]=ev;
		m_sCount++;
		return true;
	}
	return false;
}
bool CRJEventListener::Remove(RJHandle& ev) {
	size_t i=m_sCount;
	while (i>0) {
		i--;
		if (m_pElems[i].pevent==& ev) {
			m_sCount--;
			if (i<m_sCount) Rebuild();
			return true;
		}
	}
	return false;
}
bool CRJEventListener::RemoveByIndex(size_t ind, RJHandle& ev, size_t p[]) {
	if (ind<m_sCount&&
		m_pElems[ind].pevent==& ev) {
		size_t i;
		m_sCount--;
		while (ind<m_sCount) {
			i=ind+1;
			m_pElems[ind].pevent=m_pElems[i].pevent;
			m_pElems[ind].handler=m_pElems[i].handler;
			p[ind]=p[i];
			ind++;
		}
	}
	return false;
}
void CRJEventListener::Reset() {
	size_t i=0;
	ListenerMapElem* pElem=m_pElems;
	HOS* pEvent=m_pEvents;
	m_sCount=0;
	while (i<m_sMax) {
		i++;
		pElem->pevent=nullptr;
		pElem->handler=& CRJEventHandler::HandlerDefault;
		* pEvent=nullptr;
		pElem++;
		pEvent++;
	}
}
void CRJEventListener::Wait(CRJEventHandler& listener, size_t sWaitDelay) {
	size_t i=static_cast<size_t>(::WaitForMultipleObjects(static_cast<DWORD>(m_sCount), m_pEvents,false, RJEvent::GetTimeout(sWaitDelay)));
	if (i<m_sCount) {
		size_t w, n;
		if (!(listener.* m_pElems[i].handler)(i)) return ;
		i++;
		n=m_sCount-i;
		while (n>0) {
			w=static_cast<size_t>(::WaitForMultipleObjects(static_cast<DWORD>(n),& m_pEvents[i],false, 0));
			if (w<n) {
				i+=w;
				if (!(listener.* m_pElems[i].handler)(i)) return ;
				i++;
				n=m_sCount-i;
			}
			else break;
		}
	}
	else if (i==WAIT_TIMEOUT) listener.OnEventHandlerTimeout();
	else listener.OnEventHandlerError(CRJDataTools::GetLastError());
}

