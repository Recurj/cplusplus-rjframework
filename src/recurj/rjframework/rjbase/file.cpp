#include "stdafx.h"
#include <recurj/rjframework/libs/base/file/file.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/unicode/allocated.h>
#include <recurj/rjframework/libs/base/file/filereaderpossaver.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/base/file/filereader.h>
#include <recurj/rjframework/libs/base/file/filereadertext.h>
using namespace rjBase;
CRJFile::CRJFile() {}
CRJFile::~CRJFile() {}
bool CRJFile::Open(CRJFileName& clFileName,int flags) {
	if (clFileName.MakeCLabel().OK()) {
#ifdef _rjOS_WIN
		return _wsopen_s(& m_hFile, clFileName.GetHeadForRead(),(flags&(~_O_CREAT)), _SH_DENYWR, 0)==0;
#else 
		m_hFile=open(clFileName.GetHeadForRead(),(flags&(~O_CREAT))| O_CLOEXEC);
		return m_hFile>=0;
#endif 
	}
	return false;
}
bool CRJFile::CreateTemp(CRJFileName& clFileName,int flags) {
	if (clFileName.MakeCLabel().OK()) {
#ifdef _rjOS_WIN
		return _wsopen_s(& m_hFile, clFileName.GetHeadForRead(), flags| _O_CREAT| _O_TEMPORARY, _SH_DENYWR, 0)==0;
#else 
		m_hFile=open(clFileName.GetHeadForRead(), flags| O_CREAT| O_CLOEXEC);
		return m_hFile>=0;
#endif 
	}
	return false;
}
size_t CRJFile::GetTextSize() {
	CRJFileReaderShared clTmp(*this);
	return clTmp.GetSize();
}
bool CRJFile::Read(void* buf, size_t lLen) {
	CRJFileReaderShared clTmp(*this);
	return clTmp.Read(buf, lLen);
}
bool CRJFile::ReadAvailable(void* buf, size_t& lLen) {
	CRJFileReaderShared clTmp(*this);
	return clTmp.ReadAvailable(buf, lLen);
}
bool CRJFile::ReadlnFormat(CRJStringByteVar& s,bool bFormat) {
	CRJFileReaderTextShared clTmp(*this);
	return clTmp.ReadlnFormat(s, bFormat);
}

