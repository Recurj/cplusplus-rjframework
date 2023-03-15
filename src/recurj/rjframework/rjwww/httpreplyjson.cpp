#include "stdafx.h"
#include <recurj/rjframework/libs/www/http/replyjson.h>
#include <recurj/rjframework/libs/www/http/server.h>
using namespace rjWWW;
CRJStringByte CHTTPReplyJSON::s_clResult("{\"Result\":0,");
CRJStringByte CHTTPReplyJSON::s_clError(",\"Error\":\"");
CHTTPReplyJSON::CHTTPReplyJSON() {
	m_lJSONErrorCode=0;
	m_clJSON.Set(CHTTPReplyJSON::s_clResult);
}
CHTTPReplyJSON::CHTTPReplyJSON(size_t s) {
	m_clJSON.HasNeededSize(s);
	m_lJSONErrorCode=0;
	m_clJSON.Set(CHTTPReplyJSON::s_clResult);
}
CHTTPReplyJSON::CHTTPReplyJSON(int64_t rc) {
	m_lJSONErrorCode=rc;
	m_clJSON.Set(CHTTPReplyJSON::s_clResult).TruncRight(2).CatInt64(rc);
}
bool CHTTPReplyJSON::ExplainError(const char* pErr) {
	return m_clJSON.Cat(CHTTPReplyJSON::s_clError).CatLabel(pErr).CatLabel("\",", 2).OK();
}
bool CHTTPReplyJSON::ExplainError(const CRJStringByte& clErr) {
	return m_clJSON.Cat(CHTTPReplyJSON::s_clError).Cat(clErr).CatLabel("\",", 2).OK();
}
bool CHTTPReplyJSON::GetHeaders(CRJByteWriter& clWriter) {
	if (m_clJSON.OK()) {
		m_clJSON.SetAt(m_clJSON.GetLength()-1,'}');
		return clWriter.Cat(CHTTP::s_clContentType).Cat(CHTTP::s_clContentMimeJSON).CatCRLF().Cat(*this).CatCRLF().Cat(m_clJSON).OK();
	}
	return false;
}
bool CHTTPReplyJSON::ExplainErrorDefault() {
	CRJStringByteVarBuffer<32>clTemp;
	clTemp.SetSingle('#').CatInt64(GetErrorCode());
	return ExplainError(clTemp);
}

