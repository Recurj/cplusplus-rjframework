#include "stdafx.h"
#include <recurj/rjframework/libs/net/ipc/netsocketipctransmit.h>
#include <recurj/rjframework/libs/net/netsend.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/errorcodes.h>
using namespace rjNet;
CRJNetSocketIPCTransmit::CRJNetSocketIPCTransmit() {}
CRJNetSocketIPCTransmit::~CRJNetSocketIPCTransmit() {}
bool CRJNetSocketIPCTransmit::Load(CRJSocket& clSock, uint64_t pid) {
#ifdef  _rjOS_FAMILY_UNIX
	m_stInfo=clSock;
	return true;
#endif 
#ifdef _rjOS_WIN
	CRJSocketIPC clSockIPC(clSock.SocketHolder());
	if (clSockIPC.SocketIPCDuplicate((DWORD) pid, m_stInfo.wsa)) {
		clSock.SocketTake(clSockIPC);
		return true;
	}
	return false;
#endif 
}
bool CRJNetSocketIPCTransmit::Send(CRJSocket& clSock, uint64_t id,const CRJStringByte& params) {
#ifdef  _rjOS_FAMILY_UNIX
	struct msghdr msg;
	struct iovec iov;
	_MEMCLR_BYTE(msg);
	iov.iov_base=& id;
	iov.iov_len=sizeof(id);
#ifdef _rjOS_LINUX
	union {
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	}
	control_un;
	struct cmsghdr* cmptr;
	msg.msg_control=control_un.control;
	msg.msg_controllen=sizeof(control_un.control);
	cmptr=CMSG_FIRSTHDR(& msg);
	cmptr->cmsg_len=SIZE_CMSG(sizeof(int));
	cmptr->cmsg_level=SOL_SOCKET;
	cmptr->cmsg_type=SCM_RIGHTS;
	*((int*) CMSG_DATA(cmptr))=m_stInfo;
#else 
	msg.msg_accrights=(caddr_t)& m_stInfo;
	msg.msg_accrightslen=sizeof(int);
#endif 
	msg.msg_name=nullptr;
	msg.msg_namelen=0;
	msg.msg_iov=& iov;
	msg.msg_iovlen=1;
	if (clSock.SocketMsgSend(msg)) return SendParams(clSock, params);
	else CRJLoggerThread::ErrorOs(EErrorOs::AppErrSendMsg);
#endif 
#ifdef _rjOS_WIN
	CRJNetSend sockSend(clSock);
	m_stInfo.id=id;
	if (sockSend.SendData((uint8_t*)& m_stInfo,sizeof(m_stInfo))) return SendParams(clSock, params);
	else CRJLoggerThread::ErrorOs(EErrorOs::AppErrSendMsg);
#endif 
	return false;
}
bool CRJNetSocketIPCTransmit::SendParams(CRJSocket& clSock,const CRJStringByte& params) {
	size_t l=params.GetLength();
	if (l>0) {
		CRJNetSend sockSend(clSock);
		CRJStringByteVarBuffer<cSizeChainDefault+1>send;
		return sockSend.SendData(send.SetSingle((uint8_t) l).Cat(params));
	}
	return true;
}

