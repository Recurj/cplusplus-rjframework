#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/byte/error.h>
#include <recurj/rjframework/libs/base/application/application.h>
using namespace rjBase;
const CRJStringByte CRJStringError::s_Warning("Warning::");
const CRJStringByte CRJStringError::s_Error("Error::");
const CRJStringByte CRJStringError::s_ErrorOs("OS ");
CRJStringError::CRJStringError() {}
CRJStringError::~CRJStringError() {}
CRJStringByte& CRJStringError::OS(int64_t lErr, int64_t lOsErr) {
	CRJStringByteDefault osErr;
	CRJStringByteLong details;
	CRJDataTools::GetErrorText(osErr, lOsErr);
	return ErrorDetails(lErr, details.Set(s_ErrorOs).CatInt64(lOsErr).CatSingle(' ').Cat(osErr));
}
CRJStringByte& CRJStringError::Error(const CRJStringByte& p) {
	return Set(s_Error).Cat(p);
}
CRJStringByte& CRJStringError::Warning(const CRJStringByte& p) {
	return Set(s_Warning).Cat(p);
}
CRJStringByte& CRJStringError::Error(int64_t lErr) {
	return Set(s_Error).CatInt64(lErr);
}
CRJStringByte& CRJStringError::ErrorDetails(int64_t lErr,const CRJStringByte& clPart) {
	return Set(s_Error).CatInt64(lErr).CatSingle('(').Cat(clPart).CatSingle(')');
}

