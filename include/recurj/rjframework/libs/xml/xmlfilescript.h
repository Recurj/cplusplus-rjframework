#pragma once
#include <recurj/rjframework/libs/xml/xmlfilestore.h>
#include <recurj/rjframework/libs/base/file/filescript.h>
namespace rjXml {
	class RJXML_API CRJXMLFileScript:public CRJFileScript {
	public:
		CRJXMLFileScript();
		virtual~CRJXMLFileScript();
		bool RunXML(CRJFileName&, CRJFileName&, CRJStringByteVar&,bool r=true);
	protected:
		bool closeScript() override;
		bool processXML(CRJStringByteFlex&);
		bool processInstruction(CRJStringByteFlex&);
		bool processDocType(CRJStringByteFlex&);
		bool processCData(CRJStringByteFlex&);
		bool processComment(CRJStringByteFlex&);
		bool processEntry(CRJStringByteFlex&);
		bool processLeave(CRJStringByteFlex&);
		bool processAttribute(CRJStringByteFlex&);
		bool processContent(CRJStringByteFlex&);
	protected:
		CRJXMLFileStore m_clStore;
		bool m_bWasClosed;
	};
}

