#include "stdafx.h"
#include <recurj/rjframework/libs/net/netsocketevent.h>
#pragma warning (disable :4548)
using namespace rjNet;
CRJSocketTCPEvent::CRJSocketTCPEvent(IRJSocketHolder&holder):CRJSocketTCP(holder)
{}
CRJSocketTCPEvent::~CRJSocketTCPEvent() {
	SocketEventClose();
}
#ifdef _rjOS_FAMILY_UNIX
 bool CRJSocketTCPEvent::SocketEventPrepareListen() {
	SocketEventClose();
	return SocketEventPrepare(0);
}
bool CRJSocketTCPEvent::SocketEventPrepareConnect() {
	SocketEventClose();
	return SocketEventPrepare(0);
}
bool CRJSocketTCPEvent::SocketEventPrepareAccepted() {
	SocketEventClose();
	return SocketEventPrepare(0);
}
bool CRJSocketTCPEvent::SocketEventPrepare(int64_t) {
	try {
		if (!SocketIsValid()) SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::SET_EVENT);
		else if (m_clEvent.Duplicate(SocketGet())) return true;
		else SocketOnError(EX_SOCKET_BAD_OPTIONS, CRJNet::eSocketActions::SET_EVENT);
	}
	catch(...) {}
	return false;
}
void CRJSocketTCPEvent::SocketEventClose() {
	m_clEvent.Close();
}
CRJSocketTCPEvent& CRJSocketTCPEvent::GetEvents(size_t lEvent, IRJNetSocketTCPEvent& cb) {
	switch (lEvent) {
	case EPOLLIN:OnNetEventRead();
		break;
	case EPOLLOUT:OnNetEventSend();
		break;
	case EPOLLHUP:OnNetEventClose();
		break;
	case EPOLLERR:OnNetEventError(SocketGetError(), CRJNet::eSocketActions::ENUM, cb);
		break;
	}
	return *this;
}
#endif 
#ifdef _rjOS_WIN
 bool CRJSocketTCPEvent::SocketEventPrepareListen() {
	SocketEventClose();
	return SocketEventPrepare(FD_ACCEPT);
}
bool CRJSocketTCPEvent::SocketEventPrepareConnect() {
	SocketEventClose();
	return SocketEventPrepare(FD_CONNECT| FD_CLOSE| FD_READ| FD_WRITE);
}
bool CRJSocketTCPEvent::SocketEventPrepareAccepted() {
	SocketEventClose();
	return SocketEventPrepare(FD_CLOSE| FD_READ| FD_WRITE);
}
bool CRJSocketTCPEvent::SocketEventPrepare(int64_t lFlags) {
	try {
		if (!SocketIsValid()) SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::SET_EVENT);
		else if (m_clEvent.EventCreate()) {
			if (!::WSAEventSelect(SocketGet(), m_clEvent,(long) lFlags)) return true;
			else SocketOnError(::WSAGetLastError(), CRJNet::eSocketActions::SET_EVENT);
		}
		else SocketOnError(EX_SOCKET_BAD_OPTIONS, CRJNet::eSocketActions::SET_EVENT);
	}
	catch(...) {}
	return false;
}
void CRJSocketTCPEvent::SocketEventClose() {
	try {
		if (RJHandle::HandleIsValid(m_clEvent))::WSAEventSelect(SocketGet(), m_clEvent, 0L);
	}
	catch(...) {}
	m_clEvent.HandleRelease();
}
CRJSocketTCPEvent& CRJSocketTCPEvent::HappenedEvents(IRJSocketManager& clManager,size_t) {
	try {
		WSANETWORKEVENTS nEvents;
		if (!::WSAEnumNetworkEvents(SocketGet(), m_clEvent,& nEvents)) {
			if (nEvents.lNetworkEvents& FD_READ) {
				if (nEvents.iErrorCode[FD_READ_BIT]) clManager.OnNetEventError(nEvents.iErrorCode[FD_READ_BIT], CRJNet::eSocketActions::READ);
				else clManager.OnNetEventRead();
			}
			if (nEvents.lNetworkEvents& FD_CLOSE) {
				if (nEvents.iErrorCode[FD_CLOSE_BIT]) clManager.OnNetEventError(nEvents.iErrorCode[FD_CLOSE_BIT], CRJNet::eSocketActions::CLOSE);
				else clManager.OnNetEventClose();
			}
			else {
				if (nEvents.lNetworkEvents& FD_WRITE) {
					if (nEvents.iErrorCode[FD_WRITE_BIT]) clManager.OnNetEventError(nEvents.iErrorCode[FD_WRITE_BIT], CRJNet::eSocketActions::SEND);
					clManager.OnNetEventSend();
				}
				if (nEvents.lNetworkEvents& FD_CONNECT) {
					if (nEvents.iErrorCode[FD_CONNECT_BIT]) clManager.OnNetEventError(nEvents.iErrorCode[FD_CONNECT_BIT], CRJNet::eSocketActions::CONNECT);
					else clManager.OnNetEventConnect();
				}
				if (nEvents.lNetworkEvents& FD_ACCEPT) {
					if (nEvents.iErrorCode[FD_ACCEPT_BIT]) clManager.OnNetEventError(nEvents.iErrorCode[FD_ACCEPT_BIT], CRJNet::eSocketActions::ACCEPT);
					else clManager.OnNetEventAccept();
				}
			}
		}
		else {
			m_clEvent.EventReset();
			clManager.OnNetEventError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::ENUM);
		}
	}
	catch(...) {}
	return *this;
}
#endif 
 void CRJSocketTCPEvent::SocketRelease() {
	SocketEventClose();
	CRJSocket::SocketRelease();
}
void CRJSocketTCPEvent::OnNetEventError(IRJSocketManager& clManager,int64_t lErr, CRJNet::eSocketActions lAction) {
	SocketOnError(lErr, lAction);
	if (!IsConnectedAndReady()) clManager.OnNetEventClose();
}
void CRJSocketTCPEventSend::OnNetEventSend() {
	m_clNetSend.SendReady();
}

