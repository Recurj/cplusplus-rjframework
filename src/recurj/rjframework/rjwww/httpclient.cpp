#include "stdafx.h"
#include <recurj/rjframework/libs/www/http/client.h>
using namespace rjWWW;
CHTTPClient::CHTTPClient() {}
CHTTPClient::~CHTTPClient(void) {}
void CHTTPClient::OnHTTPReset() {
	CHTTP::OnHTTPReset();
	m_clHTTPResult.SetEmpty();
}
bool CHTTPClient::OnHTTPRecvFirstLine(CRJStringByteFlex& clInput) {
	OnHTTPReset();
	if (!clInput.Trim(' ').WasFoundTopAndMovedHead2Var(' ', m_clHTTPMessage.m_clVersionRecv)) return OnHTTPError(BAD_VERSION_IN_STATUS_LINE, Status::CLIENT_BAD_REQUEST);
	if (!clInput.TrimLeft(' ').WasFoundTopAndMovedTail2Var(' ', m_clHTTPComments)) return OnHTTPError(BAD_COMMENT_IN_STATUS_LINE, Status::CLIENT_BAD_REQUEST);
	m_clHTTPResult.Set(clInput);
	return OnHTTPAnalyzeVersion();
}
bool CHTTPClient::OnHTTPError(int64_t lError, size_t) {
	m_clStorage.SetVarRegionError(lError);
	return OnHTTPRecvFailure();
}

