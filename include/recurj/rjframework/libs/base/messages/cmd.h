#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/messages/message.h>
namespace rjBase {
	class RJBASE_API CRJMessageCmd:public CRJMessage {
	public:
		static CRJMessageCmd* Get(size_t, size_t);
		virtual~CRJMessageCmd();
		void*operator new(size_t);
		void operator delete(void*);
	protected:
		CRJMessageCmd(size_t, size_t);
		static CRJAllocatorRuntime m_clHeap;
	};
}

