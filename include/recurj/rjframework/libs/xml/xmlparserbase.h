#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/xml/xmldefs.h>
#include <recurj/rjframework/libs/xml/xmlerror.h>
namespace rjXml {
	constexpr size_t SIZE_XML_PARSER_ATTR_DEFAULT=1024;
	constexpr uint8_t XML_PARSER_SYMBOL_TAG_OPEN='<';
	constexpr uint8_t XML_PARSER_SYMBOL_TAG_CLOSE='>';
	constexpr uint8_t XML_PARSER_SYMBOL_TAG_CLOSE_PREFIX='/';
	constexpr uint8_t XML_PARSER_SYMBOL_PROCESS='?';
	constexpr uint8_t XML_PARSER_SYMBOL_EX='!';
	constexpr uint8_t XML_PARSER_SYMBOL_COMMENT='-';
	constexpr uint8_t XML_PARSER_SYMBOL_CDATA_START='[';
	constexpr uint8_t XML_PARSER_SYMBOL_CDATA_END=']';
	constexpr uint8_t XML_PARSER_SYMBOL_AMPERSAND='&';
	constexpr uint8_t XML_PARSER_SYMBOL_ATTR_SEPARATOR='=';
	constexpr uint8_t XML_PARSER_SYMBOL_VALUE_SEPARATOR='"';
	constexpr uint8_t XML_PARSER_SYMBOL_NEW_LINE=0x00;
	constexpr uint8_t XML_PARSER_SYMBOL_SPACE=0x20;
	constexpr int64_t SIZE_XMLPARSER_ENCODING=2;
	class RJXML_API CRJXMLParserObserver {
	public:
		CRJXMLParserObserver()noexcept {}
		virtual~CRJXMLParserObserver() {}
		virtual bool OnInstruction(const CRJStringByte&) {
			return true;
		}
		virtual bool OnDoctype(const CRJStringByte&) {
			return true;
		}
		virtual bool OnCDATA(const CRJStringByte&) {
			return true;
		}
		virtual bool OnComment(const CRJStringByte&) {
			return true;
		}
		virtual bool OnTagEntry(const CRJStringByte&) {
			return true;
		}
		virtual bool OnTagCloseSelf(const CRJStringByte& s) {
			return OnTagClose(s);
		}
		virtual bool OnTagClose(const CRJStringByte&) {
			return true;
		}
		virtual bool OnTagContinue(const CRJStringByte&) {
			return true;
		}
		virtual bool OnTagContent(const CRJStringByte&, CRJStringByteVar&) {
			return true;
		};
		virtual bool OnTagAttribute(const CRJStringByte&,const CRJStringByte&, CRJStringByteVar&) {
			return true;
		}
		virtual bool OnTagAttributeWithoutData(const CRJStringByte&,const CRJStringByte&) {
			return true;
		}
		virtual bool AllowAttributeWithoutData() {
			return false;
		}
		CRJXMLError m_clError;
	};
	class RJXML_API CRJXMLParserBase {
	public:
		CRJXMLParserBase(CRJXMLParserObserver&);
		virtual~CRJXMLParserBase();
	protected:
		virtual bool XMLParserGetByte(uint8_t&)=0;
		virtual size_t XMLParserGetStorageHead()=0;
		virtual bool XMLParserReadDataBlock(size_t, size_t)=0;
		bool XMLParserParse();
		bool XMLParserGetDefinedByte(uint8_t&);
		bool XMLParserGetUTF8Symbol(bool, uint8_t&);
		bool XMLParserIsSymbolEQValue(uint8_t,const uint8_t*, size_t);
		bool XMLParserIsSpaceSymbol(uint8_t);
		bool XMLParserParseProlog();
		bool XMLParserParseTagBody(const CRJStringByte&);
		bool XMLParserParseComment();
		bool XMLParserParseInstruction();
		bool XMLParserParseDTD(uint8_t);
		bool XMLParserParseCData();
		bool XMLParserParseTag(uint8_t);
		bool XMLParserTagClose(const CRJStringByte&);
		bool XMLParserParseTagAttr(const CRJStringByte&, CRJStringByteVar&, uint8_t&);
		bool XMLParserOnTagPrefixClose(const CRJStringByte&,bool);
		void XMLParserNextChar();
		void XMLParserNextLine();
		bool XMLParserOnError(CRJXMLError::eUXMLError);
	protected:
		static CRJStringByte m_clKeyWordCDATA;
		static CRJStringByte m_clKeyWordDOCTYPE;
		static CRJAllocatorRuntime m_clHeap;
		CRJXMLParserObserver& m_clObserver;
		CRJStringByteExpanded m_clXMLDataAttrValue, m_clXMLDataWork;
		bool m_bXMLParserNeedMoveFile;
		int64_t m_lXMLParserLine;
		int64_t m_lXMLParserPosChars;
		int64_t m_lXMLParserAllocatedDataSize;
	};
}

