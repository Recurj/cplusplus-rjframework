#include "stdafx.h"
#include <recurj/rjframework/libs/security/openssl.h>
#include <recurj/rjframework/libs/security/opensslbio.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#define THIS_BIO ((BIO *)m_hSSLHandle)
using namespace rjSecurity;
CRJSecurityBIO::CRJSecurityBIO(openSSLHandle h):
	CRJSecurityHandle(h) {}
CRJSecurityBIO::~CRJSecurityBIO() {
	SSLHandleFree();
}
void CRJSecurityBIO::SSLHandleFree() {
	try {
		if (SSLHandleIsValid()) {
			BIO_free(THIS_BIO);
		}
	}
	catch(...) {}
	CRJSecurityHandle::SSLHandleFree();
}
int64_t CRJSecurityBIO::pGetDataLen() {
	return BIO_pending(THIS_BIO);
}
int64_t CRJSecurityBIO::pRead(CRJStringByteVar& s) {
	return BIO_read(THIS_BIO, s.GetHeadForUpdate(),(int) s.GetSize());
}
void CRJSecurityBIO::SSLBIOReset() {
	try {
		if (SSLHandleIsValid()) {
			BIO_reset(THIS_BIO);
		}
	}
	catch(...) {}
}
bool CRJSecurityBIO::SSLBIOAddDataSafed(CRJStringByteFlex& st) {
	int l=(int) st.GetLength();
	return BIO_write(THIS_BIO, st.GetHeadForRead(), l) == l;
}
bool CRJSecurityBIO::SSLBIOHasDataSafed(CRJStringByteFlex& st) {
	uint8_t* p;
	int l=BIO_get_mem_data(THIS_BIO,& p);
	if (l<=0) return false;
	st.SetFromData(p,(size_t) l);
	return true;
}
CRJSecurityBIOMemory::CRJSecurityBIOMemory():
	CRJSecurityBIO(nullptr) {
	SSLBIOMemoryAlloc();
}
CRJSecurityBIOMemory::CRJSecurityBIOMemory(const CRJStringByteFlex& st):
	CRJSecurityBIO(nullptr) {
	SSLBIOMemoryAlloc(st);
}
CRJSecurityBIOMemory::~CRJSecurityBIOMemory() {}
void CRJSecurityBIOMemory::SSLBIOMemoryAlloc() {
	try {
		SSLHandleAttach(BIO_new(BIO_s_mem()));
	}
	catch(...) {}
}
void CRJSecurityBIOMemory::SSLBIOMemoryAlloc(const CRJStringByteFlex& st) {
	try {
		SSLHandleAttach(BIO_new_mem_buf(st.GetHeadForRead(), (int)st.GetLength()));
	}
	catch(...) {}
}

