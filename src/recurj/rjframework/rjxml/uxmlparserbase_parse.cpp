#include "stdafx.h"
#include <recurj/rjframework/libs/xml/xmlparserbase.h>
using namespace rjXml;
CRJStringByte CRJXMLParserBase::m_clKeyWordCDATA("CDATA[");
CRJStringByte CRJXMLParserBase::m_clKeyWordDOCTYPE("DOCTYPE");
#pragma warning(push)
#pragma warning(disable : 4702)
 bool CRJXMLParserBase::XMLParserParseProlog() {
	uint8_t b=0;
	do {
		if (!XMLParserGetByte(b)) return false;
	}
	while (XMLParserIsSpaceSymbol(b));
	if (b!=XML_PARSER_SYMBOL_TAG_OPEN) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
	if (!XMLParserGetDefinedByte(b)) return false;
	if (b==XML_PARSER_SYMBOL_PROCESS) return XMLParserParseInstruction();
	else if (b==XML_PARSER_SYMBOL_EX) {
		if (!XMLParserGetDefinedByte(b)) return false;
		if (b==XML_PARSER_SYMBOL_COMMENT) return XMLParserParseComment();
		return XMLParserParseDTD(b);
	}
	if (b==XML_PARSER_SYMBOL_TAG_OPEN||
		b==XML_PARSER_SYMBOL_TAG_CLOSE) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
	return XMLParserParseTag(b);
}
#pragma warning (pop)
 bool CRJXMLParserBase::XMLParserParseTagBody(const CRJStringByte& clTag) {
	uint8_t b;
	size_t lFrom=XMLParserGetStorageHead();
	while (XMLParserGetUTF8Symbol(true, b)) {
		if (b==XML_PARSER_SYMBOL_TAG_OPEN) {
			if (!XMLParserReadDataBlock(lFrom, XMLParserGetStorageHead()-1)) return false;
			else if (!m_clObserver.OnTagContent(clTag, m_clXMLDataWork)) return false;
			else if (XMLParserGetDefinedByte(b)) {
				switch (b) {
				case XML_PARSER_SYMBOL_TAG_CLOSE_PREFIX: return XMLParserTagClose(clTag);
				case XML_PARSER_SYMBOL_EX:
					if (!XMLParserGetDefinedByte(b)) return false;
					switch (b) {
					case XML_PARSER_SYMBOL_COMMENT:
						if (!XMLParserParseComment()) return false;
						break;
					case XML_PARSER_SYMBOL_CDATA_START:
						if (!XMLParserParseCData()) return false;
						break;
					default:
						return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
					}
					break;
				default:
					if (b==XML_PARSER_SYMBOL_TAG_OPEN||
						b==XML_PARSER_SYMBOL_TAG_CLOSE) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
					else if (!XMLParserParseTag(b)) return false;
				}
			}
			lFrom=XMLParserGetStorageHead();
		}
		else if (b==XML_PARSER_SYMBOL_TAG_CLOSE) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
	}
	return false;
}
bool CRJXMLParserBase::XMLParserParseComment() {
	uint8_t b;
	size_t lFrom=XMLParserGetStorageHead()+1;
	if (!XMLParserGetDefinedByte(b)) return false;
	if (b!=XML_PARSER_SYMBOL_COMMENT) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
	while (XMLParserGetUTF8Symbol(true, b)) {
		if (b==XML_PARSER_SYMBOL_COMMENT) {
			if (!XMLParserGetUTF8Symbol(true, b)) return false;
			if (b==XML_PARSER_SYMBOL_COMMENT) {
				if (!XMLParserGetUTF8Symbol(true, b)) return false;
				if (b==XML_PARSER_SYMBOL_TAG_CLOSE) return (XMLParserReadDataBlock(lFrom, XMLParserGetStorageHead()-3))?
					m_clObserver.OnComment(m_clXMLDataWork):
					false;
			}
		}
	}
	return false;
}
bool CRJXMLParserBase::XMLParserParseInstruction() {
	uint8_t b;
	size_t lFrom=XMLParserGetStorageHead();
	while (XMLParserGetUTF8Symbol(true, b)) {
		if (b==XML_PARSER_SYMBOL_PROCESS) {
			if (!XMLParserGetUTF8Symbol(true, b)) return false;
			if (b==XML_PARSER_SYMBOL_TAG_CLOSE) return (XMLParserReadDataBlock(lFrom, XMLParserGetStorageHead()-2))?
				m_clObserver.OnInstruction(m_clXMLDataWork):
				false;
		}
	}
	return false;
}
bool CRJXMLParserBase::XMLParserParseDTD(uint8_t b) {
	CRJStringByteShort tag;
	int64_t line=m_lXMLParserLine;
	int64_t pos=m_lXMLParserPosChars;
	tag.SetSingle(b);
	while (XMLParserGetUTF8Symbol(true, b)) {
		if (b==XML_PARSER_SYMBOL_SPACE)break;
		if (tag.CatSingle(b).GetLength()>m_clKeyWordDOCTYPE.GetLength()) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
	}
	if (tag!=m_clKeyWordDOCTYPE) {
		m_lXMLParserLine=line;
		m_lXMLParserPosChars=pos;
		return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
	}
	size_t lFrom=XMLParserGetStorageHead();
	while (XMLParserGetUTF8Symbol(true, b)) {
		if (b==XML_PARSER_SYMBOL_TAG_CLOSE) return (XMLParserReadDataBlock(lFrom, XMLParserGetStorageHead()-1))?
			m_clObserver.OnDoctype(m_clXMLDataWork):
			false;
	}
	return false;
}
bool CRJXMLParserBase::XMLParserParseCData() {
	uint8_t b;
	size_t i, c=CRJXMLParserBase::m_clKeyWordCDATA.GetLength(),
	lFrom=XMLParserGetStorageHead()+c;
	const uint8_t* p=(const uint8_t*) CRJXMLParserBase::m_clKeyWordCDATA.GetHeadForRead();
	for (i=0; i<c; i++) {
		if (!XMLParserGetDefinedByte(b)) return false;
		if (b!=* p) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
		p++;
	}
	while (XMLParserGetUTF8Symbol(true, b)) {
		if (b==XML_PARSER_SYMBOL_CDATA_END) {
			if (!XMLParserGetUTF8Symbol(true, b)) return false;
			if (b==XML_PARSER_SYMBOL_CDATA_END) {
				if (!XMLParserGetUTF8Symbol(true, b)) return false;
				if (b==XML_PARSER_SYMBOL_TAG_CLOSE) return (XMLParserReadDataBlock(lFrom, XMLParserGetStorageHead()-3))?
					m_clObserver.OnCDATA(m_clXMLDataWork):
					false;
			}
		}
	}
	return false;
}
bool CRJXMLParserBase::XMLParserParseTag(uint8_t bSym) {
	CRJStringByteDefault clTag, clAttr;
	uint8_t bLastInAttr;
	bool bNoSpace=true;
	clTag.SetSingle(bSym);
	while (XMLParserGetUTF8Symbol(false, bSym)) {
		switch (bSym) {
		case XML_PARSER_SYMBOL_SPACE:
			if (bNoSpace) {
				bNoSpace=false;
				if (!clTag.GetLength()) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
				else if (!m_clObserver.OnTagEntry(clTag)) return false;
			}
			break;
		case XML_PARSER_SYMBOL_NEW_LINE:
			if (bNoSpace) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
			break;
		case XML_PARSER_SYMBOL_TAG_CLOSE_PREFIX: return XMLParserOnTagPrefixClose(clTag, bNoSpace);
		case XML_PARSER_SYMBOL_TAG_CLOSE:
			if (!clTag.GetLength()) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
			return ((!bNoSpace)||
				m_clObserver.OnTagEntry(clTag))&&
				m_clObserver.OnTagContinue(clTag)&&
				XMLParserParseTagBody(clTag);
			break;
		case XML_PARSER_SYMBOL_TAG_OPEN: return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
		default:
			if (bNoSpace) clTag.CatSingle(bSym);
			else {
				clAttr.SetSingle(bSym);
				bLastInAttr=0;
				if (!XMLParserParseTagAttr(clTag, clAttr, bLastInAttr)) return false;
				if (bLastInAttr!=0) {
					if (bLastInAttr==XML_PARSER_SYMBOL_TAG_CLOSE) return m_clObserver.OnTagContinue(clTag)&&
						XMLParserParseTagBody(clTag);
					else if (bLastInAttr==XML_PARSER_SYMBOL_TAG_CLOSE_PREFIX) return XMLParserOnTagPrefixClose(clTag, bNoSpace);
					return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
				}
			}
			break;
		}
	}
	return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NOT_CLOSED_TAG0);
}
bool CRJXMLParserBase::XMLParserOnTagPrefixClose(const CRJStringByte& clTag,bool bNoSpace) {
	uint8_t bSym;
	if (!clTag.GetLength()) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
	if (!XMLParserGetUTF8Symbol(false, bSym)) return false;
	if (bSym!=XML_PARSER_SYMBOL_TAG_CLOSE) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
	return ((!bNoSpace)||
		m_clObserver.OnTagEntry(clTag))&&
		m_clObserver.OnTagCloseSelf(clTag);
}
bool CRJXMLParserBase::XMLParserTagClose(const CRJStringByte& clWasTag) {
	uint8_t b;
	CRJStringByteDefault clCloseTag;
	bool bNoSpace=true;
	while (XMLParserGetUTF8Symbol(false, b)) {
		switch (b) {
		case XML_PARSER_SYMBOL_SPACE:
			if (bNoSpace) {
				bNoSpace=false;
				if (!clCloseTag.GetLength()) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
			}
			break;
		case XML_PARSER_SYMBOL_NEW_LINE:
			if (bNoSpace) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
			break;
		case XML_PARSER_SYMBOL_TAG_CLOSE_PREFIX: return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
		case XML_PARSER_SYMBOL_TAG_CLOSE:
			if (!clCloseTag.GetLength()) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
			else if (clCloseTag==clWasTag) return m_clObserver.OnTagClose(clWasTag);
			else return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NOT_SAME_CLOSE_TAG);
		case XML_PARSER_SYMBOL_TAG_OPEN: return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
		default:
			if (bNoSpace) clCloseTag.CatSingle(b);
			else return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NOT_SAME_CLOSE_TAG);
		}
	}
	return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NOT_CLOSED_TAG1);
}
bool CRJXMLParserBase::XMLParserParseTagAttr(const CRJStringByte& clTag, CRJStringByteVar& clAttr, uint8_t& bLast) {
	uint8_t b;
	bool bNoSpace=true, bInValue=false;
	uint8_t bWait=XML_PARSER_SYMBOL_ATTR_SEPARATOR;
	m_clXMLDataAttrValue.SetEmpty();
	while (XMLParserGetUTF8Symbol(false, b)) {
		switch (b) {
		case XML_PARSER_SYMBOL_SPACE:
			if (bInValue) m_clXMLDataAttrValue.CatSingle(b);
			else bNoSpace=false;
			break;
		case XML_PARSER_SYMBOL_NEW_LINE:
			if (bWait==XML_PARSER_SYMBOL_ATTR_SEPARATOR) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
			break;
		default:
			if (b==bWait) {
				switch (bWait) {
				case XML_PARSER_SYMBOL_ATTR_SEPARATOR:bWait=XML_PARSER_SYMBOL_VALUE_SEPARATOR;
					break;
				case XML_PARSER_SYMBOL_VALUE_SEPARATOR:
					if (bInValue) return m_clObserver.OnTagAttribute(clTag, clAttr, m_clXMLDataAttrValue);
					else bInValue=true;
					break;
				}
			}
			else if (b==XML_PARSER_SYMBOL_TAG_OPEN) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
			else if (b==XML_PARSER_SYMBOL_TAG_CLOSE||
				b==XML_PARSER_SYMBOL_TAG_CLOSE_PREFIX) {
				if (bInValue) m_clXMLDataAttrValue.CatSingle(b);
				else 
				switch (bWait) {
				case XML_PARSER_SYMBOL_ATTR_SEPARATOR:
					if (m_clObserver.AllowAttributeWithoutData()) {
						bLast=b;
						return m_clObserver.OnTagAttributeWithoutData(clTag, clAttr);
					}
					else XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
					break;
				default:
					return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
				}
			}
			else {
				switch (bWait) {
				case XML_PARSER_SYMBOL_ATTR_SEPARATOR:
					if (bNoSpace) clAttr.CatSingle(b);
					else if (m_clObserver.AllowAttributeWithoutData()) {
						if (!m_clObserver.OnTagAttributeWithoutData(clTag, clAttr)) return false;
						clAttr.SetSingle(b);
						bNoSpace=true;
					}
					else XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
					break;
				case XML_PARSER_SYMBOL_VALUE_SEPARATOR:
					if (bInValue) m_clXMLDataAttrValue.CatSingle(b);
					else return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_BAD_SYMBOL);
					break;
				}
			}
			break;
		}
	}
	return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NOT_CLOSED_ATTR);
}

