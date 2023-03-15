#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/security/security.h>
namespace rjSecurity {
	const int64_t theSSLHandleMaxErrorInStep=32;
	class RJSECURITY_API CRJSecurityHandle {
		friend class CRJSecurityBIO;
		friend class CRJSecurityConnection;
		friend class CRJSecurityConnectionServer;
		friend class CRJSecurityConnectionClient;
		friend class CRJSecurityFactory;
		friend class CRJSecurityX509;
		friend class CRJSecurityX509Extension;
		friend class CRJSecurityX509Name;
		friend class CRJSecurityDHParams;
		friend class CRJSecurityCipher;
		friend class CRJSecurityPublicKey;
		friend class CRJSecurityPublicKeyEx;
		friend class CRJSecurityRSA;
		friend class CRJSecurityRSAContext;
	public:
		CRJSecurityHandle();
		CRJSecurityHandle(openSSLHandle);
		virtual~CRJSecurityHandle();
		inline openSSLHandle SSLHandleGet() const {
			return m_hSSLHandle;
		}
		inline operator bool() const {
			return m_hSSLHandle!=nullptr;
		}
	protected:
		virtual bool SSLHandleError();
		virtual void SSLHandleFree();
		inline bool SSLHandleIsValid() const {
			return m_hSSLHandle!=nullptr;
		}
		void OnSSLHandleError(const CRJStringByte&);
		bool SSLHandleAttach(openSSLHandle);
	protected:
		static CRJStringByte s_clSSLHandleWordError;
		static CRJStringByte s_clSSLHandleWordSubjectAltName;
		static CRJStringByte s_clSSLHandleWordDNS;
	private:
		openSSLHandle m_hSSLHandle;
	};
}

