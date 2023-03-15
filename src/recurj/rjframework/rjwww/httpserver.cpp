#include "stdafx.h"
#include <recurj/rjframework/libs/www/http/server.h>
#include <recurj/rjframework/libs/www/http/reply.h>
#include <recurj/rjframework/libs/www/http/replyjson.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/application/application.h>
using namespace rjWWW;
CRJStringByte CHTTPServer::s_clHeaderReplyServer("Server: RUN24\x0d\x0a");
CRJStringByte CHTTPServer::s_clMethodGet("GET");
CRJStringByte CHTTPServer::s_clMethodHead("HEAD");
CRJStringByte CHTTPServer::s_clMethodPost("POST");
CRJStringByte CHTTPServer::s_clMethodPut("PUT");
CRJStringByte CHTTPServer::s_clMethodTrace("TRACE");
CRJStringByte CHTTPServer::s_clMethodOptions("OPTIONS");
CRJStringByte CHTTPServer::s_clMethodDelete("DELETE");
CRJStringByte CHTTPServer::s_clSessionId("SessionId");
CRJStringByte CHTTPServer::s_clLangCookie("Language");
CRJStringByte CHTTPServer::s_clContentEncoding("Content-Encoding: gzip");
CRJStringByte CHTTPServer::s_htmlServerError("html/serverinternal.html");
CRJStringByteVarBuffer<8>CHTTPServer::m_clHTTPServerVersion;
void CHTTPServer::SetVersion(const CRJStringByte& s) {
	m_clHTTPServerVersion.Set(s);
}
CHTTPServer::CHTTPServer(){}
CHTTPServer::~CHTTPServer(void) {}
bool CHTTPServer::OnHTTPError(int64_t, size_t lRemoteError) {
	CHTTPReply clReply(lRemoteError);
	HTTPReply(clReply);
	return OnHTTPRecvFailure();
}
void CHTTPServer::OnHTTPReset() {
	CHTTP::OnHTTPReset();
	m_lHTTPMethod=CHTTP::Method::UNKNOWN;
	m_clHTTPURL.SetEmpty();
}
bool CHTTPServer::OnHTTPRecvFirstLine(CRJStringByteFlex& clInput) {
	CRJStringByteVarBuffer<16>clHTTPMethod;
	OnHTTPReset();
	m_clHTTPMessage.Restart();
	if (!clInput.Trim(' ').WasFoundTopAndMovedHead2Var(' ', clHTTPMethod)) return OnHTTPError(BAD_METHOD_IN_STATUS_LINE, Status::CLIENT_BAD_REQUEST);
	if (!clInput.TrimLeft(' ').WasFoundTopAndMovedHead2Var(' ', m_clHTTPURL)) return OnHTTPError(BAD_URL_IN_STATUS_LINE, Status::CLIENT_BAD_REQUEST);
	m_clHTTPURL.Decode().Replace('\\','/').Trim(' ');
	m_clHTTPMessage.m_clVersionRecv.Set(clInput.TrimLeft(' '));
	if (clHTTPMethod==CHTTPServer::s_clMethodGet) m_lHTTPMethod=Method::GET;
	else if (clHTTPMethod==CHTTPServer::s_clMethodPost) m_lHTTPMethod=Method::POST;
	else if (clHTTPMethod==CHTTPServer::s_clMethodHead) m_lHTTPMethod=Method::HEAD;
	else if (clHTTPMethod==CHTTPServer::s_clMethodPut) m_lHTTPMethod=Method::PUT;
	else if (clHTTPMethod==CHTTPServer::s_clMethodTrace) m_lHTTPMethod=Method::TRACE;
	else if (clHTTPMethod==CHTTPServer::s_clMethodOptions) m_lHTTPMethod=Method::OPTIONS;
	else if (clHTTPMethod==CHTTPServer::s_clMethodDelete) m_lHTTPMethod=Method::DEL;
	else return OnHTTPError(StreamError::BAD_METHOD, Status::SERVER_NOT_IMPLEMENTED);
	return OnHTTPAnalyzeVersion();
}
bool CHTTPServer::HTTPReply(CHTTPReply& clReply) {
	size_t s;
	CRJByteWriter& clWriter=HTTPGetWriter();
	int64_t r=clReply.GetStatus();
	clWriter.ResetWriter().Cat(m_clHTTPServerVersion).CatSingle(' ').CatInt64(r).CatSingle(' ').Cat(GetHTTPStatusCode(r)).CatCRLF().Cat(CHTTPServer::s_clHeaderReplyServer);
	s=clReply.GetBinaryDataLength();
	clWriter.Cat(CHTTPServer::s_clHeaderContentLength).CatData((const uint8_t*)": ", 2).CatSize(s).CatCRLF();
	return clReply.GetHeaders(clWriter)&&
		clReply.GetBinaryData(clWriter, s)&&
		HTTPSendWriter(clWriter);
}
bool CHTTPServer::OnHTTPRecvBinaryData(size_t lSize) {
	return m_clHTTPMessage.m_clBinaryData.CatData(m_clStorage.GetHeadForRead(), lSize).OK();
}

