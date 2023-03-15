#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/unicode/var.h>
using namespace rjBase;
CRJStringUnicodeVarAssigned::CRJStringUnicodeVarAssigned():
	CRJStringUnicodeVar(0) {}
CRJStringUnicodeVarAssigned::CRJStringUnicodeVarAssigned(size_t s):
	CRJStringUnicodeVar(s) {}
CRJStringUnicodeVarAssigned::CRJStringUnicodeVarAssigned(wchar_t* p, size_t lSize):
	CRJStringUnicodeVar(lSize) {
	SetHeadAndLength(p, 0);
}
void CRJStringUnicodeVarAssigned::InitBuffer(wchar_t* p, size_t s) {
	m_sSize=s;
	SetHeadAndLength(p, 0);
}
CRJStringUnicodeVarAssigned& CRJStringUnicodeVarAssigned::SetStorage(wchar_t* p, size_t s) {
	InitBuffer(p, s);
	return *this;
}
CRJStringUnicodeVarAssignedValue::CRJStringUnicodeVarAssignedValue(wchar_t* p, size_t lSize, size_t lLen):
	CRJStringUnicodeVarAssigned(lSize) {
	SetHeadAndLength(p, lLen);
}
CRJStringUnicodeVarAssignedValue::CRJStringUnicodeVarAssignedValue(const CRJStringUnicodeVar& s):
	CRJStringUnicodeVarAssigned(s.GetSize()) {
	SetHeadAndLength(s.GetHeadForRead(), s.GetLength());
}

