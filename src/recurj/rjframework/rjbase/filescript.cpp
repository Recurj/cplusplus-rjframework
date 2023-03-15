#include "stdafx.h"
#include <recurj/rjframework/libs/base/file/filescript.h>
#include <recurj/rjframework/libs/base/file/filereader.h>
using namespace rjBase;
CRJFileScript::CRJFileScript():
	m_clResult(DataResutRegionScript),
m_sLine(0) {}
CRJFileScript::~CRJFileScript() {}
bool CRJFileScript::RunText(CRJFileName& fnSrc, CRJFileName& fnDst, CRJStringByteVar& s) {
	return run(fnSrc, fnDst, s,& CRJFileScript::processText);
}
bool CRJFileScript::closeScript() {
	return Flush()&&
		Close();
}
bool CRJFileScript::run(CRJFileName& fnSrc, CRJFileName& fnDst, CRJStringByteVar& s, processFunc func) {
	try {
		CRJFileReader fr;
		if (!fr.OpenEncoding(fnSrc)) return setError(sErrorScriptOpen);
		else if (!fr.LoadFile(s)) return setError(sErrorScriptLoad);
		else if (!Create(fnDst)) return setError(sErrorScriptCreate);
		else if (!SetEncoding(CRJFileEncode::eRJFileEncoding::UTF8)) return setError(sErrorScriptSave);
		else {
			CRJStringByteFlex h, f(s);
			f.Trim(' ');
			m_sLine=0;
			while (f.WasFoundTopAndMovedHead(0x0A, h)) {
				m_sLine++;
				if (!(this->* func)(h)) return false;
			}
			if ((this->* func)(f)) {
				if (closeScript()) return true;
				return setError(sErrorScriptClose);
			}
		}
	}
	catch(...) {}
	Close();
	return false;
}
bool CRJFileScript::setError(size_t err) {
	m_clResult.Result(err);
	return false;
}
bool CRJFileScript::processText(CRJStringByteFlex& f) {
	if (f.TrimRight(0x0D).Trim(' ').GetLength()>0) {
		if (!substitute(f)) return false;
		return Writeln(m_clSubstition)?
			true:setError(sErrorScriptSave);
	};
	return true;
}
bool CRJFileScript::substitute(const CRJStringByte& s) {
	bool bUpdate=false;
	CRJStringByteFlex h, v, f(s);
	m_clWork.SetEmpty();
	while (f.WasFoundTopAndMovedHead('%', h)) {
		bUpdate=true;
		m_clWork.Cat(h);
		if (f.IsFirstThat('%')) {
			m_clWork.CatSingle('%');
			f.TruncLeft(1);
		}
		else {
			if (!f.WasFoundTopAndMovedHead('%', v)) return setError(sErrorScriptVarNotDefined);
			if (!getVariable(v)) return setError(sErrorScriptVarInvalid);
		}
	}
	if (bUpdate) {
		m_clWork.Cat(f);
		if (m_clWork.HasVarError()) return setError(sErrorScriptSubstitionTooLong);
		m_clSubstition.Set(m_clWork);
	}
	else m_clSubstition.Set(s);
	return true;
}

