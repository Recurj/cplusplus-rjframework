#pragma once
#include <recurj/rjframework/libs/xml/xmlparserbase.h>
namespace rjXml {
	class RJXML_API CRJXMLParserMemory:public CRJXMLParserBase {
	public:
		CRJXMLParserMemory(CRJXMLParserObserver&,const CRJStringByte&);
		virtual~CRJXMLParserMemory();
		bool XMLParserDo();
	protected:
		virtual bool XMLParserGetByte(uint8_t&);
		virtual size_t XMLParserGetStorageHead() {return m_lXMLParserMemoryHead;}
		virtual bool XMLParserReadDataBlock(size_t, size_t);
	private:
		const CRJStringByte& m_clXMLData;
		size_t m_lXMLParserMemoryHead;
	};
}

