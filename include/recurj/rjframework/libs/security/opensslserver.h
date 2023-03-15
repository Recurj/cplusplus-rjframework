#pragma once
#include <cdata/comm/septadacommsocket.h>
#include <cdata/security/septadaopensslconnection.h>
namespace rjSecurity {
	class USECURITY_API CRJSecurityServer:public CRJSecurityConnection {
	public:
		CRJSecurityServer(CRJSecurityFactoryServer&);
		virtual~CRJSecurityServer();
		virtual void SSLConnectionClose();
	protected:
		virtual bool DoSendNetworkData(const CRJValue&);
		virtual bool SSLConnectionDoHandshake();
		virtual bool SSLConnectionHandshakeDone();
		bool SSLConnectionServerAccept();
	protected:
		bool SSLConnectionPostCheck();
		void SSLConnectionServerAlloc();
	protected:
		CSeptadaAtomCommSocket m_clSocket;
		CSeptadaAtomCommEvent m_clSockEvent;
		CSeptadaCommCollector m_clSockRaw;
		SSeptadaCommSocketSendMetrics m_stSockSendMetrics;
	};
}

