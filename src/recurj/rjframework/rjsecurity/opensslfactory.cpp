#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/security/openssl.h>
#include <recurj/rjframework/libs/security/opensslfactory.h>
#include <recurj/rjframework/libs/security/opensslapplication.h>
#define THIS_CTX ((SSL_CTX  *)m_hSSLHandle)
using namespace rjSecurity;
extern "C" {
	int OpenSSLFactoryCallbackPwd(char* pBuf,int lSize,int,void* pPassword) {
		CRJStringByte& clPassword=*((CRJStringByte*) pPassword);
		CRJStringByteVarAssigned clStr((uint8_t*) pBuf,(size_t) lSize);
		return (int) clStr.Set(clPassword).MakeCLabel().GetLength();
	};
	int OpenSSLFactoryCallbackVerify(int preverify_ok, X509_STORE_CTX*) {
		return preverify_ok;
	}
	DH* OpenSSLFactoryCallbackDHParams(SSL* pSSL,int,int lLength) {
		if (pSSL) {
			SSL_CTX* pCTX=SSL_get_SSL_CTX(pSSL);
			if (pCTX!=nullptr&&
				CRJSecurityApplication::m_lSSLAppDataIndex>=0) {
				CRJSecurityFactory* p=(CRJSecurityFactory*) SSL_CTX_get_ex_data(pCTX, CRJSecurityApplication::m_lSSLAppDataIndex);
				if (p) return (DH*)(p->OnSSLFactoryCallbackDHParams((size_t) lLength));
			}
		}
		return nullptr;
	};
}
void* CRJSecurityFactory::OnSSLFactoryCallbackDHParams(size_t lLength) {
	switch (lLength) {
	case 512: return m_clDH512.m_hSSLHandle;
	case 2048: return m_clDH2048.m_hSSLHandle;
	case 4096: return m_clDH4096.m_hSSLHandle;
	}
	return m_clDH1024.m_hSSLHandle;
}
CRJSecurityFactory::CRJSecurityFactory() {
	SSLFactoryAlloc();
	m_bVerifyMustBe=false;
}
CRJSecurityFactory::CRJSecurityFactory(CRJStringByte& clFQDN):
	m_clFQDN(clFQDN) {
	m_clFQDN.UpperCase();
	SSLFactoryAlloc();
	m_bVerifyMustBe=false;
}
CRJSecurityFactory::~CRJSecurityFactory() {
	SSLHandleFree();
}
void CRJSecurityFactory::SSLFactoryAlloc() {
	try {
		if (SSLHandleAttach(SSL_CTX_new(SSLv23_server_method()))) {
			if (CRJSecurityApplication::m_lSSLAppDataIndex>=0) SSL_CTX_set_ex_data(THIS_CTX, CRJSecurityApplication::m_lSSLAppDataIndex,this);
			SSL_CTX_set_default_passwd_cb(THIS_CTX, OpenSSLFactoryCallbackPwd);
			SSL_CTX_set_default_passwd_cb_userdata(THIS_CTX,(void*)& m_clPassword);
			SSL_CTX_set_options(THIS_CTX, SSL_OP_NO_SSLv2);
			SSL_CTX_set_verify(THIS_CTX, SSL_VERIFY_NONE,nullptr);
			SSLHandleError();
		}
	}
	catch(...) {}
}
void CRJSecurityFactory::SSLHandleFree() {
	try {
		if (SSLHandleIsValid()) {
			SSL_CTX_free(THIS_CTX);
		}
	}
	catch(...) {}
	CRJSecurityHandle::SSLHandleFree();
}
void CRJSecurityFactory::SSLFactoryReset() {
	SSLHandleFree();
	SSLFactoryAlloc();
}
bool CRJSecurityFactory::SSLFactoryValidate() {
	try {
		if (SSLHandleIsValid()) {
			if (SSL_CTX_check_private_key(THIS_CTX)) {
				return true;
			}
			else return SSLHandleError();
		}
	}
	catch(...) {}
	return false;
}
bool CRJSecurityFactory::SSLFactoryLoadChain(CRJStringByte& cert) {
	try {
		if (SSLHandleIsValid()) {
			CRJStringByteDefault fn(cert);
			if (fn.MakeCLabel().OK()) {
				if (SSL_CTX_use_certificate_chain_file(THIS_CTX,(const char*) fn.GetHeadForRead())>0) return true;
				else return SSLHandleError();
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJSecurityFactory::SSLFactoryLoadKey(CRJStringByte& key) {
	try {
		if (SSLHandleIsValid()) {
			CRJStringByteDefault fn(key);
			if (fn.MakeCLabel().OK()) {
				if (SSL_CTX_use_PrivateKey_file(THIS_CTX,(const char*) fn.GetHeadForRead(), SSL_FILETYPE_PEM)>0) return true;
				else return SSLHandleError();
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJSecurityFactory::SSLFactoryLoadCertificate(CRJStringByte& cert) {
	try {
		if (SSLHandleIsValid()) {
			CRJStringByteDefault fn(cert);
			if (fn.MakeCLabel().OK()) {
				if (SSL_CTX_use_certificate_file(THIS_CTX,(const char*) fn.GetHeadForRead(), SSL_FILETYPE_PEM)>0) return true;
				else return SSLHandleError();
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJSecurityFactory::SSLFactoryLoadTrusted(CRJStringByte& path,bool bMustBe, size_t lDepth) {
	try {
		if (SSLHandleIsValid()) {
			CRJStringByteDefault fnPath(path);
			if (fnPath.MakeCLabel().OK()) {
				if (SSL_CTX_load_verify_locations(THIS_CTX,nullptr,(const char*) fnPath.GetHeadForRead())) {
					int32_t lMode=SSL_VERIFY_PEER;
					if (bMustBe) lMode|=SSL_VERIFY_FAIL_IF_NO_PEER_CERT;
					m_bVerifyMustBe=bMustBe;
					SSL_CTX_set_verify(THIS_CTX, lMode, OpenSSLFactoryCallbackVerify);
					SSL_CTX_set_verify_depth(THIS_CTX,(int) lDepth);
					return true;
				}
				else return SSLHandleError();
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJSecurityFactory::SSLFactoryLoadDH(CRJStringByte& fn) {
	try {
		if (SSLHandleIsValid()) {
			SSL_CTX_set_tmp_dh_callback(THIS_CTX, OpenSSLFactoryCallbackDHParams);
			if ((m_clDH512.LoadFromFile(fn, 512))&&
				(m_clDH1024.LoadFromFile(fn, 1024))&&
				(m_clDH2048.LoadFromFile(fn, 2048))&&
				(m_clDH4096.LoadFromFile(fn, 4096))) {
				SSL_CTX_set_options(THIS_CTX, SSL_OP_NO_SSLv2| SSL_OP_SINGLE_DH_USE);
				return true;
			}
			else return SSLHandleError();
		}
	}
	catch(...) {}
	return false;
}

