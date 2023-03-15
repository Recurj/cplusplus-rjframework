#pragma once
#include <recurj/rjframework/libs/base/datatools/bytestorage.h>
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/security/opensslbio.h>
namespace rjSecurity {
	class CRJSecurityFactory;
	class RJSECURITY_API CRJSecurityConnectionApplData:public CRJByteStorage {
	public:
		CRJSecurityConnectionApplData(size_t);
		virtual~CRJSecurityConnectionApplData();
		inline bool IsReady() const {
			return m_pBuffer!=nullptr;
		}
	protected:
		uint8_t* m_pBuffer;
	};
	class RJSECURITY_API CRJSecurityConnection:public CRJSecurityHandle {
	public:
		CRJSecurityConnection(size_t);
		virtual~CRJSecurityConnection();
		bool SSLConnectionRecv(CRJStringByteFlex&);
		bool SSLConnectionInited();
		bool SSLConnectionSend(CRJStringByteFlex&);
		bool SSLConnectionClose();
		bool SSLConnectionReset();
		bool SSLConnectionHandshake();
		static void SSLClearThreadErrors();
		inline CRJSecurityConnection& SSLConnectionGetMe() {
			return *this;
		}
	protected:
		virtual bool SSLConnectionReady();
		void SSLHandleFree() override;
		bool SSLHandleError() override;
		virtual bool OnSSLDataApplication(const CRJStringByteFlex&)=0;
		virtual bool OnSSLDataNetwork(const CRJStringByteFlex&)=0;
		virtual int SSLHandshakeStart()=0;
		virtual bool OnSSLHandshakeDone();
	private:
		bool SSLConnectionCheckApplData();
		bool SSLConnectionCheckNetData();
		bool SSLConnectionRead(bool&);
	protected:
		static CRJStringByte s_clSSLConnectionErrorTooMuchData;
		static CRJStringByte s_clSSLConnectionErrorNoCertificate;
	protected:
		CRJSecurityConnectionApplData m_clApplData;
		CRJSecurityBIOMemory m_clRead;
		CRJSecurityBIOMemory m_clSend;
	};
	class RJSECURITY_API CRJSecurityConnectionClient:public CRJSecurityConnection {
	public:
		CRJSecurityConnectionClient(size_t);
		virtual~CRJSecurityConnectionClient();
	protected:
		int SSLHandshakeStart() override;
		void SSLConnectionAlloc();
	private:
		CRJSecurityHandle m_clCTX;
	};
	class RJSECURITY_API CRJSecurityConnectionServer:public CRJSecurityConnection {
	public:
		CRJSecurityConnectionServer(CRJSecurityFactory&, size_t);
		virtual~CRJSecurityConnectionServer();
	protected:
		int SSLHandshakeStart() override;
		bool OnSSLHandshakeDone() override;
		void SSLConnectionAlloc();
	private:
		bool SSLConnectionPostCheck();
	private:
		CRJSecurityFactory& m_clFactory;
	};
}

