#pragma once
#include <recurj/rjframework/libs/base/os/hlistener.h>
#include <recurj/rjframework/libs/net/netsocketeventpool.h>
#include <recurj/rjframework/libs/base/tasks/taskqueuepool.h>
#include <recurj/rjframework/libs/net/messages/netmessagesock.h>
using namespace rjNet;
namespace rjNet {
	class IRJSocketPoolManager {
	public:
		virtual CRJTask* PoolCreateTask(size_t)=0;
		virtual void PoolFreeTask(CRJTask*)=0;
		virtual CRJSocketTCPPool* PoolElem(size_t)=0;
		virtual const SRJSocketProcessMetrics& PoolSendMetrics() const=0;
		virtual CRJTask& GetParentTask()=0;
	};
	class RJNET_API CRJNetTaskPool:public CRJTaskQueuePool {
	public:
		CRJNetTaskPool(IRJSocketPoolManager&, size_t);
		virtual~CRJNetTaskPool();
		bool IsValidTaskTicket(const SRJTaskTicket& stPost);
		const SRJSocketProcessMetrics& PoolSendMetrics() const {return m_clManager.PoolSendMetrics();}
		CRJStringByteVar& GetName(CRJStringByteVar&) override;
	protected:
		CRJMessage* OnTaskQueueMsg(CRJMessage&) override;
		void OnEventHandlerError(int64_t) override;
		bool OnWaitEventElement(size_t) override;
		CRJMessage* OnSockAccepted(CRJMessageSocket&);
		CRJMessage* Propogate(CRJMessageSocket&);
		CRJMessage* onMsgRegionNet(CRJMessage&);
		void repos();
		void remove(size_t);
	protected:
		CRJTask* m_pTaskNext;
		CRJSocketTCPPool* m_pElems[CRJOSPollElemCount];
		IRJSocketPoolManager& m_clManager;
		size_t m_sIndex, m_sAllocated;
	};
}

