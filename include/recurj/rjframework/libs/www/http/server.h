#pragma once
#include <recurj/rjframework/libs/base/tasks/task.h>
#include <recurj/rjframework/libs/www/url.h>
#include <recurj/rjframework/libs/www/http/defs.h>
namespace rjWWW {
	class CHTTPReply;
	class CHTTPReplyJSON;
	class RJWWW_API CHTTPServer:public CHTTP {
		friend class CHTTPReply;
	public:
		CHTTPServer();
		virtual~CHTTPServer();
		bool HTTPReply(CHTTPReply&);
		virtual CLang& GetLang()=0;
		virtual CRJByteWriter& HTTPGetWriter()=0;
		virtual bool HTTPSendWriter(CRJByteWriter&)=0;
		virtual bool OnHTTPRecvTotal()=0;
		virtual void HTTPExplainError(CHTTPReplyJSON&) {}
		static void SetVersion(const CRJStringByte&);
	protected:
		virtual void OnHTTPReset();
		virtual bool OnHTTPRecvFirstLine(CRJStringByteFlex&);
		virtual bool OnHTTPRecvBinaryData(size_t);
		virtual bool OnHTTPError(int64_t, size_t);
	public:
		static CRJStringByte s_clHeaderReplyServer;
		static CRJStringByte s_clMethodGet;
		static CRJStringByte s_clMethodHead;
		static CRJStringByte s_clMethodPost;
		static CRJStringByte s_clMethodPut;
		static CRJStringByte s_clMethodTrace;
		static CRJStringByte s_clMethodOptions;
		static CRJStringByte s_clMethodDelete;
		static CRJStringByte s_clSessionId;
		static CRJStringByte s_clLangCookie;
		static CRJStringByte s_clContentEncoding;
		static CRJStringByte s_htmlServerError;
	protected:
		static CRJStringByteVarBuffer<8>m_clHTTPServerVersion;
		Method m_lHTTPMethod;
		CRJURL m_clHTTPURL;
	};
}

