#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/xml/xmltagattribute.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionxml.h>
using namespace rjXml;
RJ_ALLOCATOR(CRJXMLTagAttribute, Allocator::Region::XML, AllocatorXML::Attribute)
CRJXMLTagAttribute* CRJXMLTagAttribute::CreateAttribute(const CRJStringByte& clName, const CRJStringByte& clValue) {
	return new CRJXMLTagAttribute(clName, clValue);
}
CRJXMLTagAttribute::CRJXMLTagAttribute(const CRJStringByte& clName, const CRJStringByte& clValue) :
	m_clName(clName), m_clValue(clValue) {}
CRJXMLTagAttribute::~CRJXMLTagAttribute() {}
void CRJXMLTagAttribute::GetReturn(CRJStringByteFlex& clReturn) {
	clReturn.Set(m_clValue);
}
bool CRJXMLTagAttribute::GetReturnNum(int64_t& lReturn) {
	return m_clValue.IsValidInt64(lReturn);
}
bool CRJXMLTagAttribute::GetReturnSize(size_t& lReturn) {
	return m_clValue.IsValidSize(lReturn);
}
bool CRJXMLTagAttribute::GetReturnDouble(double& dReturn) {
	return m_clValue.IsValidDouble(dReturn);
}
bool CRJXMLTagAttribute::GetReturnBool(bool& bReturn) {
	return m_clValue.IsValidBool(bReturn);
}
int64_t CRJXMLTagAttribute::GetReturnValidInt64(int64_t lDefault) {
	int64_t p;
	return GetReturnNum(p) ?
		p : lDefault;
}
int32_t CRJXMLTagAttribute::GetReturnValidInt32(int32_t lDefault) {
	int64_t p;
	return GetReturnNum(p) ?
		(int32_t)p:lDefault;
}
size_t CRJXMLTagAttribute::GetReturnValidSize(size_t lDefault) {
	size_t p;
	return GetReturnSize(p) ?
		p : lDefault;
}
double CRJXMLTagAttribute::GetReturnValidDouble(double dDefault) {
	double d;
	return GetReturnDouble(d) ?
		d : dDefault;
};
bool CRJXMLTagAttribute::GetReturnValidBool(bool bDefault) {
	bool b;
	return GetReturnBool(b) ?
		b : bDefault;
}

void CRJXMLTagAttributes::freeElem(CRJXMLTagAttribute* p)
{
	if (p == nullptr) return;
	try {
		delete p;
	}
	catch (...) {}
}