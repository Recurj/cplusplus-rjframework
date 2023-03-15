#pragma once
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/www/http/defs.h>
namespace rjWWW {
	class RJWWW_API CHTTPReply:public CRJStringByteExpanded {
	public:
		CHTTPReply();
		CHTTPReply(size_t);
		virtual~CHTTPReply();
		virtual size_t GetBinaryDataLength() {
			return 0;
		};
		virtual bool GetBinaryData(CRJByteWriter&, size_t) {
			return true;
		}
		virtual bool GetHeaders(CRJByteWriter&);
		CHTTPReply& AddHeader(const CRJStringByte&);
		CHTTPReply& AddHeader(const CRJStringByte&,const CRJStringByte&);
		CHTTPReply& AddSessionId(const CRJStringByte&);
		CHTTPReply& RemoveSessionId(const CRJStringByte&);
		CHTTPReply& SetLangId(const CRJStringByte&);
		inline size_t GetStatus() const {
			return m_lStatus;
		}
	protected:
		CHTTPReply& AddLanguage(const CRJStringByte&);
		CHTTPReply& RemoveCookie(const CRJStringByte&,const CRJStringByte&);
		size_t m_lStatus;
	};
	class CHTTPReplyText:public CHTTPReply {
	public:
		CHTTPReplyText(const CRJStringByte&);
		virtual~CHTTPReplyText() {}
		virtual size_t GetBinaryDataLength() {
			return m_szText.GetLength();
		};
		virtual bool GetBinaryData(CRJByteWriter&, size_t);
		void SetText(const CRJStringByte& s) {
			m_szText.Set(s);
		}
	protected:
		CRJStringByteExpanded m_szText;
	};
}

