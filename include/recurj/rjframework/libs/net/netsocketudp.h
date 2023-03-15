#pragma once
#include <recurj/rjframework/libs/net/netsocket.h>
#include <recurj/rjframework/libs/net/netsend.h>
namespace rjNet {
	class RJNET_API CRJSocketUDP:public CRJSocket {
	public:
		CRJSocketUDP(IRJSocketHolder&);
		virtual~CRJSocketUDP();
		CRJSocketUDP& SocketUDPAlloc();
		bool SocketUDPBindServer(size_t,
		struct sockaddr_in&);
		bool SocketUDPBindServerLocal(size_t,
		struct sockaddr_in&);
		bool SocketUDPBindClient();
		bool SocketUDPRead(uint8_t*, size_t,const SRJSocketProcessMetrics&, size_t&);
		bool SocketUDPReadBlock(uint8_t*, size_t&);
		bool SocketUDPSend(CRJStringByteFlex&, CRJNetSend&,
		struct sockaddr_in&);
	};
}

