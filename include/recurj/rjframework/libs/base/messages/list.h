#pragma once
#include <recurj/rjframework/libs/base/messages/message.h>
namespace rjBase {
	class CRJQueueMessages;
	class RJBASE_API CRJListMessages: public CRJListBase<CRJMessage> {
	public:
		CRJListMessages()noexcept;
		virtual~CRJListMessages();
		bool QueueBot(CRJQueueMessages&);
	protected:
		void freeElem(CRJMessage*p) override;
	};
}

