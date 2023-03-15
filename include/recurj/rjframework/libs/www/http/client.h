#pragma once
#include <recurj/rjframework/libs/www/http/defs.h>
namespace rjWWW {
	class RJWWW_API CHTTPClient:public CHTTP {
	public:
		CHTTPClient();
		virtual~CHTTPClient();
	protected:
		virtual void OnHTTPReset();
		virtual bool OnHTTPRecvFirstLine(CRJStringByteFlex&);
		virtual bool OnHTTPError(int64_t, size_t);
	protected:
		CRJStringByteDefault m_clHTTPResult;
		CRJStringByteExpanded m_clHTTPComments;
	};
}

