#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/messages/message.h>
#include <recurj/rjframework/libs/base/tasks/task.h>
#include <recurj/rjframework/libs/net/netsocket.h>
#include <recurj/rjframework/libs/net/netsessionmetrics.h>
namespace rjNet {
	class RJNET_API CRJMessageNetSession:public CRJMessage {
		const size_t NetSessionTransmitMaxTry=3;
	public:
		CRJMessageNetSession(CRJSocket&,const CRJStringByte&);
		virtual~CRJMessageNetSession();
		void*operator new(size_t);
		void operator delete(void*);
		bool NextTry();
		CRJSocket m_clSock;
		SRJTaskTicket m_stTicket;
		SUNetSessionMetrics m_stMetric;
		CRJStringByteDefault m_clParam;
		size_t m_lTrySend;
	protected:
		static CRJAllocatorRuntime m_clHeap;
	};
}

