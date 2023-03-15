#pragma once
#include <recurj/rjframework/libs/www/http/reply.h>
namespace rjWWW {
	class RJWWW_API CHTTPReplyWebSocket:public CHTTPReply {
	public:
		CHTTPReplyWebSocket(const CRJStringByte&);
		virtual~CHTTPReplyWebSocket();
		virtual size_t GetBinaryDataLength() {
			return 0;
		};
		static CRJStringByte s_webSocketKeySuffix;
	};
}

