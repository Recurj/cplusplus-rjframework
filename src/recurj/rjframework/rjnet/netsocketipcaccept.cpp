#include "stdafx.h"
#include <recurj/rjframework/libs/net/ipc/netsocketipcaccept.h>
#include <recurj/rjframework/errorcodes.h>
using namespace rjNet;
CRJNetSocketIPCAccept::CRJNetSocketIPCAccept() {}
CRJNetSocketIPCAccept::~CRJNetSocketIPCAccept() {}
bool CRJNetSocketIPCAccept::Read(CRJSocket& clSock) {
#ifdef _rjOS_FAMILY_UNIX
	struct msghdr msg;
	struct iovec iov;
	_MEMCLR_BYTE(msg);
	iov.iov_base=& m_lId;
	iov.iov_len=sizeof(m_lId);
#ifdef _rjOS_LINUX
	union {
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	}
	control_un;
	msg.msg_control=control_un.control;
	msg.msg_controllen=sizeof(control_un.control);
#else 
	msg.msg_accrights=(caddr_t)& m_stInfo;
	msg.msg_accrightslen=sizeof(int);
#endif 
	msg.msg_name=nullptr;
	msg.msg_namelen=0;
	msg.msg_iov=& iov;
	msg.msg_iovlen=1;
	if (clSock.SocketMsgRead(msg, 60)) {
#ifdef _rjOS_LINUX
		struct cmsghdr* cmptr=CMSG_FIRSTHDR(& msg);
		if (cmptr!=nullptr&&
			cmptr->cmsg_len==SIZE_CMSG(sizeof(int))&&
			cmptr->cmsg_level==SOL_SOCKET&&
			cmptr->cmsg_type==SCM_RIGHTS) {
			m_stInfo=(*((int*) CMSG_DATA(cmptr)));
			return ReadParams(clSock);
		}
#else 
		return msg.msg_accrightslen==sizeof(int);
#endif 
	}
#endif 
#ifdef _rjOS_WIN
	size_t s;
	if (clSock.SocketReadNeeded((uint8_t*)& m_stInfo,sizeof(m_stInfo), SRJSocketProcessMetrics::s_stSendMetrics, s)) return ReadParams(clSock);
	else CRJLoggerThread::ErrorOs(EErrorOs::AppErrSendMsg);
#endif 
	return false;
}
bool CRJNetSocketIPCAccept::Load(CRJSocket& clSock) {
	CRJSocketIPC clSockIPC(clSock.SocketHolder());
#ifdef _rjOS_FAMILY_UNIX
	bool b=clSockIPC.SocketAllocIPC(m_stInfo);
#endif 
#ifdef _rjOS_WIN
	bool b=clSockIPC.SocketIPCAlloc(m_stInfo.wsa).SocketIsValid();
#endif 
	if (b) {
		clSock.SocketTake(clSockIPC);
	}
	return b;
}
bool CRJNetSocketIPCAccept::ReadParams(CRJSocket& clSock) {
	size_t s;
	uint8_t b;
	if (clSock.SocketReadNeeded((uint8_t*)& b, 1, SRJSocketProcessMetrics::s_stSendMetrics, s)) {
		size_t l=b;
		if (l<cSizeChainDefault) {
			if (l==0||
				clSock.SocketReadNeeded(m_clParam.GetHeadForUpdate(), l, SRJSocketProcessMetrics::s_stSendMetrics, s)) {
				m_clParam.SetLengthSafed(l);
				return true;
			}
		}
	}
	return false;
}

