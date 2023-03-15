#pragma once
#include <recurj/rjframework/libs/net/netsocketevent.h>
namespace rjNet {
	class RJNET_API CRJSocketTCPAccepted:public CRJSocketTCPEventSend {
	public:
		CRJSocketTCPAccepted(IRJSocketHolder&);
		CRJSocketTCPAccepted(CRJSocket&);
		virtual~CRJSocketTCPAccepted();
	};
}

