#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/net/netsocketeventsessionpooled.h>
#include <recurj/rjframework/libs/net/tasks/nettaskconnector.h>
namespace rjNet {
	class RJNET_API CRJNetTaskSessionConnector:public CRJNetTaskConnector {
	public:
		CRJNetTaskSessionConnector();
		virtual bool StartSession(CRJSocket&, uint64_t,const CRJStringByte&)=0;
		void OnConnect() override;
		void OnReceive() override;
		void Ping() override;
		void SetVolume(size_t v) {m_sVolume=v;}
	protected:
		virtual CRJMessage* OnTaskQueueMsg(CRJMessage&);
		CRJMessage* onMsgRegionNet(CRJMessage&);
		void OnSessionDone(size_t);
		void SendConfirm(uint64_t);
		void SendDone(uint64_t);
		void SendPID(uint64_t);
		void SendStatistic();
		void SendPing();
	protected:
		CRJStringByteDefault m_clWriteBuffer;
		size_t m_sVolume, m_sInUse;
	};
}

