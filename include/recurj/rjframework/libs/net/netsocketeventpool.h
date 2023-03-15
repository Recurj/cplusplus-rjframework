#pragma once
#include <recurj/rjframework/libs/net/netsocketevent.h>
namespace rjNet {
	class CRJNetTaskPool;
	class RJNET_API CRJSocketTCPPool:public CRJSocketTCPEvent, public IRJSocketManager {
	public:
		CRJSocketTCPPool(IRJSocketHolder&);
		virtual~CRJSocketTCPPool();
		void OnNetEventClose() override;
		void OnNetEventConnect() override;
		void OnNetEventAccept() override;
		void OnNetEventError(int64_t, CRJNet::eSocketActions) override;
		virtual bool PoolStart();
		virtual void PoolRelease();
		void PoolBind(size_t, CRJNetTaskPool&);
		bool PoolReady();
		bool PoolAdd(CRJSocket&);
		SRJTaskTicket& BuildTicket(SRJTaskTicket&);
		void SocketRedirected();
		inline CRJSocketTCPPool& GetMeInPool() {return *this;}
		inline bool IsThatSockLife(size_t l) const {return m_sLifeSock.load()==l;}
		inline size_t GetSockLifeId() const {return m_sLifeSock.load();}
		inline void IncSockLifeId() {m_sLifeSock.fetch_add(1);}
	protected:
		CRJNetTaskPool* m_pPool;
		std::atomic<size_t>m_sLifeSock;
		size_t m_sIndex;
	};
}

