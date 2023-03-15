#pragma once
#include <recurj/rjframework/libs/security/opensslhandle.h>
#include <recurj/rjframework/libs/base/strings/byte/strings.h>
namespace rjSecurity {
	const size_t cSeptadaAES256Key=32;
	const size_t cSeptadaAES256IV=16;
	class RJSECURITY_API CRJSecurityCipher:public CRJSecurityHandle {
	public:
		CRJSecurityCipher();
		virtual~CRJSecurityCipher();
		int64_t pBlockSize();
		bool EncryptStep(const CRJStringByteFlex&, CRJStringByteVarAssigned&);
		bool DecryptStep(const CRJStringByteFlex&, CRJStringByteVarAssigned&);
	protected:
		void SSLHandleFree() override;
		void SSLCipherAlloc();
	};
	class RJSECURITY_API CRJSecurityCipherAES256:public CRJSecurityCipher {
	public:
		using CRJSecurityCipher::CRJSecurityCipher;
		void Load(const uint8_t*,const uint8_t*);
		bool EncryptInit();
		bool DecryptInit();
	private:
		uint8_t m_bKey[cSeptadaAES256Key];
		uint8_t m_bIV[cSeptadaAES256IV];
	};
}

