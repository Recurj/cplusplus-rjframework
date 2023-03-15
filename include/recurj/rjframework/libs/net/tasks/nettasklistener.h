#pragma once
#include <recurj/rjframework/libs/base/tasks/taskqueue.h>
#include <recurj/rjframework/libs/base/os/hlistener.h>
#include <recurj/rjframework/libs/net/netsocketevent.h>

namespace rjNet {
	class RJNET_API CRJNetTaskListener:public CRJNetTask {
	public:
		CRJNetTaskListener();
		virtual~CRJNetTaskListener();
		CRJStringByteVar& GetName(CRJStringByteVar&) override;
		void OnNetEventConnect() override;
		void OnNetEventAccept() override;
		void OnNetEventClose() override;
		void OnNetEventRead() override;
		void OnNetEventSend()  override;
		void OnNetEventError(int64_t, CRJNet::eSocketActions) override;
		SRJNetSocketEndPoint& GetSource() {return m_clSource;}
	protected:
		bool TaskRunReady() override;
		void TaskRunDone(bool) override;
		void TaskRunFailed() override;
		void TaskRunBody() override;
		void TaskQueueReady() override;
		virtual CRJTask& GetAcceptor()=0;
		virtual bool PrepareAcceptor()=0;
		virtual void ReleaseAcceptor()=0;
		bool onSocket(size_t);
		void listen();
		void errorNoAcceptor();
	protected:
		CRJSocketTCPEvent m_clSock;
		SRJNetSocketEndPoint m_clSource;
	};
}

