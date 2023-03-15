#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections//lists/defs.h>
#include <recurj/rjframework/libs/xml/xmltagcontent.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionxml.h>
using namespace rjXml;
RJ_ALLOCATOR(CRJXMLTagContent, Allocator::Region::XML, AllocatorXML::Content);
CRJXMLTagContent* CRJXMLTagContent::CreateTagContent(const CRJStringByte& clValue) {
	return new CRJXMLTagContent(clValue);
}
CRJXMLTagContent::CRJXMLTagContent(const CRJStringByte& clValue) :
	CRJXMLElement(eUXMLElementType::uXmlElementContent, FreeListElem<CRJXMLElement, CRJXMLTagContent>),
	m_clValue(clValue) {}
CRJXMLTagContent::~CRJXMLTagContent() {}

