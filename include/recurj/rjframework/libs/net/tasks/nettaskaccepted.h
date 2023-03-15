#pragma once
#include <recurj/rjframework/libs/base/tasks/taskqueue.h>
#include <recurj/rjframework/libs/base/os/hlistener.h>
#include <recurj/rjframework/libs/net/netsocketeventaccepted.h>
namespace rjNet {
	class RJNET_API CRJNetTaskAccepted :public CRJNetTask {
		using funcEvents = void(CRJNetTaskAccepted::*)(size_t);
	public:
		CRJNetTaskAccepted(const SRJSocketProcessMetrics&);
		CRJNetTaskAccepted(CRJSocket&, const SRJSocketProcessMetrics&);
		virtual~CRJNetTaskAccepted();
		void OnNetEventConnect() override;
		void OnNetEventAccept() override;
		void OnNetEventClose() override;
		void OnNetEventRead() override;
		void OnNetEventSend()  override;
		void OnNetEventError(int64_t, CRJNet::eSocketActions) override;
		CRJStringByteVar& GetName(CRJStringByteVar&) override;
		void Take(CRJSocket&);
		inline size_t GetPause() const { return m_sPause; }
		inline void SetPause(size_t p) { m_sPause = p; }
		//		inline CRJNetTaskAccepted& GetTaskAccepted() {return *this;}
		inline CRJSocketTCPAccepted& GetSock() { return m_clSock; }
	protected:
		void OnEventHandlerTimeout() override;
		bool TaskRunReady() override;
		void TaskRunDone(bool) override;
		void TaskRunBody() override;
		bool onSocket(size_t);
		virtual void OnReceive() = 0;
		virtual void Disconnected() = 0;
		virtual void Ping() = 0;
	protected:
		CRJSocketTCPAccepted m_clSock;
		size_t m_sPause;
	};
}

