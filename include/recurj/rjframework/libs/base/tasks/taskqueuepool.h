#pragma once
#include <recurj/rjframework/libs/base/os/hevent.h>
#include <recurj/rjframework/libs/base/tasks/taskqueue.h>
namespace rjBase {
	constexpr size_t CRJOSPollElemCount=cRJOSPollMax-2;
	constexpr size_t CRJOSPollElemCountBytes=(CRJOSPollElemCount/ 8+1);
	class RJBASE_API CRJTaskQueuePool:public CRJTaskQueueVariant<cRJOSPollMax> {
	public:
		CRJTaskQueuePool();
		virtual~CRJTaskQueuePool();
		bool Insert(size_t, RJEvent&);
		bool Remove(size_t, RJEvent&);
		inline bool HasSpace() const {
			return m_clListener.HasSpace();
		}
	protected:
		virtual bool OnWaitEventElement(size_t)=0;
		void Reset();
	protected:
		size_t m_sMapBusy[CRJOSPollElemCount];
		size_t m_sMapFree[CRJOSPollElemCount];
		size_t m_sFree;
	};
}

