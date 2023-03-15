#pragma once
#include <recurj/rjframework/libs/base/tasks/taskqueue.h>
#include <recurj/rjframework/libs/base/os/hlistener.h>
#include <recurj/rjframework/libs/net/netsocketevent.h>
namespace rjNet {
	const size_t cUNetTaskConnectorReconnect = 10000;
	class RJNET_API CRJNetTaskConnector :public CRJNetTask {
		using funcEvents = void(CRJNetTaskConnector::*)(size_t);
	public:
		CRJNetTaskConnector();
		virtual~CRJNetTaskConnector();
		void OnSocketError(int64_t, size_t) override;
		void OnSocketWarning(int64_t, size_t) override;
		void OnNetEventConnect() override;
		void OnNetEventAccept() override;
		void OnNetEventClose() override;
		void OnNetEventRead() override;
		void OnNetEventSend()  override;
		void OnNetEventError(int64_t, CRJNet::eSocketActions) override;
		CRJSocketTCPEventSend& GetSock() { return m_clSock; };
		SRJNetSocketEndPoint& GetDestination() { return m_clDestination; }
		SRJNetSocketEndPoint& GetSource() { return m_clSource; }
		CRJStringByteVar& GetName(CRJStringByteVar&) override;
		void Play(RJEvent&);
		void Pause(RJEvent&, CRJSocket&);
		void Stop();
		void Next(CRJSocket& sock);
		inline SRJNetSocketEndPoint& GetDestinationToUpdate() { return m_clDestination; }
		inline SRJNetSocketEndPoint& GetSourceToUpdate() { return m_clSource; }
	protected:
		void OnEventHandlerTimeout() override;
		bool TaskRunReady() override;
		void TaskRunDone(bool) override;
		void TaskRunFailed() override;
		void TaskQueueReady() override;
		virtual void OnConnecting();
		virtual void OnConnect();
		virtual void OnDisconnect();
		virtual void OnNoconnect() = 0;
		virtual void OnReceive() = 0;
		virtual void Ping() = 0;
		virtual void onConnectError(int64_t);
		bool sockPrepare();
		void sockClose();
		bool sockConnect();
		void sockRestart();
		bool onSocket(size_t);
		void logNetTryConnect();
		inline void setReconnect(size_t p) { m_sReconnect = p; }
	protected:
		CRJSocketTCPEventSend m_clSock;
		SRJNetSocketEndPoint m_clSource, m_clDestination;
		size_t m_sReconnect;
		bool m_bKeepAlive, m_bStopped;
	};
}

