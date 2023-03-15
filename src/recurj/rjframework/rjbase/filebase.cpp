#include "stdafx.h"
#include <recurj/rjframework/libs/base/file/filebase.h>
#include <recurj/rjframework/libs/base/file/filereaderpossaver.h>
using namespace rjBase;
CRJFileBase::CRJFileBase() {
	m_hFile=-1;
}
CRJFileBase::~CRJFileBase() {
	Close();
}
void CRJFileBase::Swap(CRJFileBase& clSrc) {
	m_hFile=clSrc.m_hFile;
	clSrc.m_hFile=-1;
}
bool CRJFileBase::Flush() {
	if (m_hFile>=0)
#ifdef _rjOS_WIN
	_commit(m_hFile);
#else 
	fsync(m_hFile);
#endif 
	return true;
}
bool CRJFileBase::Close() {
	if (m_hFile>=0) {
#ifdef _rjOS_WIN
		_close(m_hFile);
#else 
		close(m_hFile);
#endif 
		m_hFile=-1;
	}
	return true;
}
bool CRJFileBase::SetPos(filepos_t k,int mode) {
#ifdef _rjOS_WIN
	return _lseeki64(m_hFile, k, mode)!=-1;
#else 
	return lseek64(m_hFile, k, mode)!=-1;
#endif 
}
bool CRJFileBase::GetPos(filepos_t& k) {
#ifdef _rjOS_WIN
	k=_lseeki64(m_hFile, 0, SEEK_CUR);
#else 
	k=lseek64(m_hFile, 0, SEEK_CUR);
#endif 
	return k!=-1;
}
bool CRJFileBase::GetLength(filepos_t& k) {
	CRJFileSaveReaderPosSaver clPos(*this);
#ifdef _rjOS_WIN
	k=_lseeki64(m_hFile, 0, SEEK_END);
#else 
	k=lseek64(m_hFile, 0, SEEK_END);
#endif 
	return k!=-1;
}

