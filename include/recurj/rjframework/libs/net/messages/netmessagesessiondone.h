#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/messages/message.h>
#include <recurj/rjframework/modules/modnet.h>
namespace rjNet {
	class RJNET_API CRJMessageNetSessionDone:public CRJMessage {
	public:
		static CRJMessageNetSessionDone* Get(size_t);
		CRJMessageNetSessionDone(size_t);
		virtual~CRJMessageNetSessionDone();
		void*operator new(size_t);
		void operator delete(void*);
		size_t m_lId;
	protected:
		static CRJAllocatorRuntime m_clHeap;
	};
}

