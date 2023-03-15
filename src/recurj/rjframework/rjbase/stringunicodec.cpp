#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/unicode/var.h>
#include <wchar.h>
#include <cwctype>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/base/strings/unicode/allocated.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
using namespace rjBase;
CRJStringUnicodeVar& CRJStringUnicodeVar::CalcCLength() {
	size_t s=GetSize();
	if (s>0) {
		wchar_t* p=GetHeadForUpdate();
		p[s-1]=0x00;
		SetLength(wcslen(p));
	}
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::UpperCase() {
	size_t i;
	wchar_t* pData=GetHeadForUpdate();
	for (i=0; i<m_sLength; i++, pData++)* pData=::towupper(* pData);
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::LowerCase() {
	size_t i;
	wchar_t* pData=GetHeadForUpdate();
	for (i=0; i<m_sLength; i++, pData++)* pData=::towlower(* pData);
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetLabel(const wchar_t* p) {
	return SetData(p, wcslen(p));
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetLabel(const char* p) {
	CRJStringByte label((char*) p);
	return CRJEncoding::translateUtf8ToUnicode(label,*this);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::CatLabel(const wchar_t* p) {
	return CatData(p, wcslen(p));
}
CRJStringUnicodeVar& CRJStringUnicodeVar::CatLabel(const char* p) {
	CRJStringByte label((char*) p);
	CRJStringUnicodeExpanded tmp;
	return Cat(tmp.Set(label));
}
CRJStringUnicodeVar& CRJStringUnicodeVar::MakeCLabel() {
	if (m_sLength<m_sSize)*((wchar_t*) GetTail())=0;
	else if (m_sSize>0) {
		m_sLength=m_sSize-1;
		*((wchar_t*) GetTail())=0;
		SetStringVarError(eUChainErrors::WasTruncated);
	}
	else {
		SetStringVarError(eUChainErrors::BadSize);
#ifdef _DEBUG
		CRJDataTools::DebugBreak();
#endif 
	}
	return *this;
}

