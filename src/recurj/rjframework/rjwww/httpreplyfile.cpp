#include "stdafx.h"
#include <recurj/rjframework/libs/www/http/replyfile.h>
#include <recurj/rjframework/libs/base/file/fileencode.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
using namespace rjWWW;
CHTTPReplyFile::CHTTPReplyFile() {
	m_sFileLength=0;
}
CHTTPReplyFile::~CHTTPReplyFile() {}
bool CHTTPReplyFile::Ready(CRJFileName& clFileName,bool bHead) {
	CRJStringByteDefault s;
	AddHeader(CHTTP::s_clHeaderLastModified, CRJDTimeTools::DT2StrRFC822(s, clFileName.GetLastUpdated()));
	if (!bHead) {
		if (!m_clFile.OpenBinary(clFileName)) return false;
		m_sFileLength=m_clFile.GetSize();
	}
	return true;
}
bool CHTTPReplyFile::GetBinaryData(CRJByteWriter& clWriter, size_t l) {
	if (l>0) {
		size_t r;
		uint8_t buf[1024];
		do {
			r=(l>sizeof(buf))?
				sizeof(buf):
				l;
			l-=r;
			if ((!m_clFile.Read(buf, r))||
				(!clWriter.CatData(buf, r).OK())) return false;
		}
		while (l>0);
	}
	return true;
}

