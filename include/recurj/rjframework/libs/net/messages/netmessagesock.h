#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/messages/message.h>
#include <recurj/rjframework/libs/net/netsocket.h>
#include <recurj/rjframework/modules/modnet.h>
namespace rjNet {
	class RJNET_API CRJMessageSocket:public CRJMessage {
	public:
		CRJMessageSocket(size_t, CRJSocket&);
		virtual~CRJMessageSocket();
		CRJSocket& GetSock() {
			return m_clSock;
		}
		void CloseSock();
		static size_t PreAllocate(size_t c) {
			return m_clHeap.PreAllocate(c,sizeof(CRJMessageSocket));
		}
		void*operator new(size_t);
		void operator delete(void*);
	protected:
		static CRJAllocatorRuntime m_clHeap;
		CRJSocket m_clSock;
	};
}

