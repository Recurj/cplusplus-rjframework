#include "stdafx.h"
#include <recurj/rjframework/libs/security/opensslpublickey.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
#include <recurj/rjframework/libs/security/openssl.h>
#include <recurj/rjframework/libs/security/opensslapplication.h>
#include <recurj/rjframework/libs/security/opensslhash.h>
#include <recurj/rjframework/libs/security/opensslrsacontext.h>
#define THIS_PKEY ((EVP_PKEY *)m_hSSLHandle)
using namespace rjSecurity;
CRJSecurityPublicKey::CRJSecurityPublicKey() {}
CRJSecurityPublicKey::CRJSecurityPublicKey(size_t bits) {
	CRJSecurityRSAContext clContext;
	if (clContext) SSLHandleAttach(clContext.Create(bits));
}
CRJSecurityPublicKey::~CRJSecurityPublicKey() {
	SSLHandleFree();
}
void CRJSecurityPublicKey::SSLPublicKeyAlloc() {
	SSLHandleFree();
	SSLHandleAttach(EVP_PKEY_new());
}
int64_t CRJSecurityPublicKey::pGetKeySize() {
	return EVP_PKEY_size(THIS_PKEY);
}
void CRJSecurityPublicKey::SSLPublicKeyAlloc(size_t bits) {
	CRJSecurityRSAContext clContext;
	SSLHandleFree();
	if (clContext) SSLHandleAttach(clContext.Create(bits));
}
void CRJSecurityPublicKey::SSLHandleFree() {
	try {
		if (SSLHandleIsValid()) EVP_PKEY_free(THIS_PKEY);
	}
	catch(...) {}
	CRJSecurityHandle::SSLHandleFree();
}
void CRJSecurityPublicKey::Close(FILE* pf) {
	try {
		if (pf) fclose(pf);
	}
	catch(...) {}
}
CRJSecurityPublicKey::eRJPKIResults CRJSecurityPublicKey::LoadFile(const CRJStringByte& fn,bool bPublic) {
	FILE* pFile=nullptr;
	eRJPKIResults rc=eRJPKIResults::Unknown;
	try {
		CRJStringByteExpanded s;
		SSLHandleFree();
		if (s.HasNeededSize(fn.GetLength()+1)&&
			s.Set(fn).MakeCLabel().OK()) {
#ifdef _rjOS_WIN
			if ((fopen_s(& pFile, s.GetHeadForReadC(),"rb"))||
				(!pFile))throw eRJPKIResults::BadFileName;
#else 
			pFile=fopen(s.GetHeadForReadC(),"rb");
			if (!pFile)throw eRJPKIResults::BadFileName;
#endif 
			SSLHandleAttach((bPublic)?
				PEM_read_PUBKEY(pFile,nullptr,nullptr,nullptr):
				PEM_read_PrivateKey(pFile,nullptr,nullptr,nullptr));
			if (!SSLHandleIsValid())throw eRJPKIResults::NoFileRead;
			return eRJPKIResults::OK;
		}
		else throw eRJPKIResults::NoFileName;
	}
	catch(eRJPKIResults err) {
		rc=err;
	}
	catch(...) {}
	Close(pFile);
	return rc;
}
CRJSecurityPublicKey::eRJPKIResults CRJSecurityPublicKey::SignMessage(const CRJStringByteFlex& src, uint8_t* signature, uint32_t* sig_len) {
	eRJPKIResults rc=eRJPKIResults::Unknown;
	try {
		CRJSecurityHash ctx;
		if (!EVP_SignInit(ctx, EVP_md5()))throw eRJPKIResults::NoSignHash;
		if (!EVP_SignUpdate(ctx, src.GetHeadForRead(), src.GetLength())) throw eRJPKIResults::NoSignUpdate;
		if (!EVP_SignFinal(ctx, signature, sig_len, THIS_PKEY))throw eRJPKIResults::NoSignFinal;
		rc=eRJPKIResults::OK;
	}
	catch(eRJPKIResults err) {
		rc=err;
	}
	catch(...) {}
	return rc;
}
CRJSecurityPublicKey::eRJPKIResults CRJSecurityPublicKey::VerifyMessage(const CRJStringByteFlex& src, CRJStringByteVar& signature) {
	try {
		CRJSecurityHash ctx;
		if (!EVP_VerifyInit(ctx, EVP_md5())) return eRJPKIResults::NoVerifyInit;
		if (!EVP_VerifyUpdate(ctx, src.GetHeadForRead(), src.GetLength())) return eRJPKIResults::NoVerifyUpdate;
		if (!EVP_VerifyFinal(ctx, signature.GetHeadForUpdate(),(unsigned int) signature.GetSize(), THIS_PKEY)) return eRJPKIResults::NoVerifyFinal;
		return eRJPKIResults::OK;
	}
	catch(...) {}
	return eRJPKIResults::Unknown;
}
CRJSecurityPublicKey::eRJPKIResults CRJSecurityPublicKey::Encrypt(const CRJStringByteFlex& src, CRJStringByteVar& dst, CRJStringByteVar& tmp) {
	dst.SetEmpty();
	try {
		const uint8_t* pSrc=src.GetHeadForRead();
		size_t lLen=src.GetLength();
		uint8_t* pDst=dst.GetHeadForUpdate();
		size_t lVol=dst.GetSize();
		int64_t k, ol, lKeySize=EVP_PKEY_size(THIS_PKEY),
		l=lKeySize-12;
		uint8_t* pBuf = tmp.GetHeadForUpdate();
		if (l<=0) return eRJPKIResults::BadKeySize;
		if (tmp.GetSize()<(size_t)(2* lKeySize)||
			!pBuf) return eRJPKIResults::NoBuf;
		while (lLen>0) {
			k=(lLen>(size_t) l)?
				l:lLen;
			ol=RSA_public_encrypt((int) k, pSrc, pBuf, EVP_PKEY_get0_RSA(THIS_PKEY), RSA_PKCS1_PADDING);
			if (ol!=lKeySize) {
				SSLHandleError();
				return eRJPKIResults::NoEncrypt;
			}
			lVol-=ol;
			if (lVol<0) return eRJPKIResults::NotEnoughBuffer;
			memcpy(pDst, pBuf,(size_t) ol);
			pDst+=ol;
			dst.GrowSafed(ol);
			lLen-=k;
			pSrc+=k;
		}
		return eRJPKIResults::OK;
	}
	catch(...) {}
	return eRJPKIResults::Unknown;
}
CRJSecurityPublicKey::eRJPKIResults CRJSecurityPublicKey::Decrypt(const CRJStringByteFlex& src, CRJStringByteVar& dst, CRJStringByteVar& tmp) {
	dst.SetEmpty();
	try {
		const uint8_t* pSrc=src.GetHeadForRead();
		size_t lLen=src.GetLength();
		uint8_t* pDst=dst.GetHeadForUpdate();
		size_t lVol=dst.GetSize();
		int64_t k, ol, lKeySize=EVP_PKEY_size(THIS_PKEY);
		uint8_t* pBuf=tmp.GetHeadForUpdate();
		if (tmp.GetSize()<(size_t)(2* lKeySize)||
			!pBuf) return eRJPKIResults::NoBuf;
		while (lLen>0) {
			k=(lLen>(size_t) lKeySize)?
				lKeySize:lLen;
			ol=RSA_private_decrypt((int) k, pSrc, pBuf, EVP_PKEY_get0_RSA(THIS_PKEY), RSA_PKCS1_PADDING);
			if (ol<0) {
				SSLHandleError();
				return eRJPKIResults::NoDecrypt;
			}
			lVol-=ol;
			if (lVol<0) return eRJPKIResults::NotEnoughBuffer;
			_MEMMOVE_BYTE_POINTER(pDst, pBuf,(size_t) ol);
			pDst+=ol;
			dst.GrowSafed(ol);
			lLen-=k;
			pSrc+=k;
		}
		return eRJPKIResults::OK;
	}
	catch(...) {}
	return eRJPKIResults::Unknown;
}
bool CRJSecurityPublicKey::Gen(uint32_t bits,const char* pPrivate,const char* pPublic) {
	bool rc=false;
	RSA* rsa=nullptr;
	FILE* priv_key_file=nullptr;
	FILE* pub_key_file=nullptr;
	try {
#ifdef _rjOS_WIN
		if ((fopen_s(& priv_key_file, pPrivate,"wb"))||
			(!priv_key_file))throw eRJPKIResults::BadFileName;
		if ((fopen_s(& pub_key_file, pPublic,"wb"))||
			(!pub_key_file))throw eRJPKIResults::BadFileName;
#else 
		priv_key_file=fopen(pPrivate,"wb");
		if (!priv_key_file)throw eRJPKIResults::BadFileName;
		pub_key_file=fopen(pPublic,"wb");
		if (!pub_key_file)throw eRJPKIResults::BadFileName;
#endif 
		rsa=RSA_generate_key((int32_t) bits, RSA_F4,nullptr,nullptr);
		if (!rsa)throw eRJPKIResults::NoRSA;
		OpenSSL_add_all_ciphers();
		PEM_write_RSAPrivateKey(priv_key_file, rsa,nullptr,nullptr, 0,nullptr,nullptr);
		PEM_write_RSAPublicKey(pub_key_file, rsa);
		rc=true;
	}
	catch(...) {}
	Close(priv_key_file);
	Close(pub_key_file);
	try {
		if (rsa) RSA_free(rsa);
	}
	catch(...) {}
	return rc;
}
bool CRJSecurityPublicKey::Extract(CRJStringByteVar& s,bool bPublic) {
	try {
		if (SSLHandleIsValid()) {
			CRJSecurityBIOMemory bio;
			if (bio) {
				bPublic=(bPublic)?
					PEM_write_bio_PUBKEY((BIO*) bio.SSLHandleGet(), THIS_PKEY)==1:PEM_write_bio_PrivateKey((BIO*) bio.SSLHandleGet(), THIS_PKEY, NULL, NULL, 0, 0, NULL)==1;
				if (bPublic) {
					int64_t lLen=bio.pGetDataLen();
					if (lLen>0&&
						s.HasNeededSize((size_t) lLen)) {
#ifdef _DEBUG
						auto r=bio.pRead(s);
						if (r==lLen) {
							s.SetLength(lLen);
							return true;
						}
						else bio.SSLHandleError();
#else 
						if (bio.pRead(s)==lLen) {
							s.SetLength(lLen);
							return true;
						}
						else bio.SSLHandleError();
#endif 
					}
				}
				else bio.SSLHandleError();
			}
		}
	}
	catch(...) {}
	return false;
}
CRJSecurityBuffer::CRJSecurityBuffer(CRJSecurityPublicKey& clKey) {
	size_t s=clKey.pGetKeySize()* 2;
	uint8_t* p=(uint8_t*) CRJSecurityApplication::m_clHeap.Alloc(s);
	if (p!=nullptr) SetStorage(p, s);
}
CRJSecurityBuffer::CRJSecurityBuffer() {}
CRJSecurityBuffer::~CRJSecurityBuffer() {
	release();
}
void CRJSecurityBuffer::release() {
	if (IsDefined()) CRJSecurityApplication::m_clHeap.Free((uint8_t *)GetHeadForRead());
}
CRJSecurityPublicKeyEx::CRJSecurityPublicKeyEx(const CRJStringByte& s,bool bPublic) {
	SSLPublicKeyExAlloc(s, bPublic);
}
CRJSecurityPublicKeyEx::~CRJSecurityPublicKeyEx() {
	m_clRSA.SSLHandleAttach(nullptr);
}
void CRJSecurityPublicKeyEx::SSLPublicKeyExAlloc(const CRJStringByte& s,bool bPublic) {
	SSLPublicKeyAlloc();
	try {
		if (SSLHandleIsValid()) {
			CRJStringByteFlex st(s);
			CRJSecurityBIOMemory bio(st);
			if (bio) {
				m_clRSA.SSLRSAAlloc(bio, bPublic);
				if (m_clRSA) {
					RSA* pRSA=(RSA*) m_clRSA.SSLHandleGet();
					if (EVP_PKEY_assign_RSA(THIS_PKEY, pRSA)==1) return ;
					SSLHandleError();
				}
			}
		}
		else SSLHandleError();
	}
	catch(...) {}
	SSLHandleFree();
}

