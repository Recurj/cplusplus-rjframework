#include "stdafx.h"
#include <recurj/rjframework/libs/base/file/filewriter.h>
#include <recurj/rjframework/libs/base/file/filewriterpossaver.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <recurj/rjframework/libs/base/file/filereader.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
using namespace rjBase;
CRJFileWriter::CRJFileWriter() {
	m_bOK=false;
}
CRJFileWriter::CRJFileWriter(CRJFileBase& clSrc) {
	Swap(clSrc);
	m_bOK=clSrc.IsOpened();
}
CRJFileWriter::~CRJFileWriter() {}
CRJFileWriter& CRJFileWriter::Backup(CRJFileName& fn, CRJFileEncode::eRJFileEncoding e) {
	CRJFileName fnBak(fn);
	CRJFileEncode encode(e);
	fnBak.CatLabel(".bak");
	m_bOK=fnBak.IsFileExist()?
		fnBak.RemoveSingle():
		true;
	m_bOK=m_bOK&&
		fn.RenameMe(fnBak)&&
		Create(fn)&&
		encode.Save(*this);
	return *this;
}
CRJFileWriter& CRJFileWriter::Write(const uint8_t* buf, size_t lLen) {
	try {
		if (m_hFile>=0&&
			m_bOK) {
#ifdef _rjOS_WIN
			m_bOK=_write(m_hFile, buf,(unsigned int) lLen)==lLen;
#else 
			m_bOK=write(m_hFile, buf, lLen)==lLen;
#endif 
		}
	}
	catch(...) {}
	return *this;
}
CRJFileWriter& CRJFileWriter::Write(const CRJStringByte& s) {
	return Write(s.GetHeadForRead(), s.GetLength());
}
CRJFileWriter& CRJFileWriter::Writeln(const CRJStringByte& s) {
	return Write(s).Writeln();
}
CRJFileWriter& CRJFileWriter::Writelf(const CRJStringByte& s) {
	return Write(s).Writelf();
}
CRJFileWriter& CRJFileWriter::Write(const CRJStringUnicode& u) {
	CRJStringByteExpanded s;
	m_bOK=m_bOK&&
		CRJEncoding::translateUnicodeToUtf8(u, s).OK();
	return Write(s);
}
CRJFileWriter& CRJFileWriter::Writeln(const CRJStringUnicode& u) {
	CRJStringByteExpanded s;
	m_bOK=m_bOK&&
		CRJEncoding::translateUnicodeToUtf8(u, s).OK();
	return Writeln(s);
}
CRJFileWriter& CRJFileWriter::Writelf(const CRJStringUnicode& u) {
	CRJStringByteExpanded s;
	m_bOK=m_bOK&&
		CRJEncoding::translateUnicodeToUtf8(u, s).OK();
	return Writelf(s);
}
CRJFileWriter& CRJFileWriter::Writeln() {
	return Write(CRJStringByte::s_LineNext);
}
CRJFileWriter& CRJFileWriter::Writelf() {
	return Write(CRJStringByte::s_LineFeed);
}
CRJFileWriter& CRJFileWriter::Update(CRJFileName& clFileName,int flags,int mode) {
	m_bOK=false;
	if (clFileName.MakeCLabel().OK()) {
#ifdef _rjOS_WIN
		m_bOK=_wsopen_s(& m_hFile, clFileName.GetHeadForRead(), flags, _SH_DENYWR, mode)==0;
#else 
		m_hFile=open(clFileName.GetHeadForRead(), flags| O_CLOEXEC, mode);
		m_bOK=m_hFile>=0;
#endif 
	}
	return *this;
}
CRJFileWriter& CRJFileWriter::Append(CRJFileName& clFileName,int flags,int mode) {
	m_bOK=false;
	if (clFileName.MakeCLabel().OK()) {
#ifdef _rjOS_WIN
		m_bOK=_wsopen_s(& m_hFile, clFileName.GetHeadForRead(), flags| O_APPEND| O_CREAT, _SH_DENYWR, mode)==0;
#else 
		m_hFile=open(clFileName.GetHeadForRead(), flags| O_APPEND| O_CREAT| O_CLOEXEC, mode);
		m_bOK=m_hFile>=0;
#endif 
	}
	return *this;
}
CRJFileWriter& CRJFileWriter::Create(CRJFileName& clFileName,int flags,int mode) {
	m_bOK=false;
	if (clFileName.MakeCLabel().OK()) {
#ifdef _rjOS_WIN
		m_bOK=_wsopen_s(& m_hFile, clFileName.GetHeadForRead(), flags| _O_CREAT| _O_TRUNC, _SH_DENYWR, mode)==0;
#else 
		m_hFile=open(clFileName.GetHeadForRead(), flags| O_CREAT| O_TRUNC| O_CLOEXEC, mode);
		m_bOK=m_hFile>=0;
#endif 
	}
	return *this;
}
CRJFileWriter& CRJFileWriter::SetEncoding(CRJFileEncode::eRJFileEncoding e) {
	CRJFileEncode enc(e);
	m_bOK=m_bOK&&enc.Save(*this);
	return *this;
}
CRJFileWriter& CRJFileWriter::WriteLabel(const char* p) {
	CRJStringByte s(p);
	return Write(s);
}
CRJFileWriter& CRJFileWriter::WriteChar(char ch) {
	return Write((uint8_t*)& ch, 1);
}

