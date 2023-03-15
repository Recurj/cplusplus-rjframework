#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/net/tasks/nettaskpool.h>
#include <recurj/rjframework/libs/net/netsocketeventsessionpooled.h>
#include <recurj/rjframework/libs/net/tasks/nettasklistener.h>
namespace rjNet {
	class CRJMessageNetSession;
	class RJNET_API CRJNetTaskSessionListener:public CRJNetTaskListener,public IRJSocketPoolManager {
	public:
		CRJNetTaskSessionListener();
		CRJTask& GetAcceptor() override;
		bool PrepareAcceptor() override;
		void ReleaseAcceptor() override;
		CRJTask* PoolCreateTask(size_t) override;
		void PoolFreeTask(CRJTask*) override;
		CRJSocketTCPPool* PoolElem(size_t) override;
		const SRJSocketProcessMetrics& PoolSendMetrics() const override {return m_stSendMetrics;}
		CRJTask& GetParentTask() override {	return TaskGetMe();}
	protected:
		class CTaskNetPool:public CRJNetTaskPool {
		public:
			CTaskNetPool(CRJNetTaskSessionListener&, size_t);
			virtual~CTaskNetPool();
			void operator delete(void*);
			void*operator new(size_t);
		protected:
			CRJMessage* OnTaskQueueMsg(CRJMessage&) override;
			CRJMessage* onMsgRegionNet(CRJMessage&);
			CRJMessage* OnFindSession(CRJMessageNetSession&);
			CRJMessage* OnSendSession(CRJMessageNetSession&);
			CRJNetSocketEventSessionPooled* GetCheckedIndex(size_t ind) {return (CRJNetSocketEventSessionPooled*) m_pElems[ind];}
			bool OnWaitEventSocket(size_t);
		protected:
			static CRJAllocatorRuntime m_clHeap;
		};
		bool Process(CRJSocket&,const CRJStringByte&);
		void FreeTaskNetPool(CTaskNetPool*);
	protected:
		CTaskNetPool* m_pTaskPool;
		SRJSocketProcessMetrics m_stSendMetrics;
	};
}

