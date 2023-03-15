#include "stdafx.h"
#include <recurj/rjframework/libs/security/openssl.h>
#include <recurj/rjframework/libs/security/opensslpassword.h>
#include <recurj/rjframework/libs/security/opensslcipher.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
using namespace rjSecurity;
const char CRJSecurityPassword::s_iv[16]="734728594782679";
const uint8_t CRJSecurityPassword::s_salt[8]= {
	0x69, 0x02, 0x76, 0x56, 0xf1, 0xf4, 0xdf, 0x05
};
CRJSecurityPassword::CRJSecurityPassword() {}
CRJSecurityPassword::~CRJSecurityPassword() {}
bool CRJSecurityPassword::Build(const CRJStringByteFlex& clSrc, CRJStringByteVarAssigned& clDst) {
	try {
		unsigned char key[32];
		if (PKCS5_PBKDF2_HMAC_SHA1((const char*) clSrc.GetHeadForRead(), (int)clSrc.GetLength(), CRJSecurityPassword::s_salt, sizeof(CRJSecurityPassword::s_salt), SecurityPasswordIterations, sizeof(key), key) == 1) {
			CRJSecurityCipherAES256 enc;
			enc.Load(key,(uint8_t*) CRJSecurityPassword::s_iv);
			return enc.EncryptInit()&&
				enc.EncryptStep(clSrc, clDst);
		}
	}
	catch(...) {}
	return false;
}

