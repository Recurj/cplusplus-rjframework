#pragma once
#include <mutex>
#include <recurj/rjframework/libs/base/datatools/atomicnumber.h>
#include <recurj/rjframework/libs/base/messages/list.h>
#include <recurj/rjframework/libs/base/os/hevent.h>
namespace rjBase {
	class RJBASE_API CRJQueueMessages :public RJEvent {
	public:
		struct SPop {
			size_t m_sCount;
			CRJMessage* m_pTop;

		};
		CRJQueueMessages()noexcept;
		virtual~CRJQueueMessages();
		virtual bool QueueStart();
		virtual void QueueStop();
		bool QueueCheck();
		void QueueClear();
		void QueueReset();
		bool QueueBot(CRJMessage&);
		bool QueueBotRestricted(CRJMessage&, size_t);
		bool QueueBot(CRJListMessages&);
		bool QueueTop(CRJMessage&);
		bool QueueTop(CRJListMessages&);
		CRJMessage* QueuePop();
		SPop& QueueMove2Cache(SPop&);
		bool IsQueueEmpty();
		size_t GetQueueCounts();
		bool QueueSet();
		inline void QueueCloseLatch() {	m_clQueueLatch.latchClose();}
		inline void QueueOpenLatch() {	m_clQueueLatch.latchOpen();	}
		inline int64_t IsQueueLatchNotClosed() const {return m_clQueueLatch.anGet();}
	protected:
		void npOnQueueEmptyList(SPop&);
	private:
		bool npQueueStart();
		void npQueueStop();
		void npQueueClear();
	protected:
		std::mutex m_clQueueCritical;
		CRJLatch m_clQueueLatch;
		CRJListMessages m_clQueueList;
	};

}