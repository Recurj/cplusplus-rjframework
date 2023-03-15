#include "stdafx.h"
#include <recurj/rjframework/libs/security/deskey.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/security/destools.h>
using namespace rjSecurity;
CRJDESKey::CRJDESKey() {
	_MEMCLR_BYTE_ARRAY(m_bKey);
	m_lKeyLen=0;
	m_bMix=false;
}
CRJDESKey::CRJDESKey(const uint8_t* b, size_t l) {
	SetKey(b, l);
	m_bMix=false;
}
CRJDESKey::CRJDESKey(const uint8_t* b, size_t l,bool bMix) {
	SetKey(b, l);
	m_bMix=bMix;
}
CRJDESKey::~CRJDESKey() {}
uint8_t* CRJDESKey::GetKey(size_t& len) {
	len=m_lKeyLen;
	return m_bKey;
}
bool CRJDESKey::SetKey(const uint8_t* b, size_t l) {
	try {
		m_lKeyLen=(l>sizeof(m_bKey))?
			sizeof(m_bKey):
			l;
		if (m_lKeyLen>0) _MEMMOVE_BYTE_POINTER(m_bKey, b, m_lKeyLen);
		return true;
	}
	catch(...) {}
	return false;
}
bool CRJDESKey::Decrypt(uint8_t* bData, size_t len) const {
	try {
		if (m_lKeyLen==cUSDESKeyLong) {
			if (m_bMix) {
				CRJSecurityDesTools::DoDecryptTripleDesBlock(bData, len, m_bKey);
			}
			else {
				CRJSecurityDesTools::DoDecryptTripleDesBlockSimple(bData, len, m_bKey);
			}
			return true;
		}
		else if (m_lKeyLen==cUSDESKeyShort) {
			CRJSecurityDesTools::DoDecryptDesBlock(bData, len, m_bKey);
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJDESKey::Encrypt(uint8_t* bData, size_t len) const {
	try {
		if (m_lKeyLen==cUSDESKeyLong) {
			if (m_bMix) {
				CRJSecurityDesTools::DoEncryptTripleDesBlock(bData, len, m_bKey);
			}
			else {
				CRJSecurityDesTools::DoEncryptTripleDesBlockSimple(bData, len, m_bKey);
			}
			return true;
		}
		else if (m_lKeyLen==cUSDESKeyShort) {
			CRJSecurityDesTools::DoEncryptDesBlock(bData, len, m_bKey);
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJDESKey::GetMixMode() const {
	return m_bMix;
}
void CRJDESKey::SetMixMode(bool b) {
	m_bMix=b;
}

