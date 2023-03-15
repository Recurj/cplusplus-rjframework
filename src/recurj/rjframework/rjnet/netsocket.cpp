#include "stdafx.h"
#include <recurj/rjframework/libs/net/netsocket.h>
#include <recurj/rjframework/libs/base/tasks/task.h>
#include <recurj/rjframework/libs/net/netsend.h>
#include <recurj/rjframework/libs/base/os/hprocess.h>
#include <recurj/rjframework/libs/base/logger/loggerthread.h>
#pragma warning (disable :4548)
using namespace rjNet;
const CRJStringByte CRJSocket::s_clConnected("Connected");
const CRJStringByte CRJSocket::s_clDisconnected("Disconnected");
const CRJStringByte CRJSocket::s_clRecv("Recv");
const CRJStringByte CRJSocket::s_clSend("Try send");
const CRJStringByte CRJSocket::s_clLocalHost("127.0.0.1");
#ifdef _rjOS_FAMILY_UNIX
 const CRJStringByte CRJSocket::s_clSockPath("/var/run/");
#endif 
CRJSocket::CRJSocket(IRJSocketHolder&holder): m_clHolder(holder) {
	m_hHandle=EX_SOCKET_INVALID;
}
CRJSocket::CRJSocket(IRJSocketHolder& holder,HSOCKET h) : m_clHolder(holder) {
	m_hHandle=h;
}
CRJSocket::~CRJSocket() {
	SocketRelease();
}
CRJSocket& CRJSocket::SocketTake(CRJSocket& clSock) {
	if (& clSock!=this) {
		SocketAttach(clSock.SocketDetach());
	}
	return *this;
}
size_t CRJSocket::SocketGetSendDeferredMax() const {
	return cSocketSendDeferredMax;
}
bool CRJSocket::SocketOnError(int64_t lErr, CRJNet::eSocketActions lAction) {
	auto call=[this, lErr, lAction](CRJLogger& clLogger)->void {
		CRJStringByteDefault sInfo;
		SocketGetConnectionInfo(sInfo);
		clLogger.GetWriter().LogPublic(clLogger.Header(CRJDTimeTools::GetDTime()).Cat(sInfo).CatLabel(":Network Error ").CatSize(lErr).CatSingle('[').CatSize(lAction).CatSingle(']'));
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartComm, cLoggerLevelTrace, cb);
	if (SocketIsErrorFatal(lErr)) SocketRelease();
	m_clHolder.OnSocketError(lErr, lAction);
	return false;
}
bool CRJSocket::SocketWarning(int64_t lErr, CRJNet::eSocketActions lAction) {
	auto call=[this, lErr, lAction](CRJLogger& clLogger)->void {
		CRJStringByteDefault sInfo;
		SocketGetConnectionInfo(sInfo);
		clLogger.GetWriter().LogPublic(clLogger.Header(CRJDTimeTools::GetDTime()).Cat(sInfo).CatLabel(":Network warning ").CatSize(lErr).CatSingle('[').CatSize(lAction).CatSingle(']'));
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartComm, cLoggerLevelTraceMiddle, cb);
	m_clHolder.OnSocketWarning(lErr, lAction);
	return false;
}
bool CRJSocket::SocketError(int64_t lErr, CRJNet::eSocketActions lAction) {
	auto call=[lErr, lAction](CRJLogger& clLogger)->void {
		clLogger.CatLabel(":Network Error ").CatSize(lErr).CatSingle('[').CatSize(lAction).CatSingle(']');
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartComm, cLoggerLevelTrace, cb);
	return false;
}
void CRJSocket::SocketInfo(size_t level,const CRJStringByte& s) {
	auto call=[this,& s](CRJLogger& clLogger)->void {
		CRJStringByteDefault sInfo;
		SocketGetConnectionInfo(sInfo);
		clLogger.GetWriter().LogPublic(clLogger.Header(CRJDTimeTools::GetDTime()).Cat(sInfo).Cat(s));
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartComm, level, cb);
}
void CRJSocket::SocketInfoData(size_t level,const CRJStringByte& s, size_t c) {
	auto call=[this,& s, c](CRJLogger& clLogger)->void {
		CRJStringByteDefault sInfo;
		SocketGetConnectionInfo(sInfo);
		clLogger.GetWriter().LogPublic(clLogger.Header(CRJDTimeTools::GetDTime()).Cat(sInfo).Cat(s).CatSize(c));
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartComm, level, cb);
}
int64_t CRJSocket::SocketOnErrorCode(int64_t lErr, CRJNet::eSocketActions lAction) {
	SocketOnError(lErr, lAction);
	return lErr;
}
bool CRJSocket::SocketIsErrorFatal(int64_t lErr) {
#ifdef _rjOS_FAMILY_UNIX
	switch (lErr) {
	case 0:
	case EX_SOCKET_ERROR_EXCEPTION:
	case EX_SOCKET_BAD_OPTIONS:
	case EX_SOCKET_ERROR_BLOCK:
	case EINPROGRESS:
	case EINTR: return false;
	}
#endif 
#ifdef _rjOS_WIN
	switch (lErr) {
	case 0:
	case EX_SOCKET_ERROR_EXCEPTION:
	case EX_SOCKET_ERROR_BLOCK:
	case EX_SOCKET_BAD_OPTIONS:
	case WSAEINPROGRESS:
	case WSAENOBUFS:
	case WSAEINVAL:
	case WSAEHOSTUNREACH: return false;
	}
#endif 
	return true;
}
void CRJSocket::SocketAttach(HSOCKET h) {
	SocketRelease();
	m_hHandle=h;
}
void CRJSocket::SocketRelease() {
	if (!SocketIsValid(m_hHandle)) return ;
	try {
#ifdef _rjOS_FAMILY_UNIX
		::shutdown(m_hHandle, SHUT_RDWR);
		::close(m_hHandle);
#endif 
#ifdef _rjOS_WIN
		::shutdown(m_hHandle, SD_BOTH);
		::closesocket(m_hHandle);
#endif 
	}
	catch(...) {}
	m_hHandle=EX_SOCKET_INVALID;
}
void CRJSocket::SocketClose() {
	try {
		if (!SocketIsValid(m_hHandle)) SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::CLOSE);
		else {
#ifdef _rjOS_FAMILY_UNIX
			::close(m_hHandle);
#endif 
#ifdef _rjOS_WIN
			::closesocket(m_hHandle);
#endif 
		}
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::CLOSE);
	}
	m_hHandle=EX_SOCKET_INVALID;
}
bool CRJSocket::SocketBind(size_t &lPort,const CRJStringByte& clLocalHost) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::BIND_LOCAL_ADDRESS);
		struct sockaddr_in sin;
		int32_t b=1L;
		_MEMCLR_BYTE(sin);
		if (clLocalHost.GetLength()>0) {
			if (!SocketFindRemoteHostByName(clLocalHost, sin.sin_addr)) return false;
		}
		else sin.sin_addr.s_addr=INADDR_ANY;
		sin.sin_port=htons((uint16_t) lPort);
		sin.sin_family=AF_INET;
		if (::setsockopt(m_hHandle, SOL_SOCKET, SO_REUSEADDR,(char*)& b,sizeof(b))) SocketWarning(CRJNet::GetNetLastError(), CRJNet::eSocketActions::BIND_LISTEN);
		if (!::bind(m_hHandle, (struct sockaddr*)&sin, sizeof(sin))) {
			if (lPort == 0) {
				SRJSocketPartInfo st;
				SocketGetConnectionLocalInfo(st);
				lPort = st.m_sPort;

			}
			return true;
		}
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::BIND_LOCAL_ADDRESS);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::BIND_LOCAL_ADDRESS);
	}
	return false;
}
bool CRJSocket::SocketConnect(size_t lPort,const CRJStringByte& clHost) {
	if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::CONNECT);
	else if (lPort<=0) return SocketOnError(EX_SOCKET_BAD_OPTIONS, CRJNet::eSocketActions::CONNECT);
	else if (!clHost.GetLength()) return SocketOnError(EX_SOCKET_BAD_OPTIONS, CRJNet::eSocketActions::CONNECT);
	struct sockaddr_in sin;
	_MEMCLR_BYTE(sin);
	if (!SocketFindRemoteHostByName(clHost, sin.sin_addr)) return SocketOnError(EX_SOCKET_BAD_OPTIONS, CRJNet::eSocketActions::CONNECT);
	sin.sin_family=AF_INET;
	sin.sin_port=htons((uint16_t) lPort);
	return SocketConnect(sin);
}
bool CRJSocket::SocketConnect(
struct sockaddr_in& s) {
	try {
#ifdef _rjOS_FAMILY_UNIX
		socklen_t l=sizeof(s);
#endif 
#ifdef _rjOS_WIN
		int32_t l=(int64_t)sizeof(s);
#endif 
		if (!::connect(m_hHandle,(struct sockaddr*)& s, l)) return true;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::CONNECT);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::CONNECT);
	}
	return false;
}
bool CRJSocket::SocketReadAvailable(CRJStringByteVar& s,const SRJSocketProcessMetrics& stMetrics) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::READ);
		size_t l=0;
		auto lRetry=stMetrics.lRetry;
		while (1) {
			if (SocketReadIsWaiting(l)) {
				if (!s.HasNeededSize(l)) l=s.GetSize();
				s.SetLength(l);
				return SocketReadBlock(s.GetHeadForUpdate(), l)&&
					l==s.GetLength();
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
bool CRJSocket::SocketReadNeeded(uint8_t* bData, size_t lLen,const SRJSocketProcessMetrics& stMetrics, size_t& lTotalRead) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::READ);
		size_t l=0;
		auto lRetry=stMetrics.lRetry;
		while (1) {
			if (SocketReadIsWaiting(l)&&
				l>=lLen) {
				lTotalRead=lLen;
				return SocketReadBlock(bData, lTotalRead)&&
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
bool CRJSocket::SocketReadBlock(uint8_t* pBuf, size_t& l) {
	try {
		int32_t lRead=::recv(m_hHandle,(char*) pBuf,static_cast<int32_t>(l), 0);
		if (lRead<0) return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::READ);
		else if (lRead>0) {
			l=static_cast<size_t>(lRead);
			return true;
		}
		return SocketOnError(EX_SOCKET_ERROR_NOT_CONNECT, CRJNet::eSocketActions::READ);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::READ);
	}
	return false;
}
bool CRJSocket::SocketReadIsWaiting(size_t& r) {
	if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::READ);
	u_long l=0L;
#ifdef _rjOS_FAMILY_UNIX
	if (!ioctl(m_hHandle, FIONREAD,& l)) {
		r=(size_t) l;
		return true;
	}
#endif 
#ifdef _rjOS_WIN
	if (!::ioctlsocket(m_hHandle, FIONREAD,& l)) {
		r=(size_t) l;
		return true;
	}
#endif 
	return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::READ);
}
bool CRJSocket::OnSocketSendNeedWait(const SRJSocketProcessMetrics& st) {
	CRJTask::ThreadSleep(st.dwWait);
	return true;
}
bool CRJSocket::OnSocketReadNeedWait(const SRJSocketProcessMetrics& st) {
	CRJTask::ThreadSleep(st.dwWait);
	return true;
}
bool CRJSocket::SocketSend(CRJStringByteFlex& block, CRJNetSend& sockSend) {
	try {
		if (!SocketIsValid(m_hHandle)) return sockSend.SetError(EX_SOCKET_ERROR_NOT_EXIST);
		auto lRetry=sockSend.GetMetrics().lRetry;
		do {
			if (!sockSend.SendResult(block,static_cast<int64_t>(::send(m_hHandle,block.GetHeadForReadC(), (int32_t)block.GetLength(), 0)))) return false;
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
int64_t CRJSocket::SocketSetOptionReUseAddr(bool bReUse) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::SET_OPTION_USE_ADDR);
		int32_t b=bReUse& 1;
		if (!::setsockopt(m_hHandle, SOL_SOCKET, SO_REUSEADDR,(char*)& b,sizeof(b))) return 0;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::SET_OPTION_USE_ADDR);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::SET_OPTION_USE_ADDR);
	}
	return EX_SOCKET_ERROR_EXCEPTION;
}
int64_t CRJSocket::SocketSetOptionLinger(bool bLinger, int64_t lTimeout) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::SET_OPTION_TOTAL_LINGER);
		linger stLinger;
		stLinger.l_onoff=(u_short)(bLinger& 1);
		stLinger.l_linger=(u_short) lTimeout;
		if (!::setsockopt(m_hHandle, SOL_SOCKET, SO_LINGER,(char*)& stLinger,sizeof(stLinger))) return 0;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::SET_OPTION_TOTAL_LINGER);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::SET_OPTION_TOTAL_LINGER);
	}
	return EX_SOCKET_ERROR_EXCEPTION;
}
int64_t CRJSocket::SocketSetOptionSendBufferSize(size_t lSize) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::SET_OPTION_SET_SEND_BUFFER);
		if (!::setsockopt(m_hHandle, SOL_SOCKET, SO_SNDBUF,(char*)& lSize,sizeof(lSize))) return 0;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::SET_OPTION_SET_SEND_BUFFER);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::SET_OPTION_SET_SEND_BUFFER);
	}
	return EX_SOCKET_ERROR_EXCEPTION;
}
int64_t CRJSocket::SocketSetOptionRecvBufferSize(size_t lSize) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::SET_OPTION_SET_RECV_BUFFER);
		if (!::setsockopt(m_hHandle, SOL_SOCKET, SO_RCVBUF,(char*)& lSize,sizeof(lSize))) return 0;
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::SET_OPTION_SET_RECV_BUFFER);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::SET_OPTION_SET_RECV_BUFFER);
	}
	return EX_SOCKET_ERROR_EXCEPTION;
}
int64_t CRJSocket::SocketSetOptionBlockMode(bool bBlockMode) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::SET_OPTION_NON_BLOCK_MODE);
#ifdef _rjOS_FAMILY_UNIX
		int64_t opts=fcntl(m_hHandle, F_GETFL);
		if (bBlockMode) opts&=(~O_NONBLOCK);
		else opts|=O_NONBLOCK;
		if (!fcntl(m_hHandle, F_SETFL, opts)) return 0;
#endif 
#ifdef _rjOS_WIN
		u_long f=(u_long)((bBlockMode)?0:1);
		if (!::ioctlsocket(m_hHandle, FIONBIO,& f)) return 0;
#endif 
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::SET_OPTION_NON_BLOCK_MODE);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::SET_OPTION_NON_BLOCK_MODE);
	}
	return EX_SOCKET_ERROR_EXCEPTION;
}
int64_t CRJSocket::SocketGetError() {
	int lErr=EX_SOCKET_ERROR_EXCEPTION;
	try {
#ifdef _rjOS_FAMILY_UNIX
		socklen_t errlen=sizeof(lErr);
		if (!getsockopt(m_hHandle, SOL_SOCKET, SO_ERROR,(void*)& lErr,& errlen)) lErr=errno;
#endif 
#ifdef _rjOS_WIN
		int errlen=sizeof(lErr);
		if (getsockopt(m_hHandle, SOL_SOCKET, SO_ERROR,(char*)& lErr,& errlen)) lErr=::WSAGetLastError();
#endif 
	}
	catch(...) {}
	return (int64_t) lErr;
}
bool CRJSocket::SocketIsConnected() const {
	try {
		if (SocketIsValid(m_hHandle)) {
			struct sockaddr_in sin;
#ifdef _rjOS_FAMILY_UNIX
			socklen_t l=sizeof(sin);
#endif 
#ifdef _rjOS_WIN
			int32_t l=static_cast<int32_t>(sizeof(sin));
#endif 
			_MEMCLR_BYTE(sin);
			if (!::getpeername(m_hHandle,(sockaddr*)& sin,& l)) return true;
		}
	}
	catch(...) {}
	return false;
}
CRJStringByteVar& SRJSocketPartInfo::AddrToString(CRJStringByteVar& s) const {
	const uint16_t* p=m_uAddress;
	uint8_t i=m_sCount;
	s.SetEmpty();
	while (i>0) {
		i--;
		s.CatSize(* p).CatSingle('.');
		p++;
	}
	return s.TruncRight(1);
}
void SRJSocketPartInfo::Reset() {
	m_bDefined=0;
	m_sCount=0;
	m_sPort=0;
}
CRJStringByteVar& SRJSocketPartInfo::ToString(CRJStringByteVar& s) const {
	if (m_sCount>0) return AddrToString(s).SetLast('[').CatSize(m_sPort).CatSingle(']');
	return s.SetSingle('?');
}
void SRJSocketEndpoint::Reset() {
	m_stLocal.Reset();
	m_stRemote.Reset();
}
CRJStringByteVar& SRJSocketEndpoint::ToString(CRJStringByteVar& s) const {
	if (m_stLocal.m_bDefined) m_stLocal.ToString(s.CatLabel("(L)", 3));
	else s.CatLabel("(L)***", 6);
	if (m_stRemote.m_bDefined) m_stRemote.ToString(s.CatLabel("(R)", 3));
	else s.CatLabel("(R)***", 6);
	return s;
}
bool CRJSocket::SocketGetConnectionInfo(CRJStringByteVar& s) {
	SRJSocketEndpoint st;
	if (SocketGetConnectionInfo(st)) return st.ToString(s).OK();
	else s.SetSingle('?');
	return false;
}
bool CRJSocket::SocketGetConnectionInfo(SRJSocketEndpoint& st) {
	return SocketGetConnectionRemoteInfo(st.m_stRemote)&&SocketGetConnectionLocalInfo(st.m_stLocal);
}
bool CRJSocket::SocketGetConnectionRemoteInfo(SRJSocketPartInfo& st) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::GET_REMOTE_INFO);
		struct sockaddr_in sin;
#ifdef _rjOS_FAMILY_UNIX
		socklen_t l=sizeof(sin);
#endif 
#ifdef _rjOS_WIN
		int32_t l=static_cast<int32_t>(sizeof(sin));
#endif 
		_MEMCLR_BYTE(sin);
		if (::getpeername(m_hHandle,(sockaddr*)& sin,& l)) {
			int64_t lErr=CRJNet::GetNetLastError();
			;
			st.Reset();
			if (lErr!=EX_SOCKET_ERROR_NOT_CONNECT) return SocketOnError(lErr, CRJNet::eSocketActions::GET_REMOTE_INFO);
		}
		else {
			SocketSin2Info(sin, st);
			return true;
		}
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::GET_REMOTE_INFO);
	}
	return false;
}
bool CRJSocket::SocketGetConnectionLocalInfo(SRJSocketPartInfo& st) {
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::GET_LOCAL_INFO);
		struct sockaddr_in sin;
#ifdef _rjOS_FAMILY_UNIX
		socklen_t l=sizeof(sin);
#endif 
#ifdef _rjOS_WIN
		int32_t l=static_cast<int32_t>(sizeof(sin));
#endif 
		_MEMCLR_BYTE(sin);
		if (::getsockname(m_hHandle,(sockaddr*)& sin,& l)) {
			st.Reset();
			SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::GET_LOCAL_INFO);
		}
		else {
			SocketSin2Info(sin, st);
			return true;
		}
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::GET_LOCAL_INFO);
	}
	return false;
}
bool CRJSocket::SocketFindRemoteHostByName(const CRJStringByte& clName, in_addr& st) {
	try {
		char tmp[256];
		size_t lLen=clName.GetLength();
		if ((lLen>0)&&
			(lLen<(sizeof(tmp)-1))) {
			struct hostent* pHost;
			_MEMMOVE_BYTE_POINTER(tmp, clName.GetHeadForRead(), lLen);
			tmp[lLen]=0x00;
			pHost=::gethostbyname(tmp);
			if (!pHost) {
				pHost=::gethostbyaddr(tmp,(int32_t) lLen, AF_INET);
				if (!pHost) return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::FIND_REMOTE_HOST);
			}
			if (pHost->h_length>0) {
				lLen=(pHost->h_length>sizeof(st))?
					sizeof(st):
					pHost->h_length;
				_MEMMOVE_BYTE_POINTER(& st, pHost->h_addr, lLen);
				return true;
			}
		}
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::FIND_REMOTE_HOST);
	}
	return false;
}
bool CRJSocket::SocketGetHostName(CRJStringByteVar& clName) {
	size_t s=clName.GetSize();
	if (s<2) return SocketError(EX_SOCKET_BAD_OPTIONS, CRJNet::eSocketActions::FIND_LOCAL_HOST);
#ifdef _rjOS_WIN
	if (!::gethostname((char*) clName.GetHeadForUpdate(),(int)(clName.GetSize()-1))) {
		clName.CalcCLength();
		return true;
	}
#else 
	if (!::gethostname((char*) clName.GetHeadForUpdate(), clName.GetSize()-1)) {
		clName.CalcCLength();
		return true;
	}
#endif 
	return SocketError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::FIND_LOCAL_HOST);
}
bool CRJSocket::SocketWaitRead(bool& bHas) {
	bHas=false;
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::WAIT_READ);
		fd_set act, err;
		int64_t lResult;
		FD_ZERO(& act);
		FD_ZERO(& err);
		FD_SET(m_hHandle,& act);
		FD_SET(m_hHandle,& err);
		lResult=select((int) m_hHandle+1,& act,nullptr,& err,nullptr);
		if (!lResult) return true;
		else if (lResult>0) {
			if (FD_ISSET(m_hHandle,& act)) bHas=true;
			return true;
		}
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::WAIT_READ);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::WAIT_READ);
	}
	return false;
}
bool CRJSocket::SocketWaitRead(int64_t lTimeout,bool& bHas,bool& bTimeout) {
	bTimeout=false;
	bHas=false;
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::WAIT_READ);
		fd_set act, err;
		int64_t lResult;
		struct timeval stTimeout;
		stTimeout.tv_sec=(long) lTimeout;
		stTimeout.tv_usec=0;
		FD_ZERO(& act);
		FD_ZERO(& err);
		FD_SET(m_hHandle,& act);
		FD_SET(m_hHandle,& err);
		lResult=select((int) m_hHandle+1,& act,nullptr,& err,& stTimeout);
		if (!lResult) {
			bTimeout=true;
			return true;
		}
		else if (lResult>0) {
			if (FD_ISSET(m_hHandle,& act)) bHas=true;
			return true;
		}
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::WAIT_READ);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::WAIT_READ);
	}
	return false;
}
bool CRJSocket::SocketWaitSend(int64_t lTimeout,bool& bHas,bool& bTimeout) {
	bTimeout=false;
	bHas=false;
	try {
		if (!SocketIsValid(m_hHandle)) return SocketOnError(EX_SOCKET_ERROR_NOT_EXIST, CRJNet::eSocketActions::WAIT_SEND);
		fd_set act, err;
		int64_t lResult;
		struct timeval stTimeout;
		stTimeout.tv_sec=(long) lTimeout;
		stTimeout.tv_usec=0;
		FD_ZERO(& act);
		FD_ZERO(& err);
		FD_SET(m_hHandle,& act);
		FD_SET(m_hHandle,& err);
		lResult=select((int) m_hHandle+1,nullptr,& act,& err,& stTimeout);
		if (!lResult) {
			bTimeout=true;
			return true;
		}
		else if (lResult>0) {
			if (FD_ISSET(m_hHandle,& act)) bHas=true;
			return true;
		}
		return SocketOnError(CRJNet::GetNetLastError(), CRJNet::eSocketActions::WAIT_SEND);
	}
	catch(...) {
		SocketOnError(EX_SOCKET_ERROR_EXCEPTION, CRJNet::eSocketActions::WAIT_SEND);
	}
	return false;
}
int64_t CRJSocket::SocketPrepareMapRead(fd_set& stRecv, fd_set& stErr) {
	FD_SET(m_hHandle,& stRecv);
	FD_SET(m_hHandle,& stErr);
	return (int64_t) m_hHandle;
}
bool CRJSocket::SocketExistInMap(fd_set& st) {
	return FD_ISSET(m_hHandle,& st)!=0;
}
void CRJSocket::SocketSin2Info(
struct sockaddr_in& sin, SRJSocketPartInfo& st) {
	st.m_bDefined=1;
	st.m_sCount=4;
	st.m_sPort=::ntohs(sin.sin_port);
#ifdef _rjOS_FAMILY_UNIX
	st.m_uAddress[0]=(sin.sin_addr.s_addr>>24)& 0xFF;
	st.m_uAddress[1]=(sin.sin_addr.s_addr>>16)& 0xFF;
	st.m_uAddress[2]=(sin.sin_addr.s_addr>>8)& 0xFF;
	st.m_uAddress[3]=(sin.sin_addr.s_addr)& 0xFF;
#endif 
#ifdef _rjOS_WIN
	st.m_uAddress[0]=sin.sin_addr.S_un.S_un_b.s_b1;
	st.m_uAddress[1]=sin.sin_addr.S_un.S_un_b.s_b2;
	st.m_uAddress[2]=sin.sin_addr.S_un.S_un_b.s_b3;
	st.m_uAddress[3]=sin.sin_addr.S_un.S_un_b.s_b4;
#endif 
}
SRJNetSocketEndPoint::SRJNetSocketEndPoint():
	m_sPort(0) {}
void SRJNetSocketEndPoint::operator=(const SRJNetSocketEndPoint& p) {
	m_sPort=p.m_sPort;
	m_clHost.Set(p.m_clHost);
}
bool SRJNetSocketEndPoint::IsDefined() const {
	return m_sPort>0&&
		m_clHost.IsNonEmpty();
}

