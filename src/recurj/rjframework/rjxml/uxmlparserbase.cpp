#include "stdafx.h"
#include <recurj/rjframework/libs/xml/xmlparserbase.h>
using namespace rjXml;
CRJXMLParserBase::CRJXMLParserBase(CRJXMLParserObserver& clObserver):
	m_clObserver(clObserver) {
	m_lXMLParserLine=1;
	m_lXMLParserPosChars=0;
	m_lXMLParserAllocatedDataSize=0;
	m_bXMLParserNeedMoveFile=false;
	m_clXMLDataAttrValue.HasNeededSize(SIZE_XML_PARSER_ATTR_DEFAULT);
}
CRJXMLParserBase::~CRJXMLParserBase() {}
bool CRJXMLParserBase::XMLParserOnError(CRJXMLError::eUXMLError lErr) {
	return m_clObserver.m_clError.SetXMLError(lErr, m_lXMLParserLine, m_lXMLParserPosChars);
}
bool CRJXMLParserBase::XMLParserParse() {
	m_lXMLParserLine=1;
	m_lXMLParserPosChars=0;
	m_clObserver.m_clError.ResetXMLError();
	m_bXMLParserNeedMoveFile=false;
	while (XMLParserParseProlog());
	return m_clObserver.m_clError.OK();
}

