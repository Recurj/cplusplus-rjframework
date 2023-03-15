#include "stdafx.h"
#include <recurj/rjframework/libs/security/openssl.h>
#include <recurj/rjframework/libs/security/opensslcipher.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>

#define THIS_CIPHER_CTX ((EVP_CIPHER_CTX *)m_hSSLHandle)
using namespace rjSecurity;
CRJSecurityCipher::CRJSecurityCipher() {
	SSLCipherAlloc();
}
CRJSecurityCipher::~CRJSecurityCipher() {
	SSLHandleFree();
}
void CRJSecurityCipher::SSLHandleFree() {
	try {
		if (SSLHandleIsValid()) {
			EVP_CIPHER_CTX_free(THIS_CIPHER_CTX);
		}
	}
	catch(...) {}
	CRJSecurityHandle::SSLHandleFree();
}
void CRJSecurityCipher::SSLCipherAlloc() {
	try {
		SSLHandleAttach(EVP_CIPHER_CTX_new());
	}
	catch(...) {}
}
int64_t CRJSecurityCipher::pBlockSize() {
	return EVP_CIPHER_CTX_block_size(THIS_CIPHER_CTX);
}
bool CRJSecurityCipher::EncryptStep(const CRJStringByteFlex& clSrc, CRJStringByteVarAssigned& clDst) {
	try {
		if (SSLHandleIsValid()) {
			size_t lSrcLen=clSrc.GetLength();
			size_t lOutLen=clDst.GetSize();
			int64_t cs=EVP_CIPHER_CTX_block_size(THIS_CIPHER_CTX);
			if (lOutLen>lSrcLen+(size_t) cs) {
				const uint8_t* pSrc = clSrc.GetHeadForRead();
				uint8_t* pOut = clDst.GetHeadForUpdate();
				int32_t l1=0;
				if (EVP_EncryptUpdate(THIS_CIPHER_CTX, pOut,& l1, pSrc,(int) lSrcLen)!=1) SSLHandleError();
				else if (lOutLen>=(size_t)(l1+cs)) {
					int32_t l2=0;
					if (EVP_EncryptFinal_ex(THIS_CIPHER_CTX, pOut+l1,& l2)!=1) SSLHandleError();
					else {
						clDst.SetLengthSafed((size_t)(l1+l2));
						return true;
					}
				}
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJSecurityCipher::DecryptStep(const CRJStringByteFlex& clSrc, CRJStringByteVarAssigned& clDst) {
	try {
		if (SSLHandleIsValid()) {
			size_t lSrcLen=clSrc.GetLength();
			size_t lOutLen=clDst.GetSize();
			int64_t cs=EVP_CIPHER_CTX_block_size(THIS_CIPHER_CTX);
			if (lOutLen>lSrcLen+(size_t) cs) {
				const uint8_t* pSrc= clSrc.GetHeadForRead();
				uint8_t* pOut= clDst.GetHeadForUpdate();
				int32_t l1=0;
				if (EVP_DecryptUpdate(THIS_CIPHER_CTX, pOut,& l1, pSrc,(int) lSrcLen)!=1) SSLHandleError();
				else if (lOutLen>=(size_t)(l1+cs)) {
					int32_t l2=0;
					if (EVP_DecryptFinal_ex(THIS_CIPHER_CTX, pOut+l1,& l2)!=1) SSLHandleError();
					else {
						clDst.SetLengthSafed((size_t)(l1+l2));
						return true;
					}
				}
			}
		}
	}
	catch(...) {}
	return false;
}
void CRJSecurityCipherAES256::Load(const uint8_t* pKey,const uint8_t* pIV) {
	try {
		*((uint64_t*) m_bKey)=*((uint64_t*) pKey);
		*((uint64_t*)(m_bKey+8))=*((uint64_t*)(pKey+8));
		*((uint64_t*)(m_bKey+16))=*((uint64_t*)(pKey+16));
		*((uint64_t*)(m_bKey+24))=*((uint64_t*)(pKey+24));
		*((uint64_t*) m_bIV)=*((uint64_t*) pIV);
		*((uint64_t*)(m_bIV+8))=*((uint64_t*)(pIV+8));
	}
	catch(...) {}
}
bool CRJSecurityCipherAES256::EncryptInit() {
	try {
		if (SSLHandleIsValid()) {
			if (EVP_EncryptInit_ex((EVP_CIPHER_CTX*) SSLHandleGet(), EVP_aes_256_cbc(), NULL, m_bKey, m_bIV)==1) return true;
			else SSLHandleError();
		}
	}
	catch(...) {}
	return false;
}
bool CRJSecurityCipherAES256::DecryptInit() {
	try {
		if (SSLHandleIsValid()) {
			if (EVP_DecryptInit_ex((EVP_CIPHER_CTX*) SSLHandleGet(), EVP_aes_256_cbc(), NULL, m_bKey, m_bIV)==1) return true;
			else SSLHandleError();
		}
	}
	catch(...) {}
	return false;
}

