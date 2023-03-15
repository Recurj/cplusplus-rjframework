#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/strings/native.h>
#include <recurj/rjframework/libs/base/messages/message.h>
namespace rjBase {
	class RJBASE_API CRJMessageLog:public CRJMessage {
	public:
		static CRJMessageLog* Get(const CRJStringByte&);
		virtual~CRJMessageLog();
		bool Log(CRJLogger&) override;
		void*operator new(size_t);
		void operator delete(void*);
	protected:
		CRJMessageLog(const CRJStringByte&);
	protected:
		static CRJAllocatorRuntime m_clHeap;
		CRJStringByteExpanded m_clData;
	};
}

