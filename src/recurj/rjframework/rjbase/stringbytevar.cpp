#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/strings/unicode/string.h>
#include <recurj/rjframework/libs/base/strings/unicode/string.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <charconv>
#include <stdio.h>
using namespace rjBase;
class CRJHexStringParser {
public:
	CRJHexStringParser(CRJStringByteVar& s):
		str(s),
	sLen(s.GetLength()),
	h(s.GetHeadForUpdate()) {}
	CRJStringByteVar& Parse() {
		int64_t lVal;
		while (sLen>0) {
			if (* h=='<') {
				if (sLen>1&&
					h[1]=='<') {
					replace('<', 1);
					continue;
				}
				else if (sLen>3&&
					h[3]=='>') {
					if (CRJDataTools::AreSame(h+1,(uint8_t*)"FS", 2)) {
						replace(0x1C, 3);
						continue;
					}
					if (CRJDataTools::AreSame(h+1,(uint8_t*)"GS", 2)) {
						replace(0x1D, 3);
						continue;
					}
					if (CRJDataTools::AreSame(h+1,(uint8_t*)"CR", 2)) {
						replace(0x0D, 3);
						continue;
					}
					if (CRJDataTools::AreSame(h+1,(uint8_t*)"LF", 2)) {
						replace(0x0A, 3);
						continue;
					}
					if (CRJDataTools::AreSame(h+1,(uint8_t*)"ES", 2)) {
						replace(0x1E, 3);
						continue;
					}
					if (CRJDataTools::AreSame(h+1,(uint8_t*)"BR", 2)) {
						replace('<', 3);
						continue;
					}
					else {
						lVal=CRJByteConvertor::PairChars2NumSafed(h+1);
						if (lVal>=0) {
							replace((uint8_t) lVal, 3);
							continue;
						}
					}
				}
				else if (sLen>4&&
					h[4]=='>') {
					if (CRJDataTools::AreSame(h+1,(uint8_t*)"ESC", 3)) {
						replace(0x1B, 4);
						continue;
					}
					if (CRJDataTools::AreSame(h+1,(uint8_t*)"TAB", 3)) {
						replace(0x09, 4);
						continue;
					}
				}
			}
			h++;
			sLen--;
		}
		return str;
	}
protected:
	void replace(char ch, size_t c) {
		size_t r=c+1;
		* h=(uint8_t) ch;
		h++;
		if (sLen>r) {
			sLen-=r;
			CRJDataTools::MoveBytes(h, h+c, sLen);
		}
		else sLen=0;
		str.SetLengthSafed(str.GetLength()-c);
	}
	CRJStringByteVar& str;
	size_t sLen;
	uint8_t* h;
};
class CRJHexStringBuilder {
public:
	CRJHexStringBuilder(const uint8_t* p, size_t l):
		sLen(l),
	h(p) {}
	CRJStringByteVar& Build(CRJStringByteVar& dst) {
		dst.SetEmpty();
		uint8_t tmp[8];
		const uint8_t* pText;
		size_t cText;
		do {
			cText=0;
			pText=h;
			while (sLen>0&&
				h[0]>=0x20) {
				h++;
				sLen--;
				cText++;
			}
			if (cText>0) dst.CatData(pText, cText);
			if (sLen==0)break;
			switch (h[0]) {
			case 0x09:dst.CatLabel("<TAB>", 5);
				break;
			case 0x1B:dst.CatLabel("<ESC>", 5);
				break;
			case 0x0D:dst.CatLabel("<CR>", 4);
				break;
			case 0x0A:dst.CatLabel("<LF>", 4);
				break;
			case 0x1C:dst.CatLabel("<FS>", 4);
				break;
			case 0x1D:dst.CatLabel("<GS>", 4);
				break;
			case 0x1E:dst.CatLabel("<ES>", 4);
				break;
			default:
				tmp[0]='<';
				CRJByteConvertor::Byte2PairHex(& tmp[1], h[0]);
				tmp[3]='>';
				dst.CatData(tmp, 4);
				break;
			};
			sLen--;
			h++;
		}
		while (sLen>0);
		return dst;
	}
protected:
	size_t sLen;
	const uint8_t* h;
};
CRJStringByteVar::CRJStringByteVar():
	m_sSize(0) {}
CRJStringByteVar::CRJStringByteVar(size_t s):
	m_sSize(s) {}
CRJStringByteVar::~CRJStringByteVar() {}
bool CRJStringByteVar::HasNeededSize(size_t s) {
	return m_sSize>=s;
}
CRJStringByteVar& CRJStringByteVar::SetStringVarError(eUChainErrors e) {
	return SetVarError(e);
}
uint8_t* CRJStringByteVar::GetHeadForUpdate() const {
	return const_cast<uint8_t*>(GetHeadForRead());
}
char* CRJStringByteVar::GetHeadForUpdateC() const {
	return const_cast<char*>(GetHeadForReadC());
}
CRJStringByteVar& CRJStringByteVar::SetEmpty() {
	m_clResult.ResetError();
	return SetLengthSafed(0);
}
CRJStringByteVar& CRJStringByteVar::SetAt(size_t s, uint8_t v) {
	if (s<m_sLength) GetHeadForUpdate()[s]=v;
	else return SetStringVarError(eUChainErrors::BadPos);
	return *this;
}
CRJStringByteVar& CRJStringByteVar::Set(const CRJStringByte& clStr) {
	return SetDataCheckLength(clStr.GetHeadForRead(), clStr.GetLength());
}
CRJStringByteVar& CRJStringByteVar::Set(const CRJStringUnicode& s) {
	return CRJEncoding::translateUnicodeToUtf8(s,*this);
}
CRJStringByteVar& CRJStringByteVar::Set(const std::string& s) {
	return SetData((const uint8_t*) s.c_str(), s.length());
}
CRJStringByteVar& CRJStringByteVar::SetSingle(uint8_t ch) {
	return SetDataCheckLength(& ch, 1);
}
CRJStringByteVar& CRJStringByteVar::SetDataAsLabel(uint8_t* p, size_t lVol) {
	p[lVol-1]=0x00;
	return SetData(p, strlen((char*) p));
}
CRJStringByteVar& CRJStringByteVar::SetData(const uint8_t* p, size_t l) {
	return (p)?
		SetDataCheckLength(p, l):
		SetStringVarError(eUChainErrors::NoPointer);
}
CRJStringByteVar& CRJStringByteVar::SetLabel(const wchar_t* p) {
	CRJStringUnicode label(p);
	return CRJEncoding::translateUnicodeToUtf8(label,*this);
}
CRJStringByteVar& CRJStringByteVar::SetLabel(const char* p, size_t l) {
	return (p)?
		SetDataCheckLength((const uint8_t*) p, l):
		SetStringVarError(eUChainErrors::NoPointer);
}
CRJStringByteVar& CRJStringByteVar::SetDataCheckLength(const uint8_t* p, size_t l) {
	if (HasNeededSize(l)) {
		if (l<=0) return SetEmpty();
		else return SetDataSafed(p, l);
	}
	return SetStringVarError(eUChainErrors::NoSpace);
}
CRJStringByteVar& CRJStringByteVar::SetDataSafed(const uint8_t* p, size_t l) {
	_MEMMOVE_BYTE_POINTER(GetHeadForUpdate(), p, l);
	return SetLengthSafed(l);
}
CRJStringByteVar& CRJStringByteVar::SetInt64(int64_t lValue) {
	m_sLength=CRJByteConvertor::Int64ToBytes(lValue, GetHeadForUpdate(), m_sSize);
	return *this;
}
CRJStringByteVar& CRJStringByteVar::SetDouble(double d) {
	if (m_sSize>0) {
		char* p=(char*) GetHeadForUpdate();
		auto res=std::to_chars(p, p+m_sSize-1, d);
		if (res.ec==std::errc()) m_sLength=res.ptr-p;
		else SetStringVarError(eUChainErrors::NoSpace);
	}
	return *this;
}
CRJStringByteVar& CRJStringByteVar::SetSize(size_t lValue) {
	m_sLength=CRJByteConvertor::SizeToBytes(lValue, GetHeadForUpdate(), m_sSize);
	return *this;
}
CRJStringByteVar& CRJStringByteVar::SetHex(size_t lValue) {
	m_sLength=CRJByteConvertor::HexToBytes(lValue, GetHeadForUpdate(), m_sSize);
	return *this;
}
CRJStringByteVar& CRJStringByteVar::SetBool(bool b) {
	return Set((b)?
		CRJStringByte::s_WordYES:CRJStringByte::s_WordNO);
}
CRJStringByteVar& CRJStringByteVar::SetFill(size_t c, uint8_t ch) {
	if (c>0&&HasNeededSize(c)) {
		uint8_t* p=GetHeadForUpdate();
		_MEMSET_BYTE_POINTER(p, ch, c);
		m_sLength=c;
	}
	return *this;
}
CRJStringByteVar& CRJStringByteVar::CatFill(size_t c, uint8_t ch) {
	if (c>0&& HasNeededSize(m_sLength+c)) {
		uint8_t* p=(uint8_t*) GetTail();
		_MEMSET_BYTE_POINTER(p, ch, c);
		m_sLength+=c;
	}
	return *this;
}
CRJStringByteVar& CRJStringByteVar::SetInt64PaddingLeft(int64_t lValue, size_t w, uint8_t ch) {
	uint8_t tmp[32];
	size_t s=CRJByteConvertor::Int64ToBytes(lValue, tmp,sizeof(tmp));
	w&=0x7F;
	if (s>=w) return SetData(tmp, s);
	w-=s;
	if ((tmp[0]=='-')&&
		(ch=='0')) return SetSingle('-').CatFill(w, ch).CatData(& tmp[1], s-1);
	return SetFill(w, ch).CatData(tmp, s);
}
CRJStringByteVar& CRJStringByteVar::SetSizePaddingLeft(size_t lValue, size_t w, uint8_t ch) {
	uint8_t tmp[32];
	size_t s=CRJByteConvertor::SizeToBytes(lValue, tmp,sizeof(tmp));
	w&=0x7F;
	if (s>=w) return CatData(tmp, s);
	w-=s;
	return SetFill(w, ch).CatData(tmp, s);
}
CRJStringByteVar& CRJStringByteVar::CatSingle(uint8_t ch) {
	return CatDataCheckLength(& ch, 1);
}
CRJStringByteVar& CRJStringByteVar::Cat(const CRJStringByte& clStr) {
	return CatDataCheckLength(clStr.GetHeadForRead(), clStr.GetLength());
}
CRJStringByteVar& CRJStringByteVar::Cat(const CRJStringUnicode& s) {
	CRJStringByteExpanded tmp;
	return Cat(tmp.Set(s));
}
CRJStringByteVar& CRJStringByteVar::Cat(const std::string& s) {
	return CatData((const uint8_t*) s.c_str(), s.length());
}
CRJStringByteVar& CRJStringByteVar::CatDataAsLabel(uint8_t* p, size_t lVol) {
	p[lVol-1]=0x00;
	return CatData(p, strlen((char*) p));
}
CRJStringByteVar& CRJStringByteVar::CatData(const uint8_t* p, size_t l) {
	return (p)?
		CatDataCheckLength(p, l):
		SetStringVarError(eUChainErrors::NoPointer);
}
CRJStringByteVar& CRJStringByteVar::CatLabel(const char* p, size_t l) {
	return (p)?
		CatDataCheckLength((const uint8_t*) p, l):
		SetStringVarError(eUChainErrors::NoPointer);
}
CRJStringByteVar& CRJStringByteVar::CatLabel(const wchar_t* p) {
	CRJStringByteExpanded s;
	CRJStringUnicode label(p);
	CRJEncoding::translateUnicodeToUtf8(label, s);
	if (s.HasVarError()) return SetStringVarError(eUChainErrors::NoSpace);
	return Cat(s);
}
CRJStringByteVar& CRJStringByteVar::CatDataCheckLength(const uint8_t* p, size_t l) {
	if (HasNeededSize(l+m_sLength)) return CatDataSafed(p, l);
	return SetStringVarError(eUChainErrors::NoSpace);
}
CRJStringByteVar& CRJStringByteVar::CatDataSafed(const uint8_t* p, size_t l) {
	if (l>0) {
		uint8_t* q=(uint8_t*) GetTail();
		_MEMMOVE_BYTE_POINTER(q, p, l);
	}
	return SetLengthSafed(l+m_sLength);
}
CRJStringByteVar& CRJStringByteVar::CatInt64(int64_t lValue) {
	uint8_t tmp[32];
	size_t s=CRJByteConvertor::Int64ToBytes(lValue, tmp,sizeof(tmp));
	return CatData(tmp, s);
}
CRJStringByteVar& CRJStringByteVar::CatDouble(double d) {
	size_t v = GetCapacity();
	if (v > 1) {
		char* p = (char*)GetTail();
		auto res = std::to_chars(p, p + v - 1, d);
		if (res.ec == std::errc()) m_sLength += (res.ptr - p);
		else SetStringVarError(eUChainErrors::NoSpace);
	}
	return *this;
}
CRJStringByteVar& CRJStringByteVar::CatSize(size_t lValue) {
	uint8_t tmp[32];
	size_t s=CRJByteConvertor::SizeToBytes(lValue, tmp,sizeof(tmp));
	return CatData(tmp, s);
}
CRJStringByteVar& CRJStringByteVar::CatInt64Fmt(int64_t lValue, size_t w, uint8_t f) {
	uint8_t tmp[32];
	uint8_t fmt[128];
	size_t s=CRJByteConvertor::Int64ToBytes(lValue, tmp,sizeof(tmp));
	w&=0x7F;
	if (s>=w) return CatData(tmp, s);
	w-=s;
	_MEMSET_BYTE_POINTER(fmt, f, w);
	if ((tmp[0]=='-')&&
		(f=='0')) return CatSingle('-').CatData(fmt, w).CatData(& tmp[1], s-1);
	return CatData(fmt, w).CatData(tmp, s);
}
CRJStringByteVar& CRJStringByteVar::CatSizeFmt(size_t lValue, size_t w, uint8_t f) {
	uint8_t tmp[32];
	uint8_t fmt[128];
	size_t s=CRJByteConvertor::SizeToBytes(lValue, tmp,sizeof(tmp));
	w&=0x7F;
	if (s>=w) return CatData(tmp, s);
	w-=s;
	_MEMSET_BYTE_POINTER(fmt, f, w);
	return CatData(fmt, w).CatData(tmp, s);
}
CRJStringByteVar& CRJStringByteVar::CatPath() {
	return CatSingle((uint8_t) std::filesystem::path::preferred_separator);
}
CRJStringByteVar& CRJStringByteVar::SubString(const CRJStringByte& str, size_t lFrom) {
	size_t lCount=str.GetLength();
	if (lFrom<lCount) return SetDataCheckLength(str.GetHeadForRead()+lFrom, lCount-lFrom);
	return SetStringVarError(eUChainErrors::BadPos);
}
CRJStringByteVar& CRJStringByteVar::SubString(const CRJStringByte& str, size_t lFrom, size_t lTo) {
	if ((lFrom<=lTo)&&
		(lTo<=str.GetLength())) return SetDataCheckLength(str.GetHeadForRead()+lFrom, lTo-lFrom);
	return SetStringVarError(eUChainErrors::BadPos);
}
CRJStringByteVar& CRJStringByteVar::RemoveAndTailLeft(size_t lOffset) {
	if (lOffset>=m_sLength) return SetStringVarError(eUChainErrors::BadPos);
	uint8_t* pData=GetHeadForUpdate()+lOffset;
	m_sLength--;
	_MEMMOVE_BYTE_POINTER(pData, pData+1, m_sLength-lOffset);
	return *this;
}
CRJStringByteVar& CRJStringByteVar::Replace(uint8_t chFind, uint8_t chReplace) {
	if (m_sLength>0) {
		CRJChainFinder st;
		uint8_t* pData=GetHeadForUpdate();
		bool b=FindPos(chFind, st);
		while (b) {
			pData[st.m_sFound]=chReplace;
			b=FindPosNext(chFind, st);
		}
	}
	return *this;
}
CRJStringByteVar& CRJStringByteVar::ReplaceSet(const CRJStringByte& set, uint8_t chReplace) {
	const uint8_t* pData=set.GetHeadForRead();
	size_t l=set.GetLength();
	while (l>0) {
		Replace(* pData, chReplace);
		l--;
		pData++;
	}
	return *this;
}
CRJStringByteVar& CRJStringByteVar::DeleteChar(uint8_t ch) {
	CRJStringByteFlex h, w(*this);
	if (w.WasFoundTopAndMovedHead(ch, h)) {
		SetLengthSafed(h.GetLength());
		while (w.WasFoundTopAndMovedHead(ch, h)) Cat(h);
		Cat(w);
	}
	return *this;
}
CRJStringByteVar& CRJStringByteVar::TrimLeft(uint8_t ch) {
	CRJChainFinder st;
	if (FindPosNotSameLeft(ch, st)) {
		if (st.m_sFound>0) return SetDataSafed(GetHeadForRead()+st.m_sFound, m_sLength-st.m_sFound);
	}
	else SetEmpty();
	return *this;
}
CRJStringByteVar& CRJStringByteVar::TrimRight(uint8_t ch) {
	CRJChainFinder st;
	if (FindPosNotSameRight(ch, st)) return SetLengthSafed(st.m_sFound+1);
	else SetEmpty();
	return *this;
}
CRJStringByteVar& CRJStringByteVar::Trim(uint8_t ch) {
	return TrimRight(ch).TrimLeft(ch);
}
bool CRJStringByteVar::WasFoundTopAndRemovedHead(uint8_t ch) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			st.m_sFound++;
			SetData(GetHeadForRead()+st.m_sFound, m_sLength-st.m_sFound);
			return true;
		}
	}
	return false;
}
bool CRJStringByteVar::WasFoundTopAndRemovedTail(uint8_t ch) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	return false;
}
bool CRJStringByteVar::WasFoundTopAndMovedTail(uint8_t ch, CRJStringByteVar& clSplit) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			size_t l=st.m_sFound+1;
			clSplit.SetData(GetHeadForRead()+l, m_sLength-l);
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringByteVar::WasFoundTopAndMovedHead(uint8_t ch, CRJStringByteVar& clSplit) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			clSplit.SetData(GetHeadForRead(), st.m_sFound);
			st.m_sFound++;
			SetDataSafed(GetHeadForRead()+st.m_sFound, m_sLength-st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringByteVar::WasFoundBackAndRemovedHead(uint8_t ch) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			st.m_sFound++;
			SetData(GetHeadForRead()+st.m_sFound, m_sLength-st.m_sFound);
			return true;
		}
	}
	return false;
}
bool CRJStringByteVar::WasFoundBackAndRemovedTail(uint8_t ch) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	return false;
}
bool CRJStringByteVar::WasFoundBackAndMovedTail(uint8_t ch, CRJStringByteVar& clSplit) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			size_t l=st.m_sFound+1;
			clSplit.SetData(GetHeadForRead()+l, m_sLength-l);
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringByteVar::WasFoundBackAndMovedHead(uint8_t ch, CRJStringByteVar& clSplit) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			clSplit.SetData(GetHeadForRead(), st.m_sFound);
			st.m_sFound++;
			SetDataSafed(GetHeadForRead()+st.m_sFound, m_sLength-st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringByteVar::WasFoundTopAndMovedTail(uint8_t ch, CRJStringByteFlex& clResult) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			size_t l=st.m_sFound+1;
			clResult.SetFromData(GetHeadForRead()+l, m_sLength-l);
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	clResult.SetEmpty();
	return false;
}
bool CRJStringByteVar::WasFoundTopAndMovedHead(uint8_t ch, CRJStringByteFlex& clResult) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			clResult.SetFromData(GetHeadForRead(), st.m_sFound);
			st.m_sFound++;
			SetDataSafed(GetHeadForRead()+st.m_sFound, m_sLength-st.m_sFound);
			return true;
		}
	}
	clResult.SetEmpty();
	return false;
}
bool CRJStringByteVar::WasFoundBackAndMovedTail(uint8_t ch, CRJStringByteFlex& clResult) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			size_t l=st.m_sFound+1;
			clResult.SetFromData(GetHeadForRead()+l, m_sLength-l);
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	clResult.SetEmpty();
	return false;
}
bool CRJStringByteVar::WasFoundBackAndMovedHead(uint8_t ch, CRJStringByteFlex& clResult) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			clResult.SetFromData(GetHeadForRead(), st.m_sFound);
			st.m_sFound++;
			SetDataSafed(GetHeadForRead()+st.m_sFound, m_sLength-st.m_sFound);
			return true;
		}
	}
	clResult.SetEmpty();
	return false;
}
CRJStringByteVar& CRJStringByteVar::SetLengthSafed(size_t s) {
	m_sLength=s;
	return *this;
}
CRJStringByteVar& CRJStringByteVar::SetLength(size_t s) {
	return (m_sSize>=s)?
		SetLengthSafed(s):
		SetStringVarError(eUChainErrors::BadPos);
}
CRJStringByteVar& CRJStringByteVar::CatCRLF() {
	return Cat(CRJStringByte::s_CRLF);
}
CRJStringByteVar& CRJStringByteVar::CatLN() {
	return Cat(CRJStringByte::s_LineNext);
}
CRJStringByteVar& CRJStringByteVar::CatLF() {
	return Cat(CRJStringByte::s_LineFeed);
}
CRJStringByteVar& CRJStringByteVar::TruncLeft(size_t s) {
	if (s<=m_sLength) {
		uint8_t* p=GetHeadForUpdate();
		m_sLength-=s;
		_MEMMOVE_BYTE_POINTER(p,& p[s], m_sLength);
	}
	else m_sLength=0;
	return *this;
}
CRJStringByteVar& CRJStringByteVar::TruncRight(size_t s) {
	if (m_sLength>=s) m_sLength-=s;
	else m_sLength=0;
	return *this;
}
CRJStringByteVar& CRJStringByteVar::DecodeNum(const CRJStringUnicode& s) {
	size_t lLen=s.GetLength();
	if (HasNeededSize(lLen)) {
		wchar_t ch;
		const wchar_t* pSrc=s.GetHeadForRead();
		uint8_t* pData=GetHeadForUpdate();
		size_t r=lLen;
		while (lLen>0) {
			lLen--;
			ch=* pSrc;
			if (ch<=0x7F)* pData=(uint8_t) ch;
			else return SetStringVarError(eUChainErrors::Decode);
			pData++;
			pSrc++;
		}
		return SetLengthSafed(r);
	}
	return SetStringVarError(eUChainErrors::NoSpace);
}
CRJStringByteVar& CRJStringByteVar::DeleteHexSymbols() {
	CRJHexStringParser parser(*this);
	return parser.Parse();
}
CRJStringByteVar& CRJStringByteVar::BuildHexSymbols(const CRJStringByte& s) {
	CRJHexStringBuilder parser(s.GetHeadForRead(), s.GetLength());
	return parser.Build(*this);
}
CRJStringByteVar& CRJStringByteVar::BuildHexSymbols(const uint8_t* p, size_t l) {
	CRJHexStringBuilder parser(p, l);
	return parser.Build(*this);
}
CRJStringByteVar& CRJStringByteVar::SetLast(uint8_t ch) {
	if (!m_sLength) return SetSingle(ch);
	GetHeadForUpdate()[m_sLength-1]=ch;
	return *this;
}
CRJStringByteVar& CRJStringByteVar::MakeCLabel() {
	if (m_sLength<m_sSize)*((uint8_t*) GetTail())=0;
	else if (m_sSize>0) {
		m_sLength=m_sSize-1;
		*((uint8_t*) GetTail())=0;
		SetStringVarError(eUChainErrors::WasTruncated);
	}
	else {
		SetStringVarError(eUChainErrors::BadSize);
#ifdef _DEBUG
		CRJDataTools::DebugBreak();
#endif 
	}
	return *this;
}
CRJStringByteVar& CRJStringByteVar::FormatAlignedWidth(CRJStringByteVar& strBuf, size_t width,bool bAlign) {
	try {
		if ((width>0)&&
			(width<m_sSize)&&
			strBuf.Set(*this).OK()) {
			char fmt[32];
			if (bAlign) sprintf_s(fmt,sizeof(fmt),"%c-%zu",'%', width);
			else sprintf_s(fmt,sizeof(fmt),"%c%zu",'%', width);
			SetLength(sprintf_s((char*) GetHeadForUpdate(), m_sSize-1, fmt, strBuf.GetHeadForRead()));
		}
	}
	catch(...) {}
	return *this;
}
CRJStringByteVar& CRJStringByteVar::SetDTime(time_t t) {
	return SetLengthSafed(CRJDTimeTools::SetDTAppl(t, GetHeadForUpdate(), m_sSize));
}
void CRJStringByteVar::CBytesCopyBlock(uint8_t* pDst, size_t lDst,const uint8_t* pSrc) {
	CRJDataTools::MoveBytes(pDst, pSrc, lDst);
	pDst[lDst-1]=0x00;
}
size_t CRJStringByteVar::CBytesCopyString(uint8_t* pDst, size_t lDst,const CRJStringByte& clSrc) {
	size_t lSrc=clSrc.GetLength();
	if (lSrc>=lDst) lSrc=lDst-1;
	CRJDataTools::MoveBytes((uint8_t*) pDst, clSrc.GetHeadForRead(), lSrc);
	pDst[lSrc]=0x00;
	return lSrc;
}
size_t CRJStringByteVar::CBytesCopyLabel(uint8_t* pDst, size_t lDst,const char* pSrc, size_t lSrc) {
	try {
		if (pSrc) {
			if (lSrc==0) lSrc=::strlen(pSrc);
			if (lSrc>=lDst) lSrc=lDst-1;
			CRJDataTools::MoveBytes((uint8_t*) pDst,(uint8_t*) pSrc, lSrc);
			pDst[lSrc]=0x00;
			return lSrc;
		}
		else pDst[0]=0x00;
	}
	catch(...) {}
	return 0;
}
CRJStringByteVar& CRJStringByteVar::SetDoubleFormatted(_In_z_ const char* pFmt,double d) {
	int l=sprintf_s(GetHeadForUpdateC(), m_sSize, pFmt, d);
	if (l>0) m_sLength=(size_t) l;
	else {
		SetEmpty();
		SetStringVarError(eUChainErrors::BadFormat);
	}
	return *this;
}
CRJStringByteVar& CRJStringByteVar::SetInt64Formatted(_In_z_ const char* pFmt, int64_t v) {
	int l=sprintf_s(GetHeadForUpdateC(), m_sSize, pFmt, v);
	if (l>0) m_sLength=(size_t) l;
	else {
		SetEmpty();
		SetStringVarError(eUChainErrors::BadFormat);
	}
	return *this;
}
CRJStringByteVar& CRJStringByteVar::SetSizeFormatted(_In_z_ const char* pFmt, size_t v) {
	int l=sprintf_s(GetHeadForUpdateC(), m_sSize, pFmt, v);
	if (l>0) m_sLength=(size_t) l;
	else {
		SetEmpty();
		SetStringVarError(eUChainErrors::BadFormat);
	}
	return *this;
}

