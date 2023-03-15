#include "stdafx.h"
#include <recurj/rjframework/libs/net/netsockettcp.h>
using namespace rjNet;
CRJSocketTCP::CRJSocketTCP(IRJSocketHolder& holder) :CRJSocket(holder) {}
CRJSocketTCP::~CRJSocketTCP() {}
CRJSocketTCP& CRJSocketTCP::SocketTCPAlloc() {
	SocketRelease();
	try {
#ifdef _rjOS_FAMILY_UNIX
		m_hHandle=::socket(AF_INET, SOCK_STREAM| SOCK_CLOEXEC, 0);
#endif 
#ifdef _rjOS_WIN
		m_hHandle=::socket(AF_INET, SOCK_STREAM, 0);
#endif 
		if (SocketIsValid(m_hHandle)) {
			SocketSetOptionLinger(false, 0);
		}
		else SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::ALLOC);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::ALLOC);
	}
	return *this;
}
bool CRJSocketTCP::SocketTCPListen(size_t lCount) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::LISTEN);
		if (!::listen(m_hHandle,(int) lCount)) return true;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::LISTEN);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::LISTEN);
	}
	return false;
}
bool CRJSocketTCP::SocketTCPAccept(CRJNetSocketTCPAccepted& clSock,bool bKeepAlive) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::ACCEPT);
		struct sockaddr s;
#ifdef _rjOS_FAMILY_UNIX
		socklen_t l=sizeof(s);
		HSOCKET h=::accept4(m_hHandle,& s,& l, SOCK_CLOEXEC);
#endif 
#ifdef _rjOS_WIN
		int32_t l=static_cast<int32_t>(sizeof(s));
		HSOCKET h=::accept(m_hHandle,& s,& l);
#endif 
		if (CRJSocket::SocketIsValid(h)) {
			clSock.SocketTCPOnAccepted(h, bKeepAlive);
			return true;
		}
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::ACCEPT);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::ACCEPT);
	}
	return false;
}
bool CRJSocketTCP::SocketTCPConnect(size_t lPort,const CRJStringByte& clHost,bool bKeepAlive) {
	SocketTCPSetOptionKeepAlive(bKeepAlive);
	return SocketConnect(lPort, clHost);
}
bool CRJSocketTCP::SocketTCPReadExisting(CRJStringByteVar& clCollector) {
	try {
		size_t l=0L;
		if (SocketReadIsWaiting(l)) {
			if (l>0) {
				size_t lAvailable=clCollector.GetCapacity();
				if (lAvailable<l) l=lAvailable;
				if (l>0) {
					if (SocketReadBlock(const_cast<uint8_t*>(clCollector.GetTail()), l)) {
						clCollector.ExpandedOutside(l);
						return true;
					}
				}
				else return true;
			}
		}
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::READ);
	}
	return false;
}
bool CRJSocketTCP::SocketTCPReadAvailable(CRJStringByteVar& clCollector) {
	try {
		size_t lAvailable=clCollector.GetCapacity();
		if ((lAvailable>0)&&
			(SocketReadBlock(const_cast<uint8_t*>(clCollector.GetTail()), lAvailable))) {
			clCollector.ExpandedOutside(lAvailable);
			return true;
		}
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::READ);
	}
	return false;
}
int64_t CRJSocketTCP::SocketTCPSetOptionNoDelay(bool bReUse) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::SET_OPTION_NO_DELAY);
		int32_t b=bReUse& 1;
		if (!::setsockopt(m_hHandle, IPPROTO_TCP, TCP_NODELAY,(char*)& b,sizeof(b))) return 0;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::SET_OPTION_NO_DELAY);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::SET_OPTION_NO_DELAY);
	}
	return EX_SOCKET_ERROR_EXCEPTION;
}
int64_t CRJSocketTCP::SocketTCPSetOptionKeepAlive(bool bKeepAlive) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::SET_OPTION_KEEPALIVE);
		int32_t b=(bKeepAlive)?
			1:0;
		if (!::setsockopt(m_hHandle, SOL_SOCKET, SO_KEEPALIVE,(char*)& b,sizeof(b))) return 0;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::SET_OPTION_KEEPALIVE);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::SET_OPTION_KEEPALIVE);
	}
	return EX_SOCKET_ERROR_EXCEPTION;
}
void CRJNetSocketTCPAccepted::SocketTCPOnAccepted(HSOCKET h,bool bKeepAlive) {
	SocketAttach(h);
	SocketTCPSetOptionKeepAlive(bKeepAlive);
}

