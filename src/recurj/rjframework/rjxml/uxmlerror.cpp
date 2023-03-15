#include "stdafx.h"
#include <recurj/rjframework/libs/xml/xmlerror.h>
using namespace rjXml;
CRJXMLError::CRJXMLError():
	CRJResult(DataResutRegionXml) {
	m_sRegion=DataResutRegionXml;
	ResetXMLError();
}
CRJXMLError::~CRJXMLError() {}
void CRJXMLError::ResetXMLError() {
	m_lResult=0;
	m_lLine=m_lPos=-1;
}
bool CRJXMLError::SetXMLError(eUXMLError lErr, int64_t lLine, int64_t lPos) {
	m_lResult=static_cast<int64_t>(lErr);
	m_lLine=lLine;
	m_lPos=lPos;
	return false;
}
bool CRJXMLError::SetError(eUXMLError lErr) {
	m_lResult=static_cast<int64_t>(lErr);
	return false;
}
CRJStringByteVar& CRJXMLError::GetErrorMsg(CRJStringByteVar& s) const {
	CRJResult::GetErrorMsg(s);
	if (m_lLine>=0) s.CatLabel("::L ").CatInt64(m_lLine);
	if (m_lPos>=0) s.CatLabel("::P ").CatInt64(m_lPos);
	return s;
}

