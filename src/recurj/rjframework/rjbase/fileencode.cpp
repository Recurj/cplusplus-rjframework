#include "stdafx.h"
#include <recurj/rjframework/libs/base/file/fileencode.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/unicode/allocated.h>
#include <recurj/rjframework/libs/base/file/filereaderpossaver.h>
#include <recurj/rjframework/libs/base/file/filereader.h>
#include <recurj/rjframework/libs/base/file/filewriter.h>
using namespace rjBase;
uint8_t CRJFileEncode::s_markerUTF8[3]= {
	0xEF, 0xBB, 0xBF
};
uint8_t CRJFileEncode::s_markerUTF32_LE[4]= {
	0xFF, 0xFE, 0x00, 0x00
};
uint8_t CRJFileEncode::s_markerUTF32_BE[4]= {
	0x00, 0x00, 0xFE, 0xFF
};
uint8_t CRJFileEncode::s_markerUTF16_LE[2]= {
	0xFF, 0xFE
};
uint8_t CRJFileEncode::s_markerUTF16_BE[2]= {
	0xFE, 0xFF
};
CRJFileEncode::CRJFileEncode() {
	m_clEncoding=eRJFileEncoding::ASCII;
}
CRJFileEncode::CRJFileEncode(eRJFileEncoding e) {
	m_clEncoding=e;
}
CRJFileEncode::~CRJFileEncode() {}
bool CRJFileEncode::Save(CRJFileWriter& clFile) const {
	clFile.SetPos(0);
	switch (m_clEncoding) {
	case eRJFileEncoding::UTF8: return Write(clFile, CRJFileEncode::s_markerUTF8,sizeof(CRJFileEncode::s_markerUTF8));
	case eRJFileEncoding::UTF16_BE: return Write(clFile, CRJFileEncode::s_markerUTF16_BE,sizeof(CRJFileEncode::s_markerUTF16_BE));
	case eRJFileEncoding::UTF16_LE: return Write(clFile, CRJFileEncode::s_markerUTF16_LE,sizeof(CRJFileEncode::s_markerUTF16_LE));
	case eRJFileEncoding::UTF32_BE: return Write(clFile, CRJFileEncode::s_markerUTF32_BE,sizeof(CRJFileEncode::s_markerUTF32_BE));
	case eRJFileEncoding::UTF32_LE: return Write(clFile, CRJFileEncode::s_markerUTF32_LE,sizeof(CRJFileEncode::s_markerUTF32_LE));
	case eRJFileEncoding::ASCII:
	case eRJFileEncoding::UTF7:
	case eRJFileEncoding::UTF1:
	case eRJFileEncoding::UTF_EBCDIC:
	case eRJFileEncoding::SCSU:break;
	}
	return true;
}
bool CRJFileEncode::Load(CRJFileName& fn) {
	CRJFileReader f;
	size_t fp;
	return f.OpenBinary(fn)&& Load(f, fp);
}
size_t CRJFileEncode::HeaderSize() const
{
	switch (m_clEncoding) {
	case eRJFileEncoding::UTF8:return 3;
	case eRJFileEncoding::SCSU:
	case eRJFileEncoding::UTF1:return 3;
	case eRJFileEncoding::UTF7:
	case eRJFileEncoding::UTF_EBCDIC:
	case eRJFileEncoding::UTF32_BE:
	case eRJFileEncoding::UTF32_LE:return 4;
	case eRJFileEncoding::UTF16_BE:
	case eRJFileEncoding::UTF16_LE:return 2;
	}
	return 0;
}

bool CRJFileEncode::Load(CRJFileReader& clFile, size_t& sLen) {
	filepos_t l=0;
	m_clEncoding=eRJFileEncoding::ASCII;
	if (clFile.IsOpened()&&
		clFile.GetLength(l)) {
		filepos_t lHeader=0;
		uint8_t buf[4]= {0x01, 0x01, 0x01, 0x01};
		clFile.SetPos(0);
		m_clEncoding=eRJFileEncoding::ASCII;
		if (Read(clFile, buf,(l>4)?	(streamsize)(4):l)) {
			if ((buf[0]==CRJFileEncode::s_markerUTF8[0])&&
				(buf[1]==CRJFileEncode::s_markerUTF8[1])&&
				(buf[2]==CRJFileEncode::s_markerUTF8[2])) {
				m_clEncoding=eRJFileEncoding::UTF8;
				lHeader=3;
			}
			else if ((buf[0]==0xFF)&&
				(buf[1]==0xFE)) {
				if ((buf[2]==0x00)&&(buf[3]==0x00)) {
					lHeader=4;
					m_clEncoding=eRJFileEncoding::UTF32_LE;
				}
				else {
					lHeader=2;
					m_clEncoding=eRJFileEncoding::UTF16_LE;
				}
			}
			else if ((buf[1]==0xFE)&&
				(buf[0]==0xFF)) {
				lHeader=2;
				m_clEncoding=eRJFileEncoding::UTF16_BE;
			}
			else if ((buf[0]==0x00)&&
				(buf[1]==0x00)&&
				(buf[2]==0xFE)&&
				(buf[3]==0xFF)) {
				lHeader=4;
				m_clEncoding=eRJFileEncoding::UTF32_BE;
			}
			else if ((buf[0]==0x00)&&
				(buf[1]==0x00)&&
				(buf[2]==0xFE)&&
				(buf[3]==0xFF)) {
				lHeader=4;
				m_clEncoding=eRJFileEncoding::UTF32_BE;
			}
			else if (((buf[0]==0x2B)&&
				(buf[1]==0x2F)&&
				(buf[2]==0x76)&&
				(buf[3]==0x38))||
				((buf[0]==0x2B)&&
				(buf[1]==0x2F)&&
				(buf[2]==0x76)&&
				(buf[3]==0x39))||
				((buf[0]==0x2B)&&
				(buf[1]==0x2F)&&
				(buf[2]==0x76)&&
				(buf[3]==0x2B))||
				((buf[0]==0x2B)&&
				(buf[1]==0x2F)&&
				(buf[2]==0x76)&&
				(buf[3]==0x2F))) {
				lHeader=4;
				m_clEncoding=eRJFileEncoding::UTF7;
			}
			else if ((buf[0]==0xF7)&&
				(buf[1]==0x64)&&
				(buf[2]==0x4C)) {
				m_clEncoding=eRJFileEncoding::UTF1;
				lHeader=3;
			}
			else if ((buf[0]==0xDD)&&
				(buf[1]==0x73)&&
				(buf[2]==0x66)&&
				(buf[3]==0x73)) {
				lHeader=4;
				m_clEncoding=eRJFileEncoding::UTF_EBCDIC;
			}
			else if ((buf[0]==0x0E)&&
				(buf[1]==0xFE)&&
				(buf[2]==0xFF)) {
				m_clEncoding=eRJFileEncoding::SCSU;
				lHeader=3;
			}
			clFile.SetPos(lHeader);
			sLen=(size_t) l-lHeader;
			return true;
		}
		else sLen=(size_t) l;
	}
	return false;
}
bool CRJFileEncode::IsUTF8(CRJFileReader& clFile, size_t& lLen) {
	filepos_t l=0;
	if (clFile.IsOpened()&&
		clFile.GetLength(l)) {
		uint8_t buf[4]= {0x01, 0x01, 0x01, 0x01};
		clFile.SetPos(0);
		m_clEncoding=eRJFileEncoding::ASCII;
		if (Read(clFile, buf, 3)&&
			buf[0]==CRJFileEncode::s_markerUTF8[0]&&
			buf[1]==CRJFileEncode::s_markerUTF8[1]&&
			buf[2]==CRJFileEncode::s_markerUTF8[2]) {
			lLen=(size_t) l-3;
			m_clEncoding=eRJFileEncoding::UTF8;
			return true;
		}
	}
	return false;
}
bool CRJFileEncode::Read(CRJFileReader& clFile, uint8_t* buf, size_t lLen) {
	return clFile.Read(buf, lLen);
}
bool CRJFileEncode::Write(CRJFileWriter& clFile,const uint8_t* buf, size_t lLen) {
	return clFile.Write(buf, lLen);
}

