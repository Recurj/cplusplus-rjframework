#pragma once
#include <recurj/rjframework/libs/security/security.h>
namespace rjSecurity {
	const size_t DES_SINGLE_KEY=0;
	const size_t DES_DOUBLE_KEY=1;
	struct SRJSecurity24KeyInfo {
		uint8_t m_uchComp1[cSizeSecurityKey];
		uint8_t m_uchComp2[cSizeSecurityKey];
		uint8_t m_uchKVV1[cSizeSecurityKey];
		uint8_t m_uchKVV2[cSizeSecurityKey];
		uint8_t m_uchKVVItself[cSizeSecurityKey];
		uint8_t m_uchItself[cSizeSecurityKey];
		uint8_t m_uchMaster[cSizeSecurityKey];
		uint8_t m_uchPlain[cSizeSecurityKey];
	};
	class RJSECURITY_API CRJSecurityDesTools {
	public:
		static void DoEncryptDes(uint8_t*,const uint8_t*,const uint8_t*);
		static void DoDecryptDes(uint8_t*,const uint8_t*,const uint8_t*);
		static void DoEncryptDesBlock(uint8_t*, size_t,const uint8_t*);
		static void DoDecryptDesBlock(uint8_t*, size_t,const uint8_t*);
		static void DoEncryptTripleDes(uint8_t*,const uint8_t*,const uint8_t*);
		static void DoDecryptTripleDes(uint8_t*,const uint8_t*,const uint8_t*);
		static void DoEncryptTripleDesBlock(uint8_t*, size_t,const uint8_t*);
		static void DoDecryptTripleDesBlock(uint8_t*, size_t,const uint8_t*);
		static void DoEncryptTripleDesBlockSimple(uint8_t*, size_t,const uint8_t*);
		static void DoDecryptTripleDesBlockSimple(uint8_t*, size_t,const uint8_t*);
		static void BuildDesMac(uint8_t*,const uint8_t*,const uint8_t*, size_t);
		static void BuildTripleDesMac(uint8_t*,const uint8_t*,const uint8_t*, size_t);
		static void BuildTripleDesMacVariant(uint8_t*,const uint8_t*,const uint8_t*, size_t);
		static bool CreateKey(SRJSecurity24KeyInfo&,const uint8_t*,bool);
		static bool CalcKVV(SRJSecurity24KeyInfo&,const uint8_t*,bool);
		static size_t GetKeyLength(size_t);
	};
}

