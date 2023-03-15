#include "stdafx.h"
#include <recurj/rjframework/libs/net/ipc/netsocketipc.h>
using namespace rjNet;
CRJSocketIPC::CRJSocketIPC(IRJSocketHolder& holder) :CRJSocket(holder) {}
CRJSocketIPC::~CRJSocketIPC() {}
#ifdef _rjOS_FAMILY_UNIX
CRJSocket& CRJSocketIPC::SocketIPCAlloc() {
	SocketRelease();
	try {
		m_hHandle=::socket(PF_UNIX, SOCK_STREAM, 0);
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
bool CRJSocketIPC::SocketUnixConnect(const CRJString& s) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::UINX_CONNECT);
		struct sockaddr_un addr;
		socklen_t l;
		CRJStringByteDefault v((uint8_t*) addr.sun_path,sizeof(addr.sun_path)-1);
		addr.sun_family=AF_UNIX;
		v.Set(s).MakeC();
		l=sizeof(addr.sun_family)+v.GetLength();
		if (::connect(m_hHandle,(
		struct sockaddr*)& addr, l)) return true;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::UINX_CONNECT);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::CONNECT);
	}
	return false;
}
bool CRJSocketIPC::SocketUnixListen(const CRJString& s) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::UNIX_LISTEN);
		struct sockaddr_un addr;
		socklen_t l;
		CRJStringByteDefault v((uint8_t*) addr.sun_path,sizeof(addr.sun_path)-1);
		addr.sun_family=AF_UNIX;
		v.Set(s).MakeC();
		l=sizeof(addr.sun_family)+v.GetLength();
		if (::bind(m_hHandle,(
		struct sockaddr*)& addr, l)&&
			!::listen(m_hHandle, 5)) return true;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::UNIX_LISTEN);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::LISTEN);
	}
	return false;
}
bool CRJSocketIPC::SocketUnixAccept(CRJSocket& clSock) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::UINX_ACCEPT);
		struct sockaddr_un addr;
		socklen_t l=(socklen_t)sizeof(addr);
		HSOCKET h=::accept4(m_hHandle,(
		struct sockaddr*)& addr,& l, SOCK_CLOEXEC);
		if (CRJSocketIPC::SocketIsValid(h)) {
			clSock.SocketAttach(h);
			return true;
		}
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::UINX_ACCEPT);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::ACCEPT);
	}
	return false;
}
bool CRJSocketIPC::SocketMsgRead(
struct msghdr& msg, size_t t) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::READMSG);
		SocketTimeOut(t);
		if (!::recvmsg(SocketGet(),& msg, 0)) return true;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::READMSG);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::READMSG);
	}
	return false;
}
bool CRJSocketIPC::SocketMsgSend(const 
struct msghdr& msg) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::SENDMSG);
		if (::sendmsg(SocketGet(),& msg, 0)!=-1) return true;
		else SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::SENDMSG);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::SENDMSG);
	}
	return false;
}
bool CRJSocketIPC::SocketAllocIPC(HSOCKET h) {
	SocketAttach(h);
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::ALLOCIPC);
		SocketSetOptionLinger(false, 0);
		return true;
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::ALLOCIPC);
	}
	return false;
}
bool CRJSocketIPC::SocketTimeOut(size_t d) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::SET_OPTION_TIMEOUT);
		struct timeval tv;
		tv.tv_sec=d;
		tv.tv_usec=0;
		if (!::setsockopt(m_hHandle, SOL_SOCKET, SO_RCVTIMEO,(const char*)& tv,sizeof tv)) return true;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::SET_OPTION_TIMEOUT);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::SET_OPTION_TIMEOUT);
	}
	return false;
}
#endif 
#ifdef _rjOS_WIN
CRJSocket& CRJSocketIPC::SocketIPCAlloc(WSAPROTOCOL_INFOW& st) {
	SocketRelease();
	try {
		m_hHandle=::WSASocketW(FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO,& st, 0, WSA_FLAG_OVERLAPPED);
		if (!SocketIsValid(m_hHandle)) SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::IPC_ALLOC);
		else SocketSetOptionLinger(false, 0);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::IPC_ALLOC);
	}
	return *this;
}
bool CRJSocketIPC::SocketIPCDuplicate(DWORD pid, WSAPROTOCOL_INFOW& st) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::IPC_DUPLICATE);
		if (!::WSADuplicateSocketW(m_hHandle, pid,& st)) return true;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::IPC_DUPLICATE);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::IPC_DUPLICATE);
	}
	return false;
}
#endif 

