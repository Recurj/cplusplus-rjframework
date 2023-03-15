#pragma once
#include <recurj/rjframework/libs/net/netsocket.h>
namespace rjNet {
	class RJNET_API CRJSocketIPC:public CRJSocket {
	public:
		CRJSocketIPC(IRJSocketHolder&);
		virtual~CRJSocketIPC();
#ifdef _rjOS_FAMILY_UNIX
		CRJSocket& SocketIPCAlloc();
		bool SocketUnixConnect(const CRJString&);
		bool SocketUnixListen(const CRJString&);
		bool SocketUnixAccept(CRJSocket&);
		bool SocketMsgRead(
		struct msghdr&, size_t);
		bool SocketMsgSend(const 
		struct msghdr&);
		bool SocketAllocIPC(HSOCKET);
		bool SocketTimeOut(size_t);
#endif 
#ifdef _rjOS_WIN
		CRJSocket& SocketIPCAlloc(WSAPROTOCOL_INFOW&);
		bool SocketIPCDuplicate(DWORD, WSAPROTOCOL_INFOW&);
#endif 
	};
}

