#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/result.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
using namespace rjBase;
CRJResult::CRJResult()noexcept {
	m_sRegion=0;
	m_lResult=0;
}
CRJResult::CRJResult(size_t region)noexcept {
	m_sRegion=region;
	m_lResult=0;
}
CRJResult::CRJResult(size_t region, int64_t rc)noexcept {
	m_sRegion=region;
	m_lResult=rc;
}
CRJStringByteVar& CRJResult::GetErrorMsg(CRJStringByteVar& s) const {
	return s.CatSize(m_sRegion).CatLabel(":").CatInt64(m_lResult);
}
CRJResultWithSize::CRJResultWithSize()noexcept {
	m_sValue=0;
}
CRJResultWithSize::CRJResultWithSize(size_t region, int64_t rc)noexcept:
CRJResult(region, rc) {
	m_sValue=0;
}
CRJStringByteVar& CRJResultWithSize::GetErrorMsg(CRJStringByteVar& s) const {
	return CRJResult::GetErrorMsg(s).CatLabel(":").CatSize(m_sValue);
}

