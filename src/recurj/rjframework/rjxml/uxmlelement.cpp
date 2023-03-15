#include "stdafx.h"
#include <recurj/rjframework/libs/xml/xmlelement.h>
using namespace rjXml;
CRJXMLElement::CRJXMLElement(eUXMLElementType clType, funcFree clFuncFree):
m_clType(clType),m_clFuncFree(clFuncFree) {}
CRJXMLElement::~CRJXMLElement() {}
CRJXMLElement::operator CRJXMLTag&() {
	return *((CRJXMLTag*)this);
}
CRJXMLElement::operator CRJXMLTagContent&() {
	return *((CRJXMLTagContent*)this);
}

void CRJXMLListElements::freeElem(CRJXMLElement* p)
{
	if (p == nullptr) return;
	try {
		auto ff = p->GetFreeFunc();
		if (ff != nullptr) ff(p);
	}
	catch (...) {}
}