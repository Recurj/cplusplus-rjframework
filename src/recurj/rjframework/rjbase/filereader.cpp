#include "stdafx.h"
#include <recurj/rjframework/libs/base/file/filereader.h>
#include <recurj/rjframework/libs/base/file/filereaderpossaver.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
using namespace rjBase;
bool CRJFileReader::Load(CRJFileName& fn, CRJStringByteVar& data) {
	CRJFileReader fh;
	return fh.OpenEncoding(fn)&&
		fh.LoadFile(data);
}
CRJFileReader::CRJFileReader():
	m_sSize(0) {}
CRJFileReader::~CRJFileReader() {}
bool CRJFileReader::OpenEncoding(CRJFileName& clFileName,int flags) {
	m_sSize=0;
	if (clFileName.MakeCLabel().OK()) {
#ifdef _rjOS_WIN
		if (_wsopen_s(& m_hFile, clFileName.GetHeadForRead(),(flags&(~_O_CREAT)), _SH_DENYWR, 0)==0) {
			return m_clEncode.Load(*this, m_sSize);
		}
#else 
		m_hFile=open(clFileName.GetHeadForRead(),(flags&(~O_CREAT))| O_CLOEXEC);
		return m_hFile>=0&&	m_clEncode.Load(*this, m_sSize);
#endif 
	}
	return false;
}
bool CRJFileReader::OpenBinary(CRJFileName& clFileName,int flags) {
	m_sSize=0;
	if (clFileName.MakeCLabel().OK()) {
		filepos_t s=0;
#ifdef _rjOS_WIN
		if (_wsopen_s(& m_hFile, clFileName.GetHeadForRead(),(flags&(~_O_CREAT)), _SH_DENYWR, 0)==0) {
			if (GetLength(s)) {
				m_clEncode.SetEncoding(CRJFileEncode::eRJFileEncoding::BINARY);
				m_sSize=s;
				return true;
			}
		}
#else 
		m_hFile=open(clFileName.GetHeadForRead(),(flags&(~O_CREAT))| O_CLOEXEC);
		if (m_hFile>=0) {
			if (GetLength(s)) {
				m_clEncode.SetEncoding(CRJFileEncode::eRJFileEncoding::BINARY);
				m_sSize=s;
				return true;
			}
		}
#endif 
	}
	return false;
}
bool CRJFileReader::Read(void* buf, size_t lLen) {
	try {
		if (m_hFile>=0) {
#ifdef _rjOS_WIN
			return _read(m_hFile, buf,(unsigned int) lLen)==lLen;
#else 
			return read(m_hFile, buf, lLen)==lLen;
#endif 
		}
	}
	catch(...) {}
	return false;
}
bool CRJFileReader::Read(CRJStringByteVar& s, size_t lLen) {
	if (s.HasNeededSize(lLen)&&
		Read(s.GetHeadForUpdate(), lLen)) {
		s.SetLengthSafed(lLen);
		return true;
	}
	return false;
}
bool CRJFileReader::ReadAvailable(void* buf, size_t& lLen) {
	try {
		if (m_hFile>=0) {
#ifdef _rjOS_WIN
			auto l=_read(m_hFile, buf,(unsigned int) lLen);
#else 
			auto l=read(m_hFile, buf, lLen);
#endif 
			if (l>0) {
				lLen=(size_t) l;
				return true;
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJFileReader::LoadFile(CRJStringByteVar& s) {
	try {
		if (m_hFile>=0) {
			if (s.HasNeededSize(m_sSize)) {
#ifdef _rjOS_WIN
				auto l=_read(m_hFile, s.GetHeadForUpdate(),(unsigned int) m_sSize);
#else 
				auto l=read(m_hFile, s.GetHeadForUpdate(), lLen);
#endif 
				if (l==(int) m_sSize) {
					s.SetLengthSafed(m_sSize);
					return true;
				}
			}
		}
	}
	catch(...) {}
	return false;
}
CRJFileCRC32::CRJFileCRC32(CRJFileName& fn):
	m_eResult(Result::OK) {
	filepos_t s;
	if (m_clFile.OpenBinary(fn)&&
		m_clFile.GetLength(s)) {
		m_sSize=s;
	}
	else {
		m_sSize=0;
		m_eResult=Result::NOFILE;
	}
}
bool CRJFileCRC32::GetBlock(CRJStringByteFlex& st) {
	if (m_sSize>0) {
		size_t v=m_clData.GetSize();
		size_t s=m_sSize>v?
			v:m_sSize;
		m_sSize-=s;
		if (m_clFile.Read(m_clData.GetHeadForUpdate(), s)) {
			m_clData.SetLengthSafed(s);
			st=m_clData;
			return true;
		}
		else m_eResult=Result::NOREAD;
	}
	return false;
}
bool CRJFileCRC32::OK() {
	return m_eResult==Result::OK;
}
CRJFileCRC32::Result CRJFileCRC32::GetResult() {
	return m_eResult;
}

