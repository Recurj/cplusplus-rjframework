#include "stdafx.h"
#include <recurj/rjframework/libs/base/datetime/dtimebinary.h>
#include <recurj/rjframework/libs/www/http/reply.h>
#include <recurj/rjframework/libs/www/http/server.h>
using namespace rjWWW;
CHTTPReply::CHTTPReply():
	m_lStatus(CHTTP::Status::SUCCESS_OK) {}
CHTTPReply::CHTTPReply(size_t lStatus):
	m_lStatus(lStatus) {}
CHTTPReply::~CHTTPReply() {}
bool CHTTPReply::GetHeaders(CRJByteWriter& clWriter) {
	return clWriter.Cat(*this).CatCRLF().OK();
}
CHTTPReplyText::CHTTPReplyText(const CRJStringByte& stext) {
	m_szText.Set(stext);
}
bool CHTTPReplyText::GetBinaryData(CRJByteWriter& clWriter, size_t) {
	return clWriter.Cat(m_szText).OK();
}
CHTTPReply& CHTTPReply::AddHeader(const CRJStringByte& s) {
	Cat(s).CatCRLF();
	return *this;
}
CHTTPReply& CHTTPReply::AddHeader(const CRJStringByte& s,const CRJStringByte& v) {
	Cat(s).Cat(v).CatCRLF();
	return *this;
}
CHTTPReply& CHTTPReply::AddSessionId(const CRJStringByte& s) {
	Cat(CHTTP::s_clHeaderSetCookie).Cat(CHTTPServer::s_clSessionId).CatSingle('=').Cat(s).CatCRLF();
	return *this;
}
CHTTPReply& CHTTPReply::RemoveSessionId(const CRJStringByte& s) {
	return RemoveCookie(CHTTPServer::s_clSessionId, s);
}
CHTTPReply& CHTTPReply::SetLangId(const CRJStringByte& s) {
	if (s.GetLength()<2) return RemoveCookie(CHTTPServer::s_clLangCookie, s);
	return AddLanguage(s);
}
CHTTPReply& CHTTPReply::AddLanguage(const CRJStringByte& s) {
	CRJStringByteVarBuffer<32>str;
	CRJDTimeTools::DT2StrCookie(str, CRJDTimeTools::GetDTime()+CRJDTimeYear);
	Cat(CHTTP::s_clHeaderSetCookie).Cat(CHTTPServer::s_clLangCookie).CatSingle('=').Cat(s).Cat(CHTTP::s_clHeaderSetCookieExpires).Cat(str).CatCRLF();
	return *this;
}
CHTTPReply& CHTTPReply::RemoveCookie(const CRJStringByte& szCookie,const CRJStringByte& szValue) {
	CRJStringByteVarBuffer<32>str;
	CRJDTimeTools::DT2StrCookie(str, CRJDTimeTools::GetDTime()-CRJDTimeHour);
	Cat(CHTTP::s_clHeaderSetCookie).Cat(szCookie).CatSingle('=').Cat(szValue).Cat(CHTTP::s_clHeaderSetCookieExpires).Cat(str).CatCRLF();
	return *this;
}

