#include "stdafx.h"
#include <recurj/rjframework/libs/xml/xmlfilestore.h>
#include <recurj/rjframework/libs/xml/xmlparserbase.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <recurj/rjframework/libs/base/file/fileencode.h>
#include <recurj/rjframework/libs/xml/xmltools.h>
using namespace rjXml;
const uint8_t CRJXMLFileStore::s_Tabs[XML_FILE_STORE_TAB_ARRAY] = {
	0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
	0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
	0x09, 0x09, 0x09, 0x09
};
const char CRJXMLFileStore::s_TagChars[4] = {
	'<','/','>', 0x00
};
const char CRJXMLFileStore::s_AttrChars[4] = {
	' ','=','\"', 0x00
};
CRJXMLFileStore::CRJXMLFileStore(CRJFileWriter& fw, bool r, size_t lBuf) :
	m_clFile(fw),
	m_bReadable(r),
	b_bTagOpened(false) {
	if (lBuf < 1024) lBuf = 1024;
	m_clBuffer.HasNeededSize(lBuf);
	m_lTabs = 0;
	m_bOK = true;
}
CRJXMLFileStore::~CRJXMLFileStore(void) {}
bool CRJXMLFileStore::Save(const CRJStringByte& s) {
	return Save(s.GetHeadForRead(), s.GetLength());
}
bool CRJXMLFileStore::Save(const uint8_t* pBuf, size_t lLen) {
	if (!m_bOK) return false;
	try {
		size_t s = m_clBuffer.GetSize(),
			v = s - m_clBuffer.GetLength();
		while (lLen > v) {
			m_clBuffer.CatData(pBuf, v);
			if (!FlushBuf()) return false;
			pBuf += v;
			lLen -= v;
			v = s;
		}
		m_clBuffer.CatData(pBuf, lLen);
	}
	catch (...) {}
	return true;
}
bool CRJXMLFileStore::Tabs(size_t sCount) {
	if (!m_bOK) return false;
	size_t c;
	Save(CRJStringByte::s_LineNext);
	while (sCount > 0) {
		c = (sCount > XML_FILE_STORE_TAB_ARRAY) ?
			XML_FILE_STORE_TAB_ARRAY : sCount;
		sCount -= c;
		if (!Save(CRJXMLFileStore::s_Tabs, c)) return false;
	}
	return true;
}
bool CRJXMLFileStore::FlushBuf() {
	if (m_clFile.Write(m_clBuffer)) {
		m_clFile.Flush();
		m_clBuffer.SetEmpty();
		return true;
	}
	return false;
}
CRJXMLFileStore& CRJXMLFileStore::Open() {
	CRJFileEncode clEncode;
	m_lTabs = 0;
	m_bOK = true;
	return SetResult(Save(CRJXMLTools::s_Header));
}
CRJXMLFileStore& CRJXMLFileStore::Open(const CRJStringByte& clDeclaration) {
	if (clDeclaration.GetLength() > 0) {
		m_lTabs = 0;
		m_bOK = true;
		return Desciption(clDeclaration);
	}
	return Open();
}
CRJXMLFileStore& CRJXMLFileStore::Close() {
	FlushBuf();
	return SetResult(m_clFile.Close());
}
CRJXMLFileStore& CRJXMLFileStore::TagOpen(const CRJStringByte& str) {
	if (b_bTagOpened) CRJXMLFileStore::TagContinue();
	b_bTagOpened = true;
	return SetResult((!m_bReadable ||
		Tabs(m_lTabs++)) &&
		Save((uint8_t*)s_TagChars, 1) &&
		Save(str));
}
CRJXMLFileStore& CRJXMLFileStore::TagClose(const CRJStringByte& str, bool bEmb) {
	int64_t wt = --m_lTabs;
	b_bTagOpened = false;
	return SetResult((!m_bReadable ||
		!bEmb ||
		Tabs(wt)) &&
		Save((uint8_t*)s_TagChars, 2) &&
		Save(str) &&
		Save((uint8_t*)&s_TagChars[2], 1));
}
CRJXMLFileStore& CRJXMLFileStore::TagClose() {
	--m_lTabs;
	b_bTagOpened = false;
	return SetResult(Save((uint8_t*)&s_TagChars[1], 2));
}
CRJXMLFileStore& CRJXMLFileStore::TagContinue() {
	b_bTagOpened = false;
	return SetResult(Save((uint8_t*)&s_TagChars[2], 1));
}
CRJXMLFileStore& CRJXMLFileStore::Attribute(const CRJStringByte& strName) {
	return SetResult(Save((uint8_t*)s_AttrChars, 1) &&
		Save(strName));
}
CRJXMLFileStore& CRJXMLFileStore::Attribute(const CRJStringByte& strName, const CRJStringByte& strValue) {
	if (Save((uint8_t*)s_AttrChars, 1) &&
		Save(strName) &&
		Save((uint8_t*)&s_AttrChars[1], 1) &&
		Save((uint8_t*)&s_AttrChars[2], 1) &&
		Save(strValue) &&
		Save((uint8_t*)&s_AttrChars[2], 1)) {
		return SetResult(true);
	}
	return SetResult(false);
}
CRJXMLFileStore& CRJXMLFileStore::AttributeInt(const CRJStringByte& strName, int64_t lVal) {
	CRJStringByteDefault str;
	return Attribute(strName, str.SetInt64(lVal));
}
CRJXMLFileStore& CRJXMLFileStore::AttributeSize(const CRJStringByte& strName, size_t lVal) {
	CRJStringByteDefault str;
	return Attribute(strName, str.SetSize(lVal));
}
CRJXMLFileStore& CRJXMLFileStore::AttributeBool(const CRJStringByte& strName, bool b) {
	CRJStringByteDefault str;
	return Attribute(strName, str.SetBool(b));
}
CRJXMLFileStore& CRJXMLFileStore::Content(const CRJStringByte& str) {
	if (b_bTagOpened) CRJXMLFileStore::TagContinue();
	return SetResult(Save(str));
}
CRJXMLFileStore& CRJXMLFileStore::ContentInt(int64_t lValue) {
	CRJStringByteDefault str;
	return Content(str.SetInt64(lValue));
}
CRJXMLFileStore& CRJXMLFileStore::ContentBool(bool b) {
	CRJStringByteDefault str;
	return Content(str.SetBool(b));
}
CRJXMLFileStore& CRJXMLFileStore::External(const CRJStringByte& clData) {
	return SetResult(Save(clData));
}
CRJXMLFileStore& CRJXMLFileStore::Desciption(const CRJStringByte& s) {
	return SetResult(Save((uint8_t*)"<?", 2) &&
		Save(s) &&
		Save((uint8_t*)"?>", 2));
}
CRJXMLFileStore& CRJXMLFileStore::DocType(const CRJStringByte& s) {
	return SetResult(Save(CRJXMLTools::s_DocType) &&
		Save(s) &&
		Save((uint8_t*)">", 1));
}
CRJXMLFileStore& CRJXMLFileStore::CData(const CRJStringByte& s) {
	return SetResult(Save(CRJXMLTools::s_CData) &&
		Save(s) &&
		Save((uint8_t*)"]]>", 3));
}
CRJXMLFileStore& CRJXMLFileStore::Comments(const CRJStringByte& s) {
	return SetResult(Save((uint8_t*)"<!--", 4) &&
		Save(s) &&
		Save((uint8_t*)"-->", 3));
}

