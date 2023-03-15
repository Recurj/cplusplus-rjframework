#include "stdafx.h"
#include <recurj/rjframework/libs/xml/xmlparserfile.h>
#include <recurj/rjframework/libs/base/file/fileencode.h>
using namespace rjXml;
CRJXMLParserFile::CRJXMLParserFile(CRJXMLParserObserver& clObserver, size_t v):
	CRJXMLParserBase(clObserver) {
	m_lXMLDataHead=0;
	m_lXMLDataSize=0;
	m_lXMLReadFrom=m_lXMLReadTo=0;
	m_lXMLReadHead=0;
	m_lXMLReadSize=0;
	m_lXMLReadBlock=v;
	if (m_lXMLReadBlock<XML_PARSER_FILE_BLOCK_MIN) m_lXMLReadBlock=XML_PARSER_FILE_BLOCK_MIN;
}
CRJXMLParserFile::~CRJXMLParserFile() {}
bool CRJXMLParserFile::XMLParserDo(CRJFileName& clFileName) {
	auto &encode = m_clFile.GetEncode();
	m_lXMLDataHead = m_lXMLReadFrom = m_lXMLReadTo = m_lXMLReadHead = m_lXMLReadSize = 0;
	if (!m_clFile.OpenEncoding(clFileName)) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NO_FILE_OPEN);
	m_lXMLDataSize = m_clFile.GetSize();
	if (encode.GetEncoding() != CRJFileEncode::eRJFileEncoding::UTF8) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NO_FILE_MOVE);
	if (!m_clXMLFileData.HasNeededSize(m_lXMLReadBlock)) return m_clObserver.m_clError.NoMemory();
	return XMLParserParse();
}
bool CRJXMLParserFile::XMLParserGetByte(uint8_t& b) {
	if (m_lXMLReadHead==m_lXMLReadSize) {
		if (m_lXMLDataSize<=m_lXMLReadTo|| !XMLParserReadNextBlock()) return false;
	}
	b=m_clXMLFileData[m_lXMLReadHead];
	m_lXMLReadHead++;
	m_lXMLDataHead++;
	return true;
}
bool CRJXMLParserFile::XMLParserSetFilePos(size_t lPos) {
	return m_clFile.SetPos((filepos_t) (lPos+ m_clFile.GetEncode().HeaderSize()));
}
bool CRJXMLParserFile::XMLParserReadNextBlock() {
	try {
		size_t v=m_lXMLDataSize-m_lXMLReadTo, s=m_clXMLFileData.GetSize();
		m_lXMLReadSize=(v>s)? s:v;
		if (m_bXMLParserNeedMoveFile) {
			if (!XMLParserSetFilePos(m_lXMLReadTo)) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NO_FILE_MOVE);
			m_bXMLParserNeedMoveFile=false;
		}
		if (m_clFile.Read(m_clXMLFileData.GetHeadForUpdate(), m_lXMLReadSize)) {
			m_clXMLFileData.SetLength(m_lXMLReadSize);
			m_lXMLReadHead=0;
			m_lXMLReadFrom=m_lXMLReadTo;
			m_lXMLReadTo+=m_lXMLReadSize;
			return true;
		}
		else return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NO_FILE_READ);
	}
	catch(...) {
		m_clObserver.m_clError.OnException();
	}
	return false;
}
bool CRJXMLParserFile::XMLParserReadDataBlock(size_t lFrom, size_t lTo) {
	try {
		size_t v=lTo-lFrom;
		if (!m_clXMLDataWork.HasNeededSize(v)) return m_clObserver.m_clError.NoMemory();
		if ((PosInReadBuffer(lFrom))&&(PosInReadBuffer(lTo))) {
			m_clXMLDataWork.SetData(m_clXMLFileData.GetHeadForRead()+lFrom-m_lXMLReadFrom, v);
			return true;
		}
		m_bXMLParserNeedMoveFile=true;
		if (!XMLParserSetFilePos(lFrom)) return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NO_FILE_MOVE);
		if (m_clFile.Read(m_clXMLDataWork.GetHeadForUpdate(), v)) {
			m_clXMLDataWork.SetLength(v);
			return true;
		}
		else return XMLParserOnError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NO_FILE_READ);
	}
	catch(...) {
		m_clObserver.m_clError.OnException();
	}
	return false;
}
bool CRJXMLParserFile::PosInReadBuffer(size_t v) {
	return v>=m_lXMLReadFrom&& v<m_lXMLReadTo;
}

