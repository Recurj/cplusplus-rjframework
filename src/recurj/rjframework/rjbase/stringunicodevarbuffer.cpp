#include "stdafx.h"
#include <local/rj/libs/base/strings/unicode/data.h>
using namespace rjBase;
CRJStringUnicodeData::CRJStringUnicodeData(wchar_t* p, size_t lSize, size_t lLen):
	CRJStringUnicodeVar(lSize) {
	SetBuffer(p, lLen);
}
CRJStringUnicodeData::CRJStringUnicodeData(wchar_t* p, size_t lSize):
	CRJStringUnicodeVar(lSize) {
	SetBuffer(p, 0);
}
void CRJStringUnicodeData::SetBuffer(wchar_t* p, size_t lLen) {
	if (p) {
		SetHead(p);
		SetLengthSafed(lLen);
	}
	else SetUndefined();
}

