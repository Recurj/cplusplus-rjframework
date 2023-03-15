#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/native.h>
using namespace rjBase;
stdString& CRJStringNativeTools::MakeStdString(const CRJString& s, stdString& str) {
	size_t l=s.GetLength();
	str.resize(l);
#ifdef _rjOS_FAMILY_UNIX
	return str.assign((const char*) s.GetHeadForRead(), l);
#endif 
#ifdef _rjOS_WIN
	return str.assign(s.GetHeadForRead(), l);
#endif 
}
stdString& CRJStringNativeTools::StdString(const CRJStringByte& s, stdString& str) {
#ifdef _rjOS_FAMILY_UNIX
	return CRJStringNativeTools::MakeStdString(s, str);
#endif 
#ifdef _rjOS_WIN
	CRJStringUnicodeExpanded v;
	return CRJStringNativeTools::MakeStdString(v.Set(s), str);
#endif 
}
stdString& CRJStringNativeTools::StdString(const CRJStringUnicode& s, stdString& str) {
#ifdef _rjOS_FAMILY_UNIX
	CRJStringByteExpanded v;
	return CRJStringNativeTools::MakeStdString(v.Set(s), str);
#endif 
#ifdef _rjOS_WIN
	return CRJStringNativeTools::MakeStdString(s, str);
#endif 
}

