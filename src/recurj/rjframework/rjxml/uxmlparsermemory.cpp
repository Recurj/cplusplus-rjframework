#include "stdafx.h"
#include <recurj/rjframework/libs/xml/xmlparsermemory.h>
using namespace rjXml;
CRJXMLParserMemory::CRJXMLParserMemory(CRJXMLParserObserver& clObserver, const CRJStringByte& s) :
	CRJXMLParserBase(clObserver),
	m_clXMLData(s) {
	m_lXMLParserMemoryHead = 0;
}
CRJXMLParserMemory::~CRJXMLParserMemory() {}
bool CRJXMLParserMemory::XMLParserGetByte(uint8_t& b) {
	if (m_lXMLParserMemoryHead == m_clXMLData.GetLength()) return false;
	b = (uint8_t)m_clXMLData[m_lXMLParserMemoryHead];
	m_lXMLParserMemoryHead++;
	return true;
}
bool CRJXMLParserMemory::XMLParserReadDataBlock(size_t lFrom, size_t lTo) {
	size_t v = lTo - lFrom;
	if (!m_clXMLDataWork.HasNeededSize(v)) return m_clObserver.m_clError.NoMemory();
	m_clXMLDataWork.SetData(m_clXMLData.GetHeadForRead() + lFrom, v);
	return true;
}
bool CRJXMLParserMemory::XMLParserDo() {
	m_lXMLParserMemoryHead = 0;
	return XMLParserParse();
}

