#pragma once
#include <recurj/rjframework/libs/base/collections/lists/defs.h>
#include <recurj/rjframework/libs/xml/xmldefs.h>
namespace rjXml {
	class CRJXMLElement;
	class CRJXMLTag;
	class CRJXMLTagContent;
	class RJXML_API CRJXMLElement:public CRJListItem<CRJXMLElement> {
	public:
		using funcFree=void(*)(CRJXMLElement*);
		enum eUXMLElementType {
			uXmlElementTag, uXmlElementContent
		};
		CRJXMLElement(eUXMLElementType, funcFree);
		virtual~CRJXMLElement();
		inline bool IsTag() const {
			return eUXMLElementType::uXmlElementTag==m_clType;
		}
		inline bool IsContent() const {
			return eUXMLElementType::uXmlElementContent==m_clType;
		}
		inline funcFree GetFreeFunc() const {
			return m_clFuncFree;
		}
		operator CRJXMLTag&();
		operator CRJXMLTagContent&();
	protected:
		eUXMLElementType m_clType;
		funcFree m_clFuncFree;
	};
	class CRJXMLListElements : public CRJList<CRJXMLElement> {
	public:

	protected:
		void freeElem(CRJXMLElement*) override;
	};
}

