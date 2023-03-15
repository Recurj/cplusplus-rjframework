#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <string.h>
#include <ctype.h>
using namespace rjBase;
CRJStringByteVar& CRJStringByteVar::CalcCLength() {
	size_t s=GetSize();
	if (s>0) {
		uint8_t* p=GetHeadForUpdate();
		p[s-1]=0x00;
		SetLength(strlen((const char*) p));
	}
	return *this;
}
CRJStringByteVar& CRJStringByteVar::Clean() {
	m_sLength=0;
	_MEMSET_BYTE_POINTER(GetHeadForUpdate(), 0x00, m_sSize);
	return *this;
}
CRJStringByteVar& CRJStringByteVar::UpperCase() {
	size_t i;
	uint8_t* pData=GetHeadForUpdate();
	for (i=0; i<m_sLength; i++, pData++)* pData=(uint8_t)::toupper(* pData);
	return *this;
}
CRJStringByteVar& CRJStringByteVar::LowerCase() {
	size_t i;
	uint8_t* pData=GetHeadForUpdate();
	for (i=0; i<m_sLength; i++, pData++)* pData=(uint8_t)::tolower(* pData);
	return *this;
}
CRJStringByteVar& CRJStringByteVar::SetLabel(const char* p) {
	return SetLabel(p, strlen(p));
}
CRJStringByteVar& CRJStringByteVar::CatLabel(const char* p) {
	return CatLabel(p, strlen(p));
}

