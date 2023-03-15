#include "stdafx.h"
#include <recurj/rjframework/libs/xml/xmlfilescript.h>
#include <recurj/rjframework/libs/base/file/filereader.h>
#include <recurj/rjframework/libs/xml/xmltools.h>
using namespace rjXml;
CRJXMLFileScript::CRJXMLFileScript():
	m_clStore(GetFileWriterMe(),true),
m_bWasClosed(false) {}
CRJXMLFileScript::~CRJXMLFileScript() {}
bool CRJXMLFileScript::RunXML(CRJFileName& fnSrc, CRJFileName& fnDst, CRJStringByteVar& s,bool r) {
	m_clStore.SetReadableMode(r);
	return run(fnSrc, fnDst, s,(processFunc)& CRJXMLFileScript::processXML);
}
bool CRJXMLFileScript::closeScript() {
	return m_clStore.Close()&&
		CRJFileScript::closeScript();
}
bool CRJXMLFileScript::processXML(CRJStringByteFlex& f) {
	if (f.TrimRight(0x0D).Trim(' ').GetLength()>0) {
		CRJStringByteFlex h;
		if (!f.WasFoundTopAndMovedHead(' ', h)) {
			if (f==CRJXMLTools::s_cmdLeave) {
				h.Set(f);
				f.SetEmpty();
			}
			else return setError(sErrorScriptNoCommand);
		}
		h.Trim(' ');
		if (h==CRJXMLTools::s_cmdContent) return processContent(f);
		if (h==CRJXMLTools::s_cmdAttribute) return processAttribute(f);
		if (h==CRJXMLTools::s_cmdEntry) return processEntry(f);
		if (h==CRJXMLTools::s_cmdLeave) return processLeave(f);
		if (h==CRJXMLTools::s_cmdIgnore) return true;
		if (h==CRJXMLTools::s_cmdInstruction) return processInstruction(f);
		if (h==CRJXMLTools::s_cmdDoctype) return processDocType(f);
		if (h==CRJXMLTools::s_cmdCData) return processCData(f);
		if (h==CRJXMLTools::s_cmdComment) return processComment(f);
	};
	return true;
}
bool CRJXMLFileScript::processInstruction(CRJStringByteFlex& f) {
	if (!substitute(f)) return false;
	return m_clStore.Desciption(m_clSubstition)?
		true:setError(sErrorScriptSave);
}
bool CRJXMLFileScript::processDocType(CRJStringByteFlex& f) {
	if (!substitute(f)) return false;
	return m_clStore.DocType(m_clSubstition)?
		true:setError(sErrorScriptSave);
}
bool CRJXMLFileScript::processCData(CRJStringByteFlex& f) {
	if (!substitute(f)) return false;
	return m_clStore.CData(m_clSubstition)?
		true:setError(sErrorScriptSave);
}
bool CRJXMLFileScript::processComment(CRJStringByteFlex& f) {
	if (!substitute(f)) return false;
	return m_clStore.Comments(m_clSubstition)?
		true:setError(sErrorScriptSave);
}
bool CRJXMLFileScript::processEntry(CRJStringByteFlex& f) {
	m_bWasClosed=false;
	return m_clStore.TagOpen(f)?
		true:setError(sErrorScriptSave);
}
bool CRJXMLFileScript::processLeave(CRJStringByteFlex& f) {
	bool b=f.IsEmpty()?
		m_clStore.TagClose():
		m_clStore.TagClose(f, m_bWasClosed);
	m_bWasClosed=true;
	return b?
		true:setError(sErrorScriptSave);
}
bool CRJXMLFileScript::processAttribute(CRJStringByteFlex& s) {
	CRJStringByteFlex a, f(s);
	if (f.WasFoundTopAndMovedHead(' ', a)) {
		if (!substitute(f)) return false;
		return m_clStore.Attribute(a, m_clSubstition)?
			true:setError(sErrorScriptSave);
	}
	return m_clStore.Attribute(s)?
		true:setError(sErrorScriptSave);
}
bool CRJXMLFileScript::processContent(CRJStringByteFlex& f) {
	if (!substitute(f)) return false;
	return m_clStore.Content(m_clSubstition)?
		true:setError(sErrorScriptSave);
}

