#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/messages/message.h>
#include <recurj/rjframework/libs/base/tasks/task.h>
#include <recurj/rjframework/libs/www/http/reply.h>
namespace rjWWW {
	class RJWWW_API CMessageHTTPReply:public CRJMessage {
	public:
		virtual CHTTPReply& GetReply()=0;
		const SRJTaskTicket& GetTaskTicket() const {
			return m_stNetPost;
		}
	protected:
		CMessageHTTPReply(size_t,const SRJTaskTicket&);
		virtual~CMessageHTTPReply();
	protected:
		SRJTaskTicket m_stNetPost;
	};
}

