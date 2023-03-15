#pragma once
#include <recurj/rjframework/libs/base/file/filereader.h>
#include <recurj/rjframework/libs/xml/xmlparserbase.h>
namespace rjXml {
	constexpr size_t XML_PARSER_FILE_BLOCK_MIN=4* 1024;
	class RJXML_API CRJXMLParserFile:public CRJXMLParserBase {
	public:
		CRJXMLParserFile(CRJXMLParserObserver&, size_t v=0);
		virtual~CRJXMLParserFile();
		bool XMLParserDo(CRJFileName&);
	protected:
		virtual bool XMLParserGetByte(uint8_t&);
		virtual size_t XMLParserGetStorageHead() {return m_lXMLDataHead;}
		virtual bool XMLParserReadDataBlock(size_t, size_t);
		bool XMLParserReadNextBlock();
		bool XMLParserSetFilePos(size_t);
		bool PosInReadBuffer(size_t);
	protected:
		CRJFileReader m_clFile;
		CRJStringByteExpanded m_clXMLFileData;
		size_t m_lXMLDataSize, m_lXMLDataHead, m_lXMLReadFrom, m_lXMLReadTo;
		size_t m_lXMLReadHead, m_lXMLReadSize, m_lXMLReadBlock;
	};
}

