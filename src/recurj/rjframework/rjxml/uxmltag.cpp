#include "stdafx.h"
#include <recurj/rjframework/libs/xml/xmltag.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/xml/xmltagattribute.h>
#include <recurj/rjframework/libs/xml/xmltagcontent.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionxml.h>
#include <recurj/rjframework/libs/xml/xmltools.h>
using namespace rjXml;
CRJStringByte CRJXMLTag::s_tagErrorHeader("\tTag <");
CRJStringByte CRJXMLTag::s_tagErrorDetails("Details of last XML's error\t");
RJ_ALLOCATOR(CRJXMLTag, Allocator::Region::XML, AllocatorXML::Tag);
CRJXMLTag* CRJXMLTag::CreateTag(CRJXMLTag* pParent, const CRJStringByte& clName) {
	return new CRJXMLTag(pParent, clName);
}
CRJXMLTag::CRJXMLTag(CRJXMLTag* pParent,const CRJStringByte& clName):
	CRJXMLElement(eUXMLElementType::uXmlElementTag, FreeListElem<CRJXMLElement, CRJXMLTag>),
m_clName(clName) {
	m_pParent=pParent;
}
CRJXMLTag::CRJXMLTag():
	CRJXMLElement(eUXMLElementType::uXmlElementTag,nullptr)
{
	m_pParent=nullptr;
}
CRJXMLTag::~CRJXMLTag() {
	FreeAttributes();
	FreeElements();
}
bool CRJXMLTag::HasContext() const {
	for (auto& elem:m_clElements) {
		if (elem.IsTag()||
			elem.IsContent()) return true;
	}
	return false;
}
bool CRJXMLTag::ForEachElement(IXMLEnumerateElements& clFunc) const {
	for (auto& elem:m_clElements) {
		if (!clFunc(elem)) return false;
	}
	return true;
}
bool CRJXMLTag::ForEachTag(IXMLEnumerateTags& clFunc) const {
	for (auto& elem:m_clElements) {
		if (elem.IsTag()&&
			!clFunc(elem)) return false;
	}
	return true;
}
size_t CRJXMLTag::HowManyTags() const {
	size_t c=0;
	for (auto& elem:m_clElements) {
		if (elem.IsTag()) c++;
	}
	return c;
}
bool CRJXMLTag::ForEachContent(IXMLEnumerateContents& clFunc) const {
	for (auto& elem:m_clElements) {
		if (elem.IsContent()&&
			!clFunc(elem)) return false;
	}
	return true;
}
bool CRJXMLTag::ForEachAttribute(IXMLEnumerateAttributes& clFunc) const {
	for (auto& elem:m_clAttributes) {
		if (!clFunc(elem)) return false;
	}
	return true;
}
void CRJXMLTag::AddElement(CRJXMLElement& clElem) {
	m_clElements.Bot(clElem);
}
CRJXMLTag* CRJXMLTag::AddTag(const CRJStringByte& clName) {
	CRJXMLTag* pTag=nullptr;
	try {
		pTag=CRJXMLTag::CreateTag(this, clName);
		if (pTag) {
			AddElement(* pTag);
			return pTag;
		}
	}
	catch(...) {}
	return nullptr;
}
bool CRJXMLTag::UpdateContent(const CRJStringByte& value) {
	CRJXMLElement* q,* p=m_clElements.GetTop();
	while (p!=nullptr) {
		q=p->GetNext();
		if (p->IsContent()) m_clElements.Remove(* p);
		p=q;
	}
	return AddContent(value);
}
CRJStringByteVar& CRJXMLTag::GetContent(CRJStringByteVar& s) const {
	s.SetEmpty();
	for (auto& elem:m_clElements) {
		if (elem.IsContent()) s.Cat(((CRJXMLTagContent&) elem).GetValue());
	}
	return s;
}
CRJStringByteVar& CRJXMLTag::GetContentByte(CRJStringByteVar& s,bool bErr) const {
	if (CRJXMLTools::Xml2Text(GetContent(s).DeleteChar(0x0D).DeleteChar(0x0A).Trim(' ')).DeleteHexSymbols().OK()) return s;
	if (bErr) CRJXMLTools::ErrorTagContent(*this);
	return s.SetStringVarError(CRJChain::eUChainErrors::BadFormat);
}
CRJStringUnicodeVar& CRJXMLTag::GetContentUnicode(CRJStringUnicodeVar& s,bool bErr) const {
	CRJStringByteExpanded sb;
	if (GetContentByte(sb, bErr).OK()) return s.Set(sb);
	return s.SetStringVarError(CRJChain::eUChainErrors::BadFormat);
}
CRJFileName& CRJXMLTag::GetContentFileName(CRJFileName& fn,bool bPath,bool bErr) const {
	if (GetContentUnicode(fn, bErr).OK()) {
		if (bPath) fn.BuildPath().LowerCase();
		return fn;
	}
	return (CRJFileName&) fn.SetStringVarError(CRJChain::eUChainErrors::BadFormat);
}
bool CRJXMLTag::AddContent(const CRJStringByte& clValue) {
	CRJXMLTagContent* pElem=nullptr;
	try {
		pElem=CRJXMLTagContent::CreateTagContent(clValue);
		if (pElem) {
			AddElement(* pElem);
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJXMLTag::AddAttribute(const CRJStringByte& clName,const CRJStringByte& clValue) {
	try {
		if (clName.GetLength()>0) {
			CRJXMLTagAttribute* pAttr=CRJXMLTagAttribute::CreateAttribute(clName, clValue);
			if (pAttr) {
				m_clAttributes.Bot(* pAttr);
				return true;
			}
		}
		else return true;
	}
	catch(...) {}
	return false;
}
void CRJXMLTag::FreeAttributes() {
	m_clAttributes.Clear();
}
void CRJXMLTag::FreeElements() {
	m_clElements.Clear();
}
bool CRJXMLTag::FindAttribute(const CRJStringByte& clName, IXMLEnumerateAttributes& clFunc) const {
	for (auto& elem:m_clAttributes) {
		if (elem.IsThatName(clName)&&
			clFunc(elem)) return true;
	}
	return false;
}
bool CRJXMLTag::GetAttributeValue(const CRJStringByte& clName, CRJStringByteFlex& clReturn) const {
	auto call=[& clReturn](CRJXMLTagAttribute& clAttr)->bool {
		clAttr.GetReturn(clReturn);
		return true;
	};
	CXMLEnumerateAttributes<decltype(call)>cb(call);
	return FindAttribute(clName, cb);
}
bool CRJXMLTag::GetAttributeValueNum(const CRJStringByte& clName, int64_t& lReturn,bool& rc) const {
	auto call=[& lReturn,& rc](CRJXMLTagAttribute& clAttr)->bool {
		rc=clAttr.GetReturnValidInt64(lReturn);
		return true;
	};
	CXMLEnumerateAttributes<decltype(call)>cb(call);
	rc=false;
	return FindAttribute(clName, cb);
}
bool CRJXMLTag::GetAttributeValueSize(const CRJStringByte& clName, size_t& sReturn,bool& rc) const {
	auto call=[& sReturn,& rc](CRJXMLTagAttribute& clAttr)->bool {
		rc=clAttr.GetReturnValidSize(sReturn);
		return true;
	};
	CXMLEnumerateAttributes<decltype(call)>cb(call);
	rc=false;
	return FindAttribute(clName, cb);
}
bool CRJXMLTag::GetAttributeValueDouble(const CRJStringByte& clName,double& dReturn,bool& rc) const {
	auto call=[& dReturn,& rc](CRJXMLTagAttribute& clAttr)->bool {
		rc=clAttr.GetReturnDouble(dReturn);
		return true;
	};
	CXMLEnumerateAttributes<decltype(call)>cb(call);
	rc=false;
	return FindAttribute(clName, cb);
}
bool CRJXMLTag::GetAttributeValueBool(const CRJStringByte& clName,bool& bReturn,bool& rc) const {
	auto call=[& bReturn,& rc](CRJXMLTagAttribute& clAttr)->bool {
		rc=clAttr.GetReturnBool(bReturn);
		return true;
	};
	CXMLEnumerateAttributes<decltype(call)>cb(call);
	rc=false;
	return FindAttribute(clName, cb);
}
void CRJXMLTag::LogOutAttribute(CRJLogger& clLogger,const CRJStringByte& header,const CRJStringByte& msg) {
	CRJStringByteLong s;
	auto call=[& s,& clLogger](CRJXMLTagAttribute& clAttr)->bool {
		clLogger.Text(s.Set(CRJXMLTag::s_tagErrorDetails).Cat(clAttr.GetName()), clAttr.GetValue());
		return true;
	};
	CXMLEnumerateAttributes<decltype(call)>cb(call);
	clLogger.Text(s.Set(header).Cat(CRJXMLTag::s_tagErrorHeader).Cat(GetName()).CatSingle('>'), msg);
	this->ForEachAttribute(cb);
}
bool CRJXMLTag::LogOutAttributes(const CRJStringByte& header,const CRJStringByte& msg) {
	auto call=[& header,& msg,this](CRJLogger& clLogger)->void {
		LogOutAttribute(clLogger, header, msg);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Do(cb);
	return false;
}
CRJStringByteFlex& CRJXMLTag::GetAttributeValueValid(const CRJStringByte& clName, CRJStringByteFlex& clReturn,const CRJStringByte& clDefault) {
	if (!GetAttributeValue(clName, clReturn)) clReturn.Set(clDefault);
	return clReturn;
}
CRJXMLTagAttribute* CRJXMLTag::GetAttribute(const CRJStringByte& name) {
	for (auto& elem:m_clAttributes)
	if (elem.IsThatName(name)) return & elem;
	return nullptr;
}
CRJXMLTag* CRJXMLTag::GetFirstTag() const {
	for (auto& elem:m_clElements) {
		if (elem.IsTag()) return (CRJXMLTag*)& elem;
	}
	return nullptr;
}
CRJXMLTag* CRJXMLTag::GetNextTag(CRJXMLElement& clTag) const {
	CRJXMLElement* p=clTag.GetNext();
	while (p!=nullptr) {
		if (p->IsTag()) return (CRJXMLTag*) p;
		p=p->GetNext();
	}
	return nullptr;
}
CRJXMLTagAttribute* CRJXMLTag::GetNextAttribute(CRJXMLTagAttribute& clAttr) const {
	return clAttr.GetNext();
}

