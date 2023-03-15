#include "stdafx.h"
#include <recurj/rjframework/libs/xml/xmlparserbase.h>
using namespace rjXml;
bool CRJXMLParserBase::XMLParserIsSpaceSymbol(uint8_t b) {
	switch (b) {
	case 0x09:
	case 0x20:XMLParserNextChar();
		break;
	case 0x0A:XMLParserNextLine();
	case 0x0D:break;
	default:
		XMLParserNextChar();
		return false;
	}
	return true;
}
bool CRJXMLParserBase::XMLParserGetDefinedByte(uint8_t& b) {
	if (!XMLParserGetByte(b)) return false;
	if (XMLParserIsSpaceSymbol(b)) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
	if (b<0x20) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
	return true;
}
bool CRJXMLParserBase::XMLParserGetUTF8Symbol(bool bWithSpace, uint8_t& b) {
	b=0;
	if (!XMLParserGetByte(b)) return false;
	switch (b) {
	case 0x09:
	case 0x20:XMLParserNextChar();
		if (!bWithSpace) b=0x20;
		break;
	case 0x0A:XMLParserNextLine();
	case 0x0D:b=0x00;
		break;
	default:
		if (b<0x20) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
		XMLParserNextChar();
	}
	return true;
}
bool CRJXMLParserBase::XMLParserIsSymbolEQValue(uint8_t bCheck,const uint8_t* pArr, size_t s) {
	return CRJStringByte::FindTopThat(pArr, s, bCheck)!=nullptr;
}
void CRJXMLParserBase::XMLParserNextChar() {
	m_lXMLParserPosChars++;
}
void CRJXMLParserBase::XMLParserNextLine() {
	m_lXMLParserPosChars=0;
	m_lXMLParserLine++;
}

