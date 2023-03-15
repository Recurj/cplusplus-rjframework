#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/security/openssl.h>
#include <recurj/rjframework/libs/security/opensslapplication.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionsecurity.h>
using namespace rjSecurity;
RJ_ALLOCATOR_ANY_OUTSIDE(CRJSecurityApplication,Allocator::Region::Security, AllocatorSecurity::App);
static void SerenarePublicKeysFree(void* p,const char*,int) {
	CRJSecurityApplication::m_clHeap.Free((uint8_t *)p);
}
std::mutex** CRJSecurityApplication::m_pTableOfCS=nullptr;
size_t CRJSecurityApplication::m_lCount=0;
int CRJSecurityApplication::m_lSSLAppDataIndex=-1;
void SSLApplicationLockingCallback(int mode,int lIndex,const char*,int) {
	if ((lIndex>=0)&&
		((size_t) lIndex<CRJSecurityApplication::m_lCount)&&
		(CRJSecurityApplication::m_pTableOfCS)&&
		(CRJSecurityApplication::m_pTableOfCS[lIndex])) {
		if (mode& CRYPTO_LOCK) CRJSecurityApplication::m_pTableOfCS[lIndex]->lock();
		else CRJSecurityApplication::m_pTableOfCS[lIndex]->unlock();
	}
}
unsigned long SSLApplicationIdCallback() {
	return (unsigned long) CRJTask::ThreadGetId();
}
static void* SerenarePublicKeysMalloc(size_t s,const char*,int) {
	return CRJSecurityApplication::m_clHeap.Alloc(CRJDataTools::SizeAlignPower(s, 5));
}
static void* SerenarePublicKeysRealloc(void* p, size_t s,const char* pFile,int line) {
	void* rc=nullptr;
	try {
		if (s<=0) CRJSecurityApplication::m_clHeap.Free((uint8_t*)p);
		else {
			void* q=SerenarePublicKeysMalloc(s, pFile, line);
			if (q) {
				size_t s0=(size_t) CRJSecurityApplication::m_clHeap.GetSize((uint8_t*)p);
				if ((s0>0)&&
					(p)) {
					if (s0>s) s0=s;
					if (s0) memcpy(q, p, s0);
				}
				CRJSecurityApplication::m_clHeap.Free((uint8_t*)p);
				rc=q;
			}
		}
	}
	catch(...) {}
	return rc;
}
CRJSecurityApplication::CRJSecurityApplication() {
	m_bReady=PrepareOpenSSLApplication();
	try {
		SSL_library_init();
		SSL_load_error_strings();
		ERR_load_crypto_strings();
		CRJSecurityApplication::m_lSSLAppDataIndex=SSL_CTX_get_ex_new_index(0,nullptr,nullptr,nullptr,nullptr);
	}
	catch(...) {}
}
CRJSecurityApplication::~CRJSecurityApplication() {
	CloseOpenSSLApplication();
}
bool CRJSecurityApplication::PrepareOpenSSLApplication() {
	try {
		size_t i;
		size_t l;
		CRJSecurityApplication::m_lCount=(size_t) CRYPTO_num_locks();
		l=CRJSecurityApplication::m_lCount*sizeof(void*);
		CRJSecurityApplication::m_pTableOfCS=(std::mutex**) OPENSSL_malloc(l);
		if (CRJSecurityApplication::m_pTableOfCS) {
			_MEMCLR_BYTE_POINTER(CRJSecurityApplication::m_pTableOfCS, l);
			for (i=0; i<CRJSecurityApplication::m_lCount; i++) {
				CRJSecurityApplication::m_pTableOfCS[i]=new std::mutex;
				if (!CRJSecurityApplication::m_pTableOfCS[i]) return false;
			}
			CRYPTO_set_locking_callback(SSLApplicationLockingCallback);
			CRYPTO_set_id_callback(SSLApplicationIdCallback);
			CRYPTO_set_mem_functions(SerenarePublicKeysMalloc, SerenarePublicKeysRealloc, SerenarePublicKeysFree);
			return true;
		}
	}
	catch(...) {}
	return false;
}
void CRJSecurityApplication::CloseOpenSSLApplication() {
	try {
		size_t i;
		std::mutex* p;
		CRYPTO_set_mem_functions(nullptr,nullptr,nullptr);
		CRYPTO_set_id_callback(nullptr);
		CRYPTO_set_locking_callback(nullptr);
		if (CRJSecurityApplication::m_pTableOfCS) {
			for (i=0; i<CRJSecurityApplication::m_lCount; i++) {
				p=CRJSecurityApplication::m_pTableOfCS[i];
				if (p) {
					delete p;
					CRJSecurityApplication::m_pTableOfCS[i]=nullptr;
				}
			}
			OPENSSL_free(CRJSecurityApplication::m_pTableOfCS);
		}
	}
	catch(...) {}
	CRJSecurityApplication::m_pTableOfCS=nullptr;
}

