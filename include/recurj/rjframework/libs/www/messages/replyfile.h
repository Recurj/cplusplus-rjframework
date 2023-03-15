#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <recurj/rjframework/libs/www/messages/reply.h>
#include <recurj/rjframework/libs/www/http/replyfile.h>
namespace rjWWW {
	class RJWWW_API CMessageHTTPReplyFile:public CMessageHTTPReply {
	public:
		static CMessageHTTPReplyFile* Get(size_t,const SRJTaskTicket&,const CRJStringByte&);
		virtual~CMessageHTTPReplyFile();
		virtual CHTTPReply& GetReply() {
			return m_clReply;
		}
		void operator delete(void*);
		const CRJStringByte& GetName() const {
			return m_clFileName;
		}
	protected:
		CMessageHTTPReplyFile(size_t,const SRJTaskTicket&,const CRJStringByte&);
		void*operator new(size_t);
	protected:
		static CRJAllocatorRuntime m_clHeap;
		CRJStringByteExpanded m_clFileName;
		CHTTPReply m_clReply;
	};
}

