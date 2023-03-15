#include "stdafx.h"
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/base/datatools/bytepacker.h>
#include <recurj/rjframework/libs/base/datetime/dtimebinary.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/base/logger/loggerapp.h>
#include <recurj/rjframework/libs/base/logger/loggerthread.h>
using namespace rjBase;
const CRJStringByte CRJLogger::s_Separator1(":", 1);
const CRJStringByte CRJLogger::s_Separator2("]:", 2);
const CRJStringByte CRJLogger::s_NoMemory("Not enough memory");
const CRJStringByte CRJLogger::s_NoMessageRoute("Could put message to task");
const CRJStringByte CRJLogger::s_logger("Logger");
CRJLoggerWriterNull CRJLogger::s_WriterDefault;
CRJNamedEmpty CRJLogger::s_NamedEmpty;
CRJStringByteVar& CRJNamedEmpty::GetName(CRJStringByteVar& s) {
	return s.SetEmpty();
}
const size_t cULoggerBinaryFormatLen=16;
SRJLoggerLevels::SRJLoggerLevels(size_t d) {
	for (size_t i=0; i<cLoggerPartMax; i++) m_sLevels[i]=d;
}
SRJLoggerLevels::SRJLoggerLevels(const SRJLoggerLevels& st) {
	for (size_t i=0; i<cLoggerPartMax; i++) m_sLevels[i]=st.m_sLevels[i];
}
CRJLogger::CRJLogger()noexcept:
m_clNamed(CRJLogger::s_NamedEmpty),
m_pWriter(& CRJLogger::s_WriterDefault) {}
CRJLogger::CRJLogger(IRJNamed& n):
	m_clNamed(n),
m_pWriter(& CRJLogger::s_WriterDefault) {}
CRJLogger::~CRJLogger() {}
size_t CRJLogger::GetLevel(size_t p) const {
	return m_stLevels.GetLevel(p);
}
bool CRJLogger::Check(size_t p, size_t d) const {
	return m_pWriter->Check(d)&&
		m_stLevels.Check(p, d);
}
bool CRJLogger::Message(const CRJStringUnicode& clMsg) {
	CRJStringByteExpanded clOut;
	return GetWriter().LogPublic(Header().Cat(CRJEncoding::translateUnicodeToUtf8(clMsg, clOut)));
}
bool CRJLogger::Message(const CRJStringByte& clMsg) {
	return GetWriter().LogPublic(Header().Cat(clMsg));
}
bool CRJLogger::Int64(const CRJStringUnicode& clMsg, int64_t lValue) {
	CRJStringByteExpanded clOut;
	return Int64(CRJEncoding::translateUnicodeToUtf8(clMsg, clOut), lValue);
}
bool CRJLogger::Int64(const CRJStringByte& s, int64_t v) {
	return GetWriter().LogPublic(Header().Cat(s).Cat(CRJLogger::s_Separator1).CatInt64(v));
}
bool CRJLogger::Size(const CRJStringUnicode& clMsg, size_t lValue) {
	CRJStringByteExpanded clOut;
	return Int64(CRJEncoding::translateUnicodeToUtf8(clMsg, clOut), lValue);
}
bool CRJLogger::Size(const CRJStringByte& s, size_t lValue) {
	return GetWriter().LogPublic(Header().Cat(s).Cat(CRJLogger::s_Separator1).CatSize(lValue));
}
bool CRJLogger::Text(const CRJStringUnicode& clMsg,const CRJStringUnicode& clText) {
	CRJStringByteExpanded clOutMsg, clOutText;
	return GetWriter().LogPublic(Header().Cat(CRJEncoding::translateUnicodeToUtf8(clMsg, clOutMsg)).Cat(CRJLogger::s_Separator1).Cat(CRJEncoding::translateUnicodeToUtf8(clText, clOutText)));
}
bool CRJLogger::Text(const CRJStringUnicode& clMsg,const CRJStringByte& v) {
	CRJStringByteExpanded clOutMsg;
	return GetWriter().LogPublic(Header().Cat(CRJEncoding::translateUnicodeToUtf8(clMsg, clOutMsg)).Cat(CRJLogger::s_Separator1).Cat(v));
}
bool CRJLogger::Text(const CRJStringByte& s,const CRJStringByte& v) {
	return GetWriter().LogPublic(Header().Cat(s).Cat(CRJLogger::s_Separator1).Cat(v));
}
bool CRJLogger::Text(const CRJStringByte& s,const CRJStringUnicode& clText) {
	CRJStringByteExpanded clOutText;
	return GetWriter().LogPublic(Header().Cat(s).Cat(CRJLogger::s_Separator1).Cat(CRJEncoding::translateUnicodeToUtf8(clText, clOutText)));
}
bool CRJLogger::Label(const CRJStringByte& s,const char* pText) {
	CRJStringByte v(pText);
	return Text(s, v);
}
bool CRJLogger::Dump(const CRJStringUnicode& clMsg,const uint8_t* d, size_t l) {
	CRJStringByteExpanded clOut;
	return GetWriter().LogPublic(Header().Cat(CRJEncoding::translateUnicodeToUtf8(clMsg, clOut)))&&
		DumpBlock(d, l);
}
bool CRJLogger::Dump(const CRJStringByte& clMsg,const uint8_t* d, size_t l) {
	return GetWriter().LogPublic(Header().Cat(clMsg))&&
		DumpBlock(d, l);
}
CRJLogger& CRJLogger::Header(time_t t) {
	CRJStringByteDefault s;
	m_clTimeSeq.Tick(t);
	return (CRJLogger&) SetDTime(t).CatSingle('[').CatSize(m_clTimeSeq.m_stSequence.sSeq).Cat(CRJLogger::s_Separator2).Cat(m_clNamed.GetName(s)).Cat(CRJLogger::s_Separator1);
}
CRJLogger& CRJLogger::Header() {
	return Header(CRJDTimeTools::GetDTime());
}
bool CRJLogger::DumpBlock(const uint8_t* pData, size_t l) {
	try {
		CRJStringByteDefault str;
		size_t i, s=0;
		size_t lf=(l>>4);
		size_t r=(l& 0xF);
		for (i=1; i<=lf; i++) {
			if (!GetWriter().LogPublic(DumpLine(str, pData, cULoggerBinaryFormatLen, s))) return false;
			pData+=cULoggerBinaryFormatLen;
			s+=cULoggerBinaryFormatLen;
		}
		if (r>0) return GetWriter().LogPublic(DumpLine(str, pData, r, s));
	}
	catch(...) {}
	return true;
}
CRJStringByte& CRJLogger::DumpLine(CRJStringByteDefault& str,const uint8_t* pSrc, size_t c, size_t s) {
	try {
		uint8_t hex;
		uint8_t tmp[32];
		size_t i, l, f;
		CRJStringByteDefault clLine;
		uint8_t* pBuf=clLine.GetHeadForUpdate();
		str.SetInt64PaddingLeft((int64_t) s, 8).CatLabel("    ", 4);
		_MEMMOVE_BYTE_POINTER(tmp, pSrc, c);
		l=0;
		for (i=0; i<c; i++) {
			hex=tmp[i];
			CRJBytePacker::ByteToHex(pBuf, hex,true,' ');
			pBuf+=3;
			l+=3;
			if (hex<0x20) tmp[i]='.';
		}
		f=3*(cULoggerBinaryFormatLen-c);
		if (f>0) {
			_MEMSET_BYTE_POINTER(pBuf,' ', f);
			pBuf+=f;
			l+=f;
		}
		*((int64_t*) pBuf)=*((int64_t*) tmp);
		*((int64_t*)& pBuf[8])=*((int64_t*)& tmp[8]);
		str.Cat(clLine.SetLength(l+c));
	}
	catch(...) {}
	return str;
}
CRJLogger& CRJLogger::FormatOpen(const CRJStringUnicode& s) {
	CRJStringByteExpanded clOut;
	return (CRJLogger&) Header().Cat(CRJEncoding::translateUnicodeToUtf8(s, clOut));
}
CRJLogger& CRJLogger::FormatOpen(const CRJStringByte& s) {
	return (CRJLogger&) Header().Cat(s);
}
CRJLogger& CRJLogger::FormatOpen(const char* pMsg) {
	CRJStringByte clMsg(pMsg);
	return CRJLogger::FormatOpen(clMsg);
}
bool CRJLogger::FormatClose() {
	return GetWriter().LogPublic(*this);
}
bool CRJLogger::Format(_Printf_format_string_ const char* pFormat,...) {
	bool rc=false;
	FUNC_FORMAT(FormatArgs);
	return rc;
}
bool CRJLogger::FormatArgs(const char* pFormat, va_list vArgs) {
	try {
		CRJStringByteExpanded str;
		if (!pFormat) str.SetLabel("Format nullptr!");
		else {
			int lLen=_vscprintf(pFormat, vArgs);
			if (lLen>0) {
				lLen++;
				if (str.HasNeededSize((size_t) lLen+1)) {
					lLen=vsprintf_s((char*) str.GetHeadForUpdate(), lLen, pFormat, vArgs);
					if (lLen>0) str.SetLength(lLen);
				}
			}
		}
		return GetWriter().LogPublic(Header().Cat(str));
	}
	catch(...) {}
	return false;
}

