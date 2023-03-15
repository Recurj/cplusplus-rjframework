#pragma once
#include <recurj/rjframework/libs/net/netsockettcp.h>
#include <recurj/rjframework/libs/net/netsend.h>
namespace rjNet {
	class RJNET_API CRJSocketTCPEvent:public CRJSocketTCP {
	public:
		CRJSocketTCPEvent(IRJSocketHolder&);
		virtual~CRJSocketTCPEvent();
		void SocketRelease() override;
		void SocketEventClose();
		bool SocketEventPrepareListen();
		bool SocketEventPrepareConnect();
		bool SocketEventPrepareAccepted();
		CRJSocketTCPEvent& HappenedEvents(IRJSocketManager& ,size_t);
		void OnNetEventError(IRJSocketManager&, int64_t, CRJNet::eSocketActions);
		inline bool IsConnectedAndReady() const {return SocketIsConnected()&&GetEvent().HandleIsReady();}
		inline RJEvent& GetEvent() const {return (RJEvent&) m_clEvent;}
	protected:
		bool SocketEventPrepare(int64_t);
	protected:
#ifdef _rjOS_FAMILY_UNIX
		RJEventDuplicated m_clEvent;
#endif 
#ifdef _rjOS_WIN
		RJEvent m_clEvent;
#endif 
	};
	class RJNET_API CRJSocketTCPEventSend:public CRJSocketTCPEvent {
	public:
		CRJSocketTCPEventSend(IRJSocketHolder& holder):CRJSocketTCPEvent(holder),m_clNetSend(SocketGetMe()) {}
		void OnNetEventSend();
		inline CRJNetSend& GetNetSend() {return m_clNetSend;}
	protected:
		CRJNetSend m_clNetSend;
	};
}

