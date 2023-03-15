#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/unicode/var.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/strings/unicode/flex.h>
#include <recurj/rjframework/libs/base/strings/unicode/string.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/strings/unicode/allocated.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
using namespace rjBase;
CRJStringUnicodeVar::CRJStringUnicodeVar():
	m_sSize(0) {}
CRJStringUnicodeVar::CRJStringUnicodeVar(size_t s):
	m_sSize(s) {}
CRJStringUnicodeVar::~CRJStringUnicodeVar() {}
bool CRJStringUnicodeVar::HasNeededSize(size_t s) {
	return m_sSize>=s;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetStringVarError(eUChainErrors e) {
	return SetVarError(e);
}
wchar_t* CRJStringUnicodeVar::GetHeadForUpdate() const {
	return const_cast<wchar_t*>(GetHeadForRead());
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetEmpty() {
	m_clResult.ResetError();
	return SetLengthSafed(0);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetAt(size_t s,wchar_t v) {
	if (s<m_sLength) GetHeadForUpdate()[s]=v;
	else return SetStringVarError(eUChainErrors::BadPos);
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::Set(const CRJStringUnicode& clStr) {
	return SetDataCheckLength(clStr.GetHeadForRead(), clStr.GetLength());
}
CRJStringUnicodeVar& CRJStringUnicodeVar::Set(const CRJStringByte& s) {
	return CRJEncoding::translateUtf8ToUnicode(s,*this);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::Set(const std::wstring& s) {
	return SetData(s.c_str(), s.length());
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetSingle(wchar_t ch) {
	return SetDataCheckLength(& ch, 1);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetData(const wchar_t* p, size_t l) {
	return (p)?
		SetDataCheckLength(p, l):
		SetStringVarError(eUChainErrors::NoPointer);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetDataCheckLength(const wchar_t* p, size_t l) {
	if (HasNeededSize(l)) {
		if (l<=0) return SetEmpty();
		else return SetDataSafed(p, l);
	}
	return SetStringVarError(eUChainErrors::NoSpace);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetDataSafed(const wchar_t* p, size_t l) {
	_MEMMOVE_UNICODE_POINTER(GetHeadForUpdate(), p, l);
	return SetLengthSafed(l);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetDataAsLabel(wchar_t* p, size_t lVol) {
	p[lVol-1]=0x00;
	return SetData(p, wcslen(p));
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetDataAsLabel(uint8_t* p, size_t lVol) {
	CRJStringByteExpanded s;
	s.SetDataAsLabel(p, lVol);
	return CRJEncoding::translateUtf8ToUnicode(s,*this);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetInt64(int64_t lValue) {
	m_sLength=CRJByteConvertor::Int64ToUnicodes(lValue, GetHeadForUpdate(), m_sSize);
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetSize(size_t lValue) {
	m_sLength=CRJByteConvertor::SizeToUnicodes(lValue, GetHeadForUpdate(), m_sSize);
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetHex(size_t lValue) {
	m_sLength=CRJByteConvertor::HexToUnicodes(lValue, GetHeadForUpdate(), m_sSize);
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetBool(bool b) {
	return Set((b)?
		CRJStringUnicode::s_WordYES:CRJStringUnicode::s_WordNO);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetInt64PaddingLeft(int64_t lValue, size_t w,wchar_t f) {
	wchar_t tmp[32];
	wchar_t fmt[128];
	size_t s=CRJByteConvertor::Int64ToUnicodes(lValue, tmp,sizeof(tmp));
	w&=0x7F;
	if (s>=w) return CatData(tmp, s);
	w-=s;
	_MEMSET_UNICODE_POINTER(fmt, f, w);
	if ((tmp[0]=='-')&&
		(f=='0')) return SetSingle('-').CatData(fmt, w).CatData(& tmp[1], s-1);
	return SetData(fmt, w).CatData(tmp, s);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetSizePaddingLeft(size_t lValue, size_t w,wchar_t f) {
	wchar_t tmp[32];
	wchar_t fmt[128];
	size_t s=CRJByteConvertor::SizeToUnicodes(lValue, tmp,sizeof(tmp));
	w&=0x7F;
	if (s>=w) return CatData(tmp, s);
	w-=s;
	_MEMSET_UNICODE_POINTER(fmt, f, w);
	return SetData(fmt, w).CatData(tmp, s);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::CatSingle(wchar_t ch) {
	return CatDataCheckLength(& ch, 1);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::Cat(const CRJStringUnicode& clStr) {
	return CatDataCheckLength(clStr.GetHeadForRead(), clStr.GetLength());
}
CRJStringUnicodeVar& CRJStringUnicodeVar::Cat(const CRJStringByte& s) {
	CRJStringUnicodeExpanded tmp;
	return Cat(tmp.Set(s));
}
CRJStringUnicodeVar& CRJStringUnicodeVar::Cat(const std::wstring& s) {
	return CatData(s.c_str(), s.length());
}
CRJStringUnicodeVar& CRJStringUnicodeVar::CatData(const wchar_t* p, size_t l) {
	return (p)?
		CatDataCheckLength(p, l):
		SetStringVarError(eUChainErrors::NoPointer);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::CatDataCheckLength(const wchar_t* p, size_t l) {
	if (HasNeededSize(l+m_sLength)) return CatDataSafed(p, l);
	return SetStringVarError(eUChainErrors::NoSpace);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::CatDataSafed(const wchar_t* p, size_t l) {
	wchar_t* q=(wchar_t*) GetTail();
	_MEMMOVE_UNICODE_POINTER(q, p, l);
	return SetLengthSafed(l+m_sLength);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::CatInt64(int64_t lValue) {
	wchar_t tmp[32];
	size_t s=CRJByteConvertor::Int64ToUnicodes(lValue, tmp,sizeof(tmp));
	return CatData(tmp, s);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::CatSize(size_t lValue) {
	wchar_t tmp[32];
	size_t s=CRJByteConvertor::SizeToUnicodes(lValue, tmp,sizeof(tmp));
	return CatData(tmp, s);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::CatInt64Fmt(int64_t lValue, size_t w,wchar_t f) {
	wchar_t tmp[32];
	wchar_t fmt[128];
	size_t s=CRJByteConvertor::Int64ToUnicodes(lValue, tmp,sizeof(tmp));
	w&=0x7F;
	if (s>=w) return CatData(tmp, s);
	w-=s;
	_MEMSET_UNICODE_POINTER(fmt, f, w);
	if ((tmp[0]==L'-')&&
		(f==L'0')) return CatSingle(L'-').CatData(fmt, w).CatData(& tmp[1], s-1);
	return CatData(fmt, w).CatData(tmp, s);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::CatSizeFmt(size_t lValue, size_t w,wchar_t f) {
	wchar_t tmp[32];
	wchar_t fmt[128];
	size_t s=CRJByteConvertor::SizeToUnicodes(lValue, tmp,sizeof(tmp));
	w&=0x7F;
	if (s>=w) return CatData(tmp, s);
	w-=s;
	_MEMSET_UNICODE_POINTER(fmt, f, w);
	return CatData(fmt, w).CatData(tmp, s);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SubString(const CRJStringUnicode& str, size_t lFrom) {
	size_t lCount=str.GetLength();
	if (lFrom<lCount) return SetDataCheckLength(str.GetHeadForRead()+lFrom, lCount-lFrom);
	return SetStringVarError(eUChainErrors::BadPos);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SubString(const CRJStringUnicode& str, size_t lFrom, size_t lTo) {
	if ((lFrom<=lTo)&&
		(lTo<=str.GetLength())) return SetDataCheckLength(str.GetHeadForRead()+lFrom, lTo-lFrom);
	return SetStringVarError(eUChainErrors::BadPos);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::RemoveAndTailLeft(size_t lOffset) {
	if (lOffset>=m_sLength) return SetStringVarError(eUChainErrors::BadPos);
	wchar_t* pData=GetHeadForUpdate()+lOffset;
	m_sLength--;
	_MEMMOVE_UNICODE_POINTER(pData, pData+1, m_sLength-lOffset);
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::Replace(wchar_t chFind,wchar_t chReplace) {
	if (m_sLength>0) {
		CRJChainFinder st;
		wchar_t* pData=GetHeadForUpdate();
		bool b=FindPos(chFind, st);
		while (b) {
			pData[st.m_sFound]=chReplace;
			b=FindPosNext(chFind, st);
		}
	}
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::ReplaceSet(const CRJStringUnicode& set,wchar_t chReplace) {
	const wchar_t* pData=set.GetHeadForRead();
	size_t l=set.GetLength();
	while (l>0) {
		Replace(* pData, chReplace);
		l--;
		pData++;
	}
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::DeleteChar(wchar_t ch) {
	CRJChainFinder st;
	wchar_t* pDst,* pSrc=GetHeadForUpdate();
	size_t l=Trim(ch).GetLength();
	while (FindPos(ch, st)) {
		pDst=pSrc+st.m_sFound;
		_MEMMOVE_UNICODE_POINTER(pDst, pDst+1, l-st.m_sFound-1);
		l--;
	}
	return SetLengthSafed(l);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::TrimLeft(wchar_t ch) {
	CRJChainFinder st;
	if ((FindPosNotSameLeft(ch, st))&&
		(st.m_sFound>0)) return SetDataSafed(GetHeadForRead()+st.m_sFound, m_sLength-st.m_sFound);
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::TrimRight(wchar_t ch) {
	CRJChainFinder st;
	if (FindPosNotSameRight(ch, st)) return SetLengthSafed(st.m_sFound+1);
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::Trim(wchar_t ch) {
	return TrimRight(ch).TrimLeft(ch);
}
bool CRJStringUnicodeVar::WasFoundTopAndMovedTail(wchar_t ch, CRJStringUnicodeVar& clSplit) {
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
bool CRJStringUnicodeVar::WasFoundTopAndMovedHead(wchar_t ch, CRJStringUnicodeVar& clSplit) {
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
bool CRJStringUnicodeVar::WasFoundBackAndMovedTail(wchar_t ch, CRJStringUnicodeVar& clSplit) {
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
bool CRJStringUnicodeVar::WasFoundBackAndMovedHead(wchar_t ch, CRJStringUnicodeVar& clSplit) {
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
bool CRJStringUnicodeVar::WasFoundTopAndMovedTail(wchar_t ch, CRJStringUnicodeFlex& clResult) {
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
bool CRJStringUnicodeVar::WasFoundTopAndMovedHead(wchar_t ch, CRJStringUnicodeFlex& clResult) {
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
bool CRJStringUnicodeVar::WasFoundBackAndMovedTail(wchar_t ch, CRJStringUnicodeFlex& clResult) {
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
bool CRJStringUnicodeVar::WasFoundBackAndMovedHead(wchar_t ch, CRJStringUnicodeFlex& clResult) {
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
CRJStringUnicodeVar& CRJStringUnicodeVar::SetLengthSafed(size_t s) {
	m_sLength=s;
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::SetLength(size_t s) {
	return (m_sSize>=s)?
		SetLengthSafed(s):
		SetStringVarError(eUChainErrors::BadPos);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::CatCRLF() {
	return Cat(CRJStringUnicode::s_CRLF);
}
CRJStringUnicodeVar& CRJStringUnicodeVar::TruncLeft(size_t s) {
	if (s<=m_sLength) {
		wchar_t* p=GetHeadForUpdate();
		m_sLength-=s;
		_MEMMOVE_UNICODE_POINTER(p,& p[s], m_sLength);
	}
	else m_sLength=0;
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::TruncRight(size_t s) {
	if (m_sLength>=s) m_sLength-=s;
	else m_sLength=0;
	return *this;
}
CRJStringUnicodeVar& CRJStringUnicodeVar::DecodeNum(const CRJStringByte& s) {
	size_t lLen=s.GetLength();
	if (HasNeededSize(lLen)) {
		const uint8_t* pSrc=s.GetHeadForRead();
		wchar_t* pData=GetHeadForUpdate();
		size_t r=lLen;
		while (lLen>0) {
			lLen--;
			* pData=(wchar_t)(* pSrc);
			pData++;
			pSrc++;
		}
		return SetLengthSafed(r);
	}
	return SetStringVarError(eUChainErrors::NoSpace);
}

