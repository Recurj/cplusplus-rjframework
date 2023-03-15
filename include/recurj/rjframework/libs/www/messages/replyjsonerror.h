#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/www/messages/reply.h>
#include <recurj/rjframework/libs/www/http/replyjson.h>
namespace rjWWW {
	class RJWWW_API CMessageHTTPReplyJSONError:public CMessageHTTPReply {
	public:
		static CMessageHTTPReplyJSONError* Get(int64_t,const SRJTaskTicket&,const CRJStringByte&);
		static void JSONError(int64_t,const SRJTaskTicket&,const CRJStringByte&);
		virtual~CMessageHTTPReplyJSONError();
		virtual CHTTPReply& GetReply() {
			return m_clReply;
		};
		void*operator new(size_t);
		void operator delete(void*);
		CHTTPReplyJSON& GetJSONReply() {
			return m_clReply;
		};
		const CHTTP::CLang& GetLang() {
			return m_clLang;
		}
	protected:
		CMessageHTTPReplyJSONError(int64_t,const SRJTaskTicket&,const CRJStringByte&);
	protected:
		static CRJAllocatorRuntime m_clHeap;
		CHTTP::CLang m_clLang;
		CHTTPReplyJSON m_clReply;
	};
}

