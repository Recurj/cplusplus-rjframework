#pragma once
#include <recurj/rjframework/libs/base/strings/native.h>
#include <recurj/rjframework/libs/base/datatools/result.h>
using namespace rjBase;
namespace rjXml {
	class RJXML_API CRJXMLError:public CRJResult {
	public:
		enum 
		class eUXMLError:int64_t {
			XML_ERROR_NO=0, XML_ERROR_NO_MEMORY, XML_ERROR_EXCEPTION, XML_PARSER_ERROR_BAD_SYMBOL,
			XML_PARSER_ERROR_NOT_SINGLE_ROOT, XML_PARSER_ERROR_NOT_SAME_CLOSE_TAG, XML_PARSER_ERROR_NO_ROOT_TAG,
			XML_PARSER_ERROR_TO_BIG_DATA, XML_PARSER_ERROR_NOT_CLOSED_TAG0, XML_PARSER_ERROR_NOT_CLOSED_TAG1,
			XML_PARSER_ERROR_NOT_CLOSED_ATTR, XML_PARSER_ERROR_NO_ATTR_TAG, XML_PARSER_ERROR_NO_CONTENT_TAG,
			XML_PARSER_ERROR_NO_FILE_OPEN, XML_PARSER_ERROR_NO_FILE_READ, XML_PARSER_ERROR_NO_FILE_MOVE,
			XML_PARSER_ERROR_NO_FILE_SAVE, XML_PARSER_ERROR_TAG_UNEXPECTED, XML_PARSER_ERROR_DATA
		};
		CRJXMLError();
		virtual~CRJXMLError();
		virtual CRJStringByteVar& GetErrorMsg(CRJStringByteVar&) const;
		void ResetXMLError();
		bool SetXMLError(eUXMLError, int64_t, int64_t);
		bool SetError(eUXMLError);
		bool NoMemory() {
			return SetError(eUXMLError::XML_ERROR_NO_MEMORY);
		};
		bool OnException() {
			return SetError(eUXMLError::XML_ERROR_EXCEPTION);
		};
		bool OK() const {
			return m_lResult==static_cast<int64_t>(eUXMLError::XML_ERROR_NO);
		}
	protected:
		int64_t m_lLine, m_lPos;
	};
}

