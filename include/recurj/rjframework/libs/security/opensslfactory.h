#pragma once
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/security/opensslhandle.h>
#include <recurj/rjframework/libs/security/openssldhparams.h>
namespace rjSecurity {
	class RJSECURITY_API CRJSecurityFactory:public CRJSecurityHandle {
		friend class CRJSecurityConnectionServer;
	public:
		CRJSecurityFactory();
		CRJSecurityFactory(CRJStringByte&);
		virtual~CRJSecurityFactory();
		virtual void SSLFactoryReset();
		void* OnSSLFactoryCallbackDHParams(size_t);
		bool SSLFactoryValidate();
		bool SSLFactoryLoadChain(CRJStringByte&);
		bool SSLFactoryLoadKey(CRJStringByte&);
		bool SSLFactoryLoadTrusted(CRJStringByte&,bool bMustBe=true, size_t lDepth=9);
		bool SSLFactoryLoadDH(CRJStringByte&);
		bool SSLFactoryVerifyMustBe() const {
			return m_bVerifyMustBe;
		}
		bool SSLFactoryLoadCertificate(CRJStringByte&);
		inline CRJStringByte& SSLFactoryGetFQDN() {
			return m_clFQDN;
		}
		inline void SSLFactorySetFQDN(const CRJStringByte& cl) {
			m_clFQDN.Set(cl).UpperCase();
		}
		inline void SSLFactorySetPassword(const CRJStringByte& cl) {
			m_clPassword.Set(cl);
		}
	protected:
		void SSLHandleFree() override;
	private:
		void SSLFactoryAlloc();
	protected:
		CRJSecurityDHParams m_clDH512, m_clDH1024, m_clDH2048, m_clDH4096;
		CRJStringByteDefault m_clFQDN, m_clPassword;
		bool m_bVerifyMustBe;
	};
}

