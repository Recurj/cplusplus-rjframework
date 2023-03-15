#pragma once
#include <recurj/rjframework/libs/net/netsocket.h>
namespace rjNet {
	class CRJNetSocketTCPAccepted;
	class RJNET_API CRJSocketTCP:public CRJSocket {
	public:
		CRJSocketTCP(IRJSocketHolder&);
		virtual~CRJSocketTCP();
		CRJSocketTCP& SocketTCPAlloc();
		bool SocketTCPListen(size_t lCount=5);
		bool SocketTCPAccept(CRJNetSocketTCPAccepted&,bool);
		bool SocketTCPConnect(size_t,const CRJStringByte&,bool);
		bool SocketTCPReadExisting(CRJStringByteVar&);
		bool SocketTCPReadAvailable(CRJStringByteVar&);
		int64_t SocketTCPSetOptionNoDelay(bool);
		int64_t SocketTCPSetOptionKeepAlive(bool);
	};
	class RJNET_API CRJNetSocketTCPAccepted:public CRJSocketTCP {
	public:
		CRJNetSocketTCPAccepted(IRJSocketHolder& holder) : CRJSocketTCP(holder) {}
		virtual~CRJNetSocketTCPAccepted() {}
		void SocketTCPOnAccepted(HSOCKET,bool);
	};
}

