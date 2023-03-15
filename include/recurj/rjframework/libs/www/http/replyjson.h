#pragma once
#include <recurj/rjframework/libs/www/json/builder.h>
#include <recurj/rjframework/libs/www/http/reply.h>
namespace rjWWW {
	class RJWWW_API CHTTPReplyJSON:public CHTTPReply {
	public:
		CHTTPReplyJSON();
		CHTTPReplyJSON(size_t);
		CHTTPReplyJSON(int64_t);
		virtual~CHTTPReplyJSON() {}
		virtual bool GetHeaders(CRJByteWriter&);
		virtual size_t GetBinaryDataLength() {
			return m_clJSON.GetLength();
		};
		CJSONBuilder& GetJSON() {
			return m_clJSON;
		}
		bool ExplainError(const char*);
		bool ExplainError(const CRJStringByte&);
		bool ExplainErrorDefault();
		inline bool IsValidData() const {
			return m_clJSON.OK();
		}
		inline int64_t GetErrorCode() const {
			return m_lJSONErrorCode;
		}
	protected:
		static CRJStringByte s_clResult;
		static CRJStringByte s_clError;
		CJSONBuilder m_clJSON;
		int64_t m_lJSONErrorCode;
	};
}

