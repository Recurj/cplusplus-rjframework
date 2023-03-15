#include "stdafx.h"
#include <recurj/rjframework/libs/www/http/replywebsocket.h>
#include <recurj/rjframework/libs/security/opensslhash.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
using namespace rjWWW;
CRJStringByte CHTTPReplyWebSocket::s_webSocketKeySuffix("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
CHTTPReplyWebSocket::CHTTPReplyWebSocket(const CRJStringByte& v):
	CHTTPReply(CHTTP::Status::INFO_SWITCH_PROTOCOL) {
	rjSecurity::CRJSecurityHashSha1 hash;
	CRJStringByteDefault h, s(CHTTP::s_clHeaderUpgrade);
	AddHeader(s.SetAt(0,'U').CatLabel(": ", 2).Cat(CHTTP::s_clUpgradeWebsocket));
	size_t l=s.Set(CHTTP::s_clHeaderConnection).SetAt(0,'C').CatLabel(": ", 2).GetLength();
	AddHeader(s.Cat(CHTTP::s_clHeaderUpgrade).SetAt(l,'U'));
	s.Set(v).Cat(CHTTPReplyWebSocket::s_webSocketKeySuffix);
	if (hash.Add(s.GetHeadForRead(), s.GetLength()).Get(h)) {
		CRJByteConvertor::Base64Encode(h, s);
		AddHeader(CHTTP::s_clHeaderWebsocketAccept, s);
	}
}
CHTTPReplyWebSocket::~CHTTPReplyWebSocket() {}

