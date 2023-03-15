#include "stdafx.h"
#include <recurj/rjframework/libs/xml/xmlcollection.h>
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/base/file/filereader.h>
#include <recurj/rjframework/libs/base/file/fileencode.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/xml/xmltag.h>
#include <recurj/rjframework/libs/xml/xmltagattribute.h>
#include <recurj/rjframework/libs/xml/xmltagcontent.h>
#include <recurj/rjframework/libs/xml/xmlparsermemory.h>
using namespace rjXml;
CRJStringByte CRJXMLCollection::s_loadings("Loading the config data::");
CRJStringByte CRJXMLCollection::s_fileError("\tXML error\tFile\t");
CRJStringByte CRJXMLCollection::s_rootTagAbsent("Root tag is absent");
CRJStringByte CRJXMLCollection::s_rootTagInvalid("Root tag is invalid");
CRJStringByte CRJXMLCollection::s_contentTrunc(" \x0d\x0a\x09");
CRJXMLCollection::CRJXMLCollection(bool bTruncContext):
m_bTruncContext(bTruncContext) {
	m_pTagWork=nullptr;
	m_pTagRoot=nullptr;
}
CRJXMLCollection::~CRJXMLCollection() {
	FreeAll();
}
void CRJXMLCollection::OnRemoveRootTag() {
	try {
		if (m_pTagRoot) {
			auto funcFree=m_pTagRoot->GetFreeFunc();
			if (funcFree) funcFree(m_pTagRoot);
		}
	}
	catch(...) {}
}
CRJXMLTag* CRJXMLCollection::IsRootTagName(const CRJStringByte& str) const {
	try {
		return (m_pTagRoot!=nullptr&&
			m_pTagRoot->IsThatName(str))?
			m_pTagRoot:nullptr;
	}
	catch(...) {}
	return nullptr;
}
void CRJXMLCollection::FreeAll() {
	OnRemoveRootTag();
	m_pTagRoot=nullptr;
}
bool CRJXMLCollection::LoadFromFile(CRJFileName& fn,const CRJStringByte& root, IXMLEnumerateTags& cb) {
	if (LoadFromFile(fn)) {
		CRJXMLTag* pTag=IsRootTagName(root);
		if (pTag) return pTag->ForEachTag(cb);
	}
	return false;
}
bool CRJXMLCollection::LoadFromFile(CRJFileName& clName) {
	CRJFileReader fh;
	try {
		if (fh.OpenEncoding(clName)) {
			CRJFileEncode clEncode;
			size_t l;
			if (clEncode.IsUTF8(fh, l)) {
				CRJStringByteAllocated str;
				if (str.HasNeededSize(l+16)) {
					if (fh.Read((void*) str.GetHeadForUpdate(), l)) {
						CRJXMLParserMemory clData(*this, str);
						str.SetLength(l);
						return clData.XMLParserDo();
					}
					else m_clError.SetError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NO_FILE_READ);
				}
				else m_clError.NoMemory();
			}
			else m_clError.SetError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NO_FILE_MOVE);
		}
		else m_clError.SetError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NO_FILE_OPEN);
	}
	catch(...) {}
	return false;
}
bool CRJXMLCollection::LoadFromData(const CRJStringByte& clSrc) {
	CRJXMLParserMemory clData(*this, clSrc);
	return clData.XMLParserDo();
}
bool CRJXMLCollection::Save2File(CRJXMLFileStore& fh,bool bReadable,const CRJStringByte& clDeclaration) {
	try {
		if (m_pTagRoot) {
			fh.SetReadableMode(bReadable);
			return fh.Open(clDeclaration)&&
				Save2File(fh,* m_pTagRoot)&&
				fh.Close();
		}
	}
	catch(...) {}
	return false;
}
bool CRJXMLCollection::Save2File(CRJXMLFileStore& fh, CRJXMLTag& clTag) {
	try {
		size_t t=0;
		auto callA=[& fh](CRJXMLTagAttribute& clAttr)->bool {
			return fh.Attribute(clAttr.GetName(), clAttr.GetValue());
		};
		auto callE=[this,& fh,& t](CRJXMLElement& clElem)->bool {
			if (clElem.IsTag()) {
				t++;
				return Save2File(fh, clElem);
			}
			return fh.Content(((CRJXMLTagContent&) clElem).GetValue());
		};
		CXMLEnumerateAttributes<decltype(callA)>cbA(callA);
		CXMLEnumerateElements<decltype(callE)>cbE(callE);
		if (fh.TagOpen(clTag.GetName())&&
			clTag.ForEachAttribute(cbA)) {
			if (clTag.HasContext()) {
				if (fh.TagContinue()&&
					clTag.ForEachElement(cbE)&&
					fh.TagClose(clTag.GetName(), t!=0)) return true;
			}
			else if (fh.TagClose()) return true;
		}
		m_clError.SetError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NO_FILE_SAVE);
	}
	catch(...) {}
	return false;
}
bool CRJXMLCollection::OnTagEntry(const CRJStringByte& clTag) {
	try {
		if ((!m_pTagWork)&&
			(m_pTagRoot)) return m_clError.SetError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NOT_SINGLE_ROOT);
		else {
			CRJXMLTag* pTag=CRJXMLTag::CreateTag(m_pTagWork, clTag);
			if (pTag) {
				if (!m_pTagWork) m_pTagRoot=pTag;
				else m_pTagWork->AddElement(* pTag);
				m_pTagWork=pTag;
				return true;
			}
			else m_clError.NoMemory();
		}
	}
	catch(...) {}
	return false;
}
bool CRJXMLCollection::OnTagClose(const CRJStringByte&) {
	try {
		if (m_pTagWork) {
			m_pTagWork=m_pTagWork->GetParent();
			return true;
		}
	}
	catch(...) {}
	return m_clError.SetError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NO_ROOT_TAG);
}
bool CRJXMLCollection::OnTagContent(const CRJStringByte&, CRJStringByteVar& clContent) {
	try {
		if (m_pTagWork) {
			if (m_bTruncContext) {
				CRJStringByteFlex f(clContent);
				while (f.IsNonEmpty()) {
					if (!f.IsFirstThat(s_contentTrunc))break;
					f.TruncRight(1);
				}
				while (f.IsNonEmpty()) {
					if (!f.IsLastThat(s_contentTrunc))break;
					f.TruncRight(1);
				}
				return f.GetLength()>0?
					m_pTagWork->AddContent(f):
					true;
			}
			else return clContent.GetLength()>0?
				m_pTagWork->AddContent(clContent):
				true;
		}
	}
	catch(...) {}
	return m_clError.SetError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NO_CONTENT_TAG);
}
bool CRJXMLCollection::OnTagAttribute(const CRJStringByte&,const CRJStringByte& clName, CRJStringByteVar& clValue) {
	try {
		if (m_pTagWork) {
			return m_pTagWork->AddAttribute(clName, clValue);
		}
	}
	catch(...) {}
	return m_clError.SetError(CRJXMLError::eUXMLError::XML_PARSER_ERROR_NO_ATTR_TAG);
}
bool CRJXMLCollection::LoadFromFileAndWalk(const CRJStringByte& clMessage, CRJFileName& fn,const CRJStringByte& szRoot, IXMLEnumerateTags& clFunc) {
	CRJXMLCollection clCollection;
	if (!clCollection.LoadFromFile(fn)) {
		CRJStringByteDefault err;
		return CRJXMLCollection::LoadFromFileError(clMessage, fn, clCollection.GetError(err));
	}
	CRJXMLTag* pTag=clCollection.IsRootTagName(szRoot);
	if (pTag) return pTag->ForEachTag(clFunc);
	return CRJXMLCollection::LoadFromFileError(clMessage, fn, CRJXMLCollection::s_rootTagAbsent);
}
bool CRJXMLCollection::LoadFromFileAndProcessRoot(const CRJStringByte& clMessage, CRJFileName& fn,const CRJStringByte& szRoot, IXMLEnumerateTags& clFunc) {
	CRJXMLCollection clCollection;
	if (!clCollection.LoadFromFile(fn)) {
		CRJStringByteDefault err;
		return CRJXMLCollection::LoadFromFileError(clMessage, fn, clCollection.GetError(err));
	}
	CRJXMLTag* pTag=clCollection.IsRootTagName(szRoot);
	if (pTag) return clFunc(* pTag);
	return CRJXMLCollection::LoadFromFileError(clMessage, fn, CRJXMLCollection::s_rootTagAbsent);
}
bool CRJXMLCollection::LoadFromFileError(const CRJStringByte& clMessage, CRJFileName& fn,const CRJStringByte& clErr) {
	auto call=[& clMessage,& fn,& clErr](CRJLogger& clLogger)->void {
		CRJStringExpanded v;
		clLogger.Message(v.Set(clMessage).Cat(CRJXMLCollection::s_fileError).Cat(fn).CatSingle('\t').Cat(clErr));
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Do(cb);
	return false;
}

