#pragma once
#include <recurj/rjframework/libs/security/opensslrsa.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
namespace rjSecurity {
	class RJSECURITY_API CRJSecurityPublicKey:public CRJSecurityHandle {
	public:
		enum 
		class eRJPKIResults {
			Unknown=-1, OK=0, NoFileName=1, BadFileName=2, NoFileRead=3, NoRSA=4, NoSignHash=5,
			NoSignUpdate=6, NoSignFinal=7, NoVerifyInit=8, NoVerifyUpdate=9, NoVerifyFinal=10,
			NoBuf=11, NoEncrypt=12, NoDecrypt=13, NotEnoughBuffer=14, BadKeySize=15
		};
		CRJSecurityPublicKey();
		CRJSecurityPublicKey(size_t);
		virtual~CRJSecurityPublicKey();
		int64_t pGetKeySize();
		bool Extract(CRJStringByteVar&,bool);
		bool Gen(uint32_t,const char*,const char*);
		eRJPKIResults LoadFile(const CRJStringByte&,bool);
		eRJPKIResults SignMessage(const CRJStringByteFlex&, uint8_t*, uint32_t*);
		eRJPKIResults VerifyMessage(const CRJStringByteFlex&, CRJStringByteVar&);
		eRJPKIResults Encrypt(const CRJStringByteFlex&, CRJStringByteVar&, CRJStringByteVar&);
		eRJPKIResults Decrypt(const CRJStringByteFlex&, CRJStringByteVar&, CRJStringByteVar&);
		void SSLPublicKeyAlloc();
		void SSLPublicKeyAlloc(size_t);
	protected:
		void SSLHandleFree() override;
		void Close(FILE*);
	};
	class RJSECURITY_API CRJSecurityBuffer final:public CRJStringByteVarAssigned {
	public:
		CRJSecurityBuffer(CRJSecurityPublicKey&);
		~CRJSecurityBuffer();
	private:
		CRJSecurityBuffer();
		void release();
	};
	class RJSECURITY_API CRJSecurityPublicKeyEx:public CRJSecurityPublicKey {
	public:
		CRJSecurityPublicKeyEx(const CRJStringByte&,bool);
		virtual~CRJSecurityPublicKeyEx();
	protected:
		void SSLPublicKeyExAlloc(const CRJStringByte&,bool);
		CRJSecurityRSA m_clRSA;
	};
}

