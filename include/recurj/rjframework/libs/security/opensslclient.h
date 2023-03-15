#pragma once
#include <cdata/comm/septadacommsocket.h>
#include <local/rj/libs/security/opensslconnection.h>
namespace rjSecurity {
	class USECURITY_API CRJSecurityClient:public CRJSecurityConnection {
	public:
		CRJSecurityClient(CRJSecurityFactoryClient&,bool);
		virtual~CRJSecurityClient();
		virtual void SSLConnectionClose();
	protected:
		bool DoClientConnect(const CRJStringByte&, int32_t, int32_t, int32_t, int32_t,bool bSSL=true);
		bool SSLConnectionClientConnect();
		bool SSLClientRead(int32_t, int32_t, std::function<bool(const CRJValue&)>);
		bool SSLClientReadBinary(int32_t, int32_t, std::function<bool(const CRJValue&)>);
		virtual bool DoSendNetworkData(const CRJValue&);
		virtual bool SSLConnectionDoHandshake();
	protected:
		void SSLConnectionClientAlloc();
		void doReset();
		bool doHandshake(int32_t);
		bool doReConnect(
		struct sockaddr_in&, int32_t);
		bool doWaitEventConnect(int32_t);
		bool doWaitEventRead(int32_t);
		bool doCheckSockEventRead();
		bool doCheckSockEventConnect();
		virtual bool onSockErrorNoEvent(int32_t);
		virtual bool onSockErrorNoRead(int32_t);
		virtual bool onSockErrorNoConnect(int32_t);
	protected:
		CSeptadaAtomCommSocket m_clSocket;
		CSeptadaAtomCommEvent m_clSockEvent;
		CSeptadaCommCollector m_clSockRaw;
		SSeptadaCommSocketSendMetrics m_stSockSendMetrics;
	};
}

