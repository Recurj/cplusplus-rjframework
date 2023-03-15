#include "stdafx.h"
#include <recurj/rjframework/libs/net/netsocketudp.h>
using namespace rjNet;
CRJSocketUDP::CRJSocketUDP(IRJSocketHolder& holder) :CRJSocket(holder) {}
CRJSocketUDP::~CRJSocketUDP() {}
CRJSocketUDP& CRJSocketUDP::SocketUDPAlloc() {
	SocketRelease();
	try {
		m_hHandle=::socket(AF_INET, SOCK_DGRAM, 0);
		if (!SocketIsValid(m_hHandle)) SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::ALLOC);
		else SocketSetOptionLinger(false, 0);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::IPC_ALLOC);
	}
	return *this;
}
bool CRJSocketUDP::SocketUDPReadBlock(uint8_t* pBuf, size_t& l) {
	try {
		struct sockaddr s;
#ifdef _rjOS_FAMILY_UNIX
		socklen_t ls=sizeof(s);
#endif 
#ifdef _rjOS_WIN
		int32_t ls=static_cast<int32_t>(sizeof(s));
#endif 
		int32_t lRead=::recvfrom(m_hHandle,(char*) pBuf,static_cast<int32_t>(l), 0,(
		struct sockaddr*)& s,& ls);
		if (lRead<0) SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::READ);
		else {
			l=static_cast<size_t>(lRead);
			return lRead>0;
		}
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::READ);
	}
	return false;
}
bool CRJSocketUDP::SocketUDPRead(uint8_t* bData, size_t lLen,const SRJSocketProcessMetrics& stMetrics, size_t& lTotalRead) {
	try {
		if (!SocketIsValid(m_hHandle)) SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::READ);
		size_t l=0;
		auto lRetry=stMetrics.lRetry;
		while (1) {
			if (SocketReadIsWaiting(l)) {
				lTotalRead=(l>lLen)?
					lLen:1;
				return SocketUDPReadBlock(bData, lTotalRead)&&
					lTotalRead==lLen;
			}
			lRetry--;
			if (lRetry<=0)break;
			if (!OnSocketReadNeedWait(stMetrics))break;
		}
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::READ);
	}
	return false;
}
bool CRJSocketUDP::SocketUDPSend(CRJStringByteFlex& block, CRJNetSend& sockSend,
struct sockaddr_in& sin) {
	try {
		if (!SocketIsValid(m_hHandle)) return sockSend.SetError(EX_SOCKET_ERROR_NOT_EXIST);
		auto lRetry=sockSend.GetMetrics().lRetry;
		do {
			if (!sockSend.SendResult(block,static_cast<int64_t>(::sendto(m_hHandle,block.GetHeadForReadC(), (int32_t)block.GetLength(), 0, (
			struct sockaddr*)& sin,sizeof(sin))))) return false;
			if (block.IsEmpty()) return true;
			else {
				lRetry--;
				if (!OnSocketSendNeedWait(sockSend.GetMetrics()))break;
			}
		}
		while (lRetry>0);
	}
	catch(...) {}
	return sockSend.SetError(EX_SOCKET_ERROR_SEND);
}
bool CRJSocketUDP::SocketUDPBindServer(size_t lPort,
struct sockaddr_in& sin) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::BIND_LISTEN);
		int32_t b=1L;
		_MEMCLR_BYTE(sin);
		sin.sin_addr.s_addr=INADDR_ANY;
		sin.sin_port=htons((uint16_t) lPort);
		sin.sin_family=AF_INET;
		if (::setsockopt(m_hHandle, SOL_SOCKET, SO_REUSEADDR,(char*)& b,sizeof(b))) SocketWarning(CRJNet::GetNetLastError(), CRJNet::eSocketActions::BIND_LISTEN);
		if (!::bind(m_hHandle,(
		struct sockaddr*)& sin,sizeof(sin))) return true;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::BIND_LISTEN);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::BIND_LISTEN);
	}
	return false;
}
bool CRJSocketUDP::SocketUDPBindServerLocal(size_t lPort,
struct sockaddr_in& sin) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::BIND_LISTEN);
		int32_t b=1L;
		_MEMCLR_BYTE(sin);
		if (!SocketFindRemoteHostByName(CRJSocketUDP::s_clLocalHost, sin.sin_addr)) return false;
		sin.sin_port=htons((uint16_t) lPort);
		sin.sin_family=AF_INET;
		if (::setsockopt(m_hHandle, SOL_SOCKET, SO_REUSEADDR,(char*)& b,sizeof(b))) SocketWarning(CRJNet::GetNetLastError(), CRJNet::eSocketActions::BIND_LISTEN);
		if (!::bind(m_hHandle,(
		struct sockaddr*)& sin,sizeof(sin))) return true;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::BIND_LISTEN);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::BIND_LISTEN);
	}
	return false;
}
bool CRJSocketUDP::SocketUDPBindClient() {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::BIND_LISTEN);
		struct sockaddr_in sin;
		_MEMCLR_BYTE(sin);
		sin.sin_addr.s_addr=INADDR_ANY;
		sin.sin_port=0;
		sin.sin_family=AF_INET;
		if (!::bind(m_hHandle,(
		struct sockaddr*)& sin,sizeof(sin))) return true;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::BIND_LISTEN);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::BIND_LISTEN);
	}
	return false;
}

