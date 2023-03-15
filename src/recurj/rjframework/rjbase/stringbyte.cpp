#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/base/datatools/bytepacker.h>
#include <string>
#include <cctype>
#include <charconv>
using namespace rjBase;
const CRJStringByte CRJStringByte::s_WordYES("YES", 3);
const CRJStringByte CRJStringByte::s_WordTRUE("TRUE", 4);
const CRJStringByte CRJStringByte::s_WordNO("NO", 2);
const CRJStringByte CRJStringByte::s_WordFALSE("FALSE", 5);
const CRJStringByte CRJStringByte::s_Word1("1", 1);
const CRJStringByte CRJStringByte::s_Word0("0", 1);
const CRJStringByte CRJStringByte::s_WordN("N", 1);
const CRJStringByte CRJStringByte::s_WordY("Y", 1);
const CRJStringByte CRJStringByte::s_clEmptyStringChar("", 0);
#ifdef _rjOS_FAMILY_UNIX
 const CRJStringByte CRJStringByte::s_LineNext("\x0A", 1);
#endif 
#ifdef _rjOS_WIN
 const CRJStringByte CRJStringByte::s_LineNext("\x0D\x0A", 2);
#endif 
 const CRJStringByte CRJStringByte::s_LineFeed("\x0A", 1);
const CRJStringByte CRJStringByte::s_CRLF("\x0D\x0A", 2);
const uint8_t CRJStringByte::s_uUnDef[1]= {
	0x00
};
const uint8_t CRJStringByte::s_uEmpty[1]= {
	0x00
};
const uint8_t* CRJStringByte::FindTopThat(const uint8_t* p, size_t lLen, uint8_t ch) {
	for (size_t i=0; i<lLen; i++)
	if (p[i]==ch) return p+i;
	return nullptr;
}
const uint8_t* CRJStringByte::FindTopNotThat(const uint8_t* p, size_t lLen, uint8_t ch) {
	for (size_t i=0; i<lLen; i++)
	if (p[i]!=ch) return p+i;
	return nullptr;
}
CRJStringByte::CRJStringByte(const char* p, size_t lLen) {
	if (p) {
		if (!lLen) lLen=::strlen(p);
		SetHeadAndLength((uint8_t*) p, lLen);
	}
	else SetConstantUndefined();
}
CRJStringByte::CRJStringByte() {
	m_pHead=CRJStringByte::s_uUnDef;
}
CRJStringByte::~CRJStringByte() {}
CRJStringByteFlex CRJStringByte::CreateBlockByte() const {
	return CRJStringByteFlex((uint8_t*) m_pHead, m_sLength);
}
const uint8_t* CRJStringByte::GetHeadByOffset(size_t off) const {
	return m_pHead + off;
}

CRJStringByte& CRJStringByte::GetAt(size_t lPos, uint8_t& ch) {
	if ((m_sLength>0)&&
		(lPos<m_sLength)) ch=GetHeadForRead()[lPos];
	else ch=0x00;
	return *this;
}
bool CRJStringByte::Contain(uint8_t ch) const {
	return _MEMFIND_BYTE_POINTER(GetHeadForRead(), ch, m_sLength)!=nullptr;
}
bool CRJStringByte::FindPos(uint8_t ch, CRJChainFinder& clFound) const {
	return FindPosSame(GetHeadForRead(), m_sLength, ch, 0, clFound);
}
bool CRJStringByte::FindPosNext(uint8_t ch, CRJChainFinder& clFound) const {
	size_t lLast=clFound.m_sFound+1;
	return (m_sLength>lLast)?
		FindPosSame(GetHeadForRead()+lLast, m_sLength-lLast, ch, lLast, clFound):
		clFound.NotFoundPos();
}
bool CRJStringByte::FindPosBack(uint8_t ch, CRJChainFinder& clFound) const {
	return FindPosSameBack(GetTail(), m_sLength, ch, clFound);
}
bool CRJStringByte::FindPosNextBack(uint8_t ch, CRJChainFinder& clFound) const {
	return ((clFound.m_sFound>0)&&
		(clFound.m_sFound<m_sLength))?
		FindPosSameBack(GetHeadForRead()+clFound.m_sFound, clFound.m_sFound, ch, clFound):
		clFound.NotFoundPos();
}
bool CRJStringByte::FindPosNotSameLeft(uint8_t ch, CRJChainFinder& clFound) const {
	return FindPosNotSame(GetHeadForRead(), m_sLength, ch, 0, clFound);
}
bool CRJStringByte::FindPosNotSameLeftNext(uint8_t ch, CRJChainFinder& clFound) const {
	size_t lLast=clFound.m_sFound+1;
	return (m_sLength>lLast)?
		FindPosNotSame(GetHeadForRead()+lLast, m_sLength-lLast, ch, lLast, clFound):
		clFound.NotFoundPos();
}
bool CRJStringByte::FindPosNotSameRight(uint8_t ch, CRJChainFinder& clFound) const {
	return FindPosNotSameBack(GetTail(), m_sLength, ch, clFound);
}
bool CRJStringByte::FindPosNotSameRightNext(uint8_t ch, CRJChainFinder& clFound) const {
	return ((clFound.m_sFound>0)&&
		(clFound.m_sFound<m_sLength))?
		FindPosNotSameBack(GetHeadForRead()+clFound.m_sFound, clFound.m_sFound, ch, clFound):
		clFound.NotFoundPos();
}
bool CRJStringByte::FindPosInSet(const CRJStringByte& clMask, CRJChainFinderSet& clFound) const {
	CRJChainFinder st;
	const uint8_t* pData=GetHeadForRead();
	size_t i, f=0, c=clMask.GetLength(),
	s=m_sLength;
	for (i=0; i<c; i++) {
		if (FindPosSame(pData, s, clMask[i], 0, st)) {
			s=st.m_sFound;
			clFound.m_sIndex=i;
			if (!s) return clFound.FoundPos(0);
			f=1;
		}
	}
	return (f==0)?
		clFound.NotFoundPos():
		clFound.FoundPos(s);
}
bool CRJStringByte::FindPosInSetNext(const CRJStringByte& clMask, CRJChainFinderSet& clFound) const {
	size_t lLast=clFound.m_sFound+1;
	if (m_sLength>lLast) {
		CRJChainFinder st;
		size_t i, f=0, c=clMask.GetLength(),
		s=m_sLength-lLast;
		const uint8_t* pData=GetHeadForRead()+lLast;
		for (i=0; i<c; i++) {
			if (FindPosSame(pData, s, clMask[i], 0, st)) {
				s=st.m_sFound;
				clFound.m_sIndex=i;
				f=1;
			}
		}
		return (f==0)?
			clFound.NotFoundPos():
			clFound.FoundPos(s+clFound.m_sFound);
	}
	return clFound.NotFoundPos();
}
bool CRJStringByte::FindPosInSetBack(const CRJStringByte& clMask, CRJChainFinderSet& clFound) const {
	CRJChainFinder st;
	size_t i, f=0, c=clMask.GetLength(),
	s=m_sLength;
	const uint8_t* pData=GetHeadForRead();
	for (i=0; i<c; i++) {
		if (FindPosSameBack(pData+s, s, clMask[i], st)) {
			s=st.m_sFound;
			clFound.m_sIndex=i;
			if (!s) return clFound.FoundPos(0);
			f=1;
		}
	}
	return (f==0)?
		clFound.NotFoundPos():
		clFound.FoundPos(s);
}
bool CRJStringByte::FindPosInSetNextBack(const CRJStringByte& clMask, CRJChainFinderSet& clFound) const {
	if ((clFound.m_sFound>0)&&
		(m_sLength>clFound.m_sFound)) {
		CRJChainFinder st;
		size_t i, f=0, c=clMask.GetLength(),
		s=clFound.m_sFound;
		const uint8_t* pData=GetHeadForRead();
		for (i=0; i<c; i++) {
			if (FindPosSameBack(pData+s, s, clMask[i], st)) {
				s=st.m_sFound;
				clFound.m_sIndex=i;
				if (!s) return clFound.FoundPos(0);
				f=1;
			}
		}
		return (f==0)?
			clFound.NotFoundPos():
			clFound.FoundPos(s);
	}
	return clFound.NotFoundPos();
}
bool CRJStringByte::FindSubStr(const CRJStringByte& clSubStr, CRJChainFinder& clFound, size_t off) const {
	size_t sLen=clSubStr.GetLength();
	if (m_sLength>off&&sLen<=(m_sLength-off)) {
		CRJChainFinder st;
		size_t sFirst=off, sLast=m_sLength-sLen+1;
		const uint8_t* pData=GetHeadForRead();
		const uint8_t* pSubStr=clSubStr.GetHeadForRead();
		uint8_t chFirst=pSubStr[0];
		while (FindPosSame(pData+sFirst, sLast-sFirst, chFirst, sFirst, st)) {
			if (!_MEMCOMPARE_BYTE_POINTER(& pData[st.m_sFound], pSubStr, sLen)) return clFound.FoundPos(st.m_sFound);
			sFirst=st.m_sFound+1;
			if (sFirst>=sLast)break;
		}
	}
	return clFound.NotFoundPos();
}
bool CRJStringByte::FindPosSame(const uint8_t* p, size_t lLen, uint8_t ch, size_t lOffset, CRJChainFinder& clFound) {
	for (size_t i=0; i<lLen; i++)
	if (ch==p[i]) return clFound.FoundPos(i+lOffset);
	return clFound.NotFoundPos();
}
bool CRJStringByte::FindPosNotSame(const uint8_t* p, size_t lLen, uint8_t ch, size_t lOffset, CRJChainFinder& clFound) {
	for (size_t i=0; i<lLen; i++)
	if (p[i]!=ch) return clFound.FoundPos(i+lOffset);
	return clFound.NotFoundPos();
}
bool CRJStringByte::FindPosSameBack(const uint8_t* p, size_t lLen, uint8_t ch, CRJChainFinder& clFound) {
	while (lLen>0) {
		lLen--;
		p--;
		if (* p==ch) return clFound.FoundPos(lLen);
	}
	return clFound.NotFoundPos();
}
bool CRJStringByte::FindPosNotSameBack(const uint8_t* p, size_t lLen, uint8_t ch, CRJChainFinder& clFound) {
	while (lLen>0) {
		lLen--;
		p--;
		if (* p!=ch) return clFound.FoundPos(lLen);
	}
	return clFound.NotFoundPos();
}
bool CRJStringByte::FindClosingBracket(char chOpen,char chClose, size_t& lPos) const {
	char ch;
	const uint8_t* pData=GetHeadForRead();
	size_t c=1, l=GetLength();
	for (lPos=0; lPos<l; lPos++, pData++) {
		ch=pData[0];
		if (ch==chOpen) c++;
		else if (ch==chClose) {
			c--;
			if (c==0) return true;
		}
	}
	return false;
}
int64_t CRJStringByte::Compare(const CRJStringByte& str) const {
	size_t lLen=str.GetLength();
	int64_t rc=rjcompare(m_sLength, lLen);
	if (!rc) rc=_MEMCOMPARE_BYTE_POINTER(GetHeadForRead(), str.GetHeadForRead(), lLen);
	else if (rc>0) {
		if (lLen>0) {
			int64_t lLocal=_MEMCOMPARE_BYTE_POINTER(GetHeadForRead(), str.GetHeadForRead(), lLen);
			if (lLocal) rc=lLocal;
		}
	}
	else if (m_sLength>0) {
		int64_t lLocal=_MEMCOMPARE_BYTE_POINTER(GetHeadForRead(), str.GetHeadForRead(), m_sLength);
		if (lLocal) rc=lLocal;
	}
	return rc;
}
int64_t CRJStringByte::Compare(const uint8_t* pBuf, size_t lLen) const {
	CRJStringByte s((char*) pBuf, lLen);
	return Compare(s);
}
bool CRJStringByte::IsValidInt64(int64_t& v) const {
	return CRJByteConvertor::Bytes2Int64(GetHeadForRead(), GetLength(), v);
}
bool CRJStringByte::IsValidInt32(int32_t& v) const {
	int64_t t;
	if (CRJByteConvertor::Bytes2Int64(GetHeadForRead(), GetLength(), t)) {
		v=(int32_t) t;
		return true;
	}
	return false;
}
bool CRJStringByte::IsValidSize(size_t& v) const {
	return CRJByteConvertor::Bytes2Size(GetHeadForRead(), GetLength(), v);
}
bool CRJStringByte::IsValidHexSize(size_t& s) const {
	const uint8_t* pData=GetHeadForRead();
	size_t l=GetLength();
	s=0;
	while (l>0) {
		if (CRJByteConvertor::IsHexDigit(* pData)) {
			s<<=4;
			s+=CRJByteConvertor::ByteDigit2Size(* pData);
		}
		else return false;
		l--;
		pData++;
	}
	return true;
}
bool CRJStringByte::IsValidDouble(double& v) const {
	const char* p=GetHeadForReadC();
	const auto res=std::from_chars(p, p+m_sLength, v);
	return res.ec==std::errc();
}
bool CRJStringByte::IsValidBool(bool& b) const {
	return ToBoolCheck(b);
}
bool CRJStringByte::ToBoolCheck(bool& bStore) const {
	if (GetLength()>0) {
		CRJStringByteShort str(*this);
		str.Trim(' ').UpperCase();
		if ((str==CRJStringByte::s_WordYES)||
			(str==CRJStringByte::s_WordTRUE)||
			(str==CRJStringByte::s_Word1)||
			(str==CRJStringByte::s_WordY)) {
			bStore=true;
			return true;
		}
		else if ((str==CRJStringByte::s_WordNO)||
			(str==CRJStringByte::s_WordFALSE)||
			(str==CRJStringByte::s_Word0)||
			(str==CRJStringByte::s_WordN)) {
			bStore=false;
			return true;
		}
	}
	return false;
}
size_t CRJStringByte::ToSizeDef(size_t s) const {
	size_t v;
	return CRJByteConvertor::Bytes2Size(GetHeadForRead(), GetLength(), v)?
		v:s;
}
int64_t CRJStringByte::ToInt64Def(int64_t s) const {
	int64_t v;
	return CRJByteConvertor::Bytes2Int64(GetHeadForRead(), GetLength(), v)?
		v:s;
}
int32_t CRJStringByte::ToInt32Def(int32_t s) const {
	int64_t v;
	return CRJByteConvertor::Bytes2Int64(GetHeadForRead(), GetLength(), v)?
		(int32_t) v:s;
}
double CRJStringByte::ToDoubleDef(double d) const {
	double v;
	return IsValidDouble(v)?
		v:d;
}
bool CRJStringByte::ToBoolDef(bool b) const {
	bool v;
	return ToBoolCheck(v)?
		v:b;
}
string& CRJStringByte::ToStdString(string& str) {
	str.resize(m_sLength);
	return str.assign((const char*) GetHeadForRead(), m_sLength);
}
bool CRJStringByte::GetC(uint8_t* p, size_t s) const {
	if ((s<=m_sLength)||
		(!p)) return false;
	_MEMMOVE_BYTE_POINTER(p, GetHeadForRead(), m_sLength);
	p[m_sLength]=0x00;
	return true;
}
bool CRJStringByte::Like(const CRJStringByte& clTemplate) const {
	return CRJStringByte::Like(clTemplate.GetHeadForRead(), clTemplate.GetLength(), GetHeadForRead(), m_sLength);
}
bool CRJStringByte::Like(const uint8_t* pTemplate, size_t lTemplate,const uint8_t* pWork, size_t lWork) {
	try {
		const uint8_t* q;
		uint8_t ch;
		size_t l, lShift=0;
		bool bWildcard=false;
		while ((lTemplate>0)&&
			(lWork>0)) {
			ch=pTemplate[0];
			switch (ch) {
			case '*':bWildcard=true;
				pTemplate++;
				lTemplate--;
				break;
			case '?':lShift++;
				pTemplate++;
				lTemplate--;
				pWork++;
				lWork--;
				break;
			default:
				if (bWildcard) {
					q=(const uint8_t*) _MEMFIND_BYTE_POINTER(pWork, ch, lWork);
					while (q) {
						l=lWork-(size_t)(q-pWork);
						if (CRJStringByte::Like(pTemplate+1, lTemplate-1, q+1, l-1)) return true;
						pWork=q+1;
						lWork=l-1;
						q=(const uint8_t*) _MEMFIND_BYTE_POINTER(pWork, ch, lWork);
					}
					return false;
				}
				else {
					if (* pTemplate!=* pWork) return false;
					pWork++;
					lWork--;
				}
				pTemplate++;
				lTemplate--;
				bWildcard=false;
				lShift=0;
			}
		}
		if (lTemplate>0) return CRJStringByte::FindTopNotThat(pTemplate, lTemplate,'*')==nullptr;
		else if (lWork>0) return bWildcard;
		else return true;
	}
	catch(...) {}
	return false;
}
void CRJStringByte::Split(uint8_t ch, IRJStringByteSplit& clFunc) const {
	CRJChainFinder clFinder;
	CRJStringByteFind str;
	size_t lLast=0;
	bool b=FindPos(ch, clFinder);
	while (b) {
		str.Slice(*this, lLast, clFinder.m_sFound);
		clFunc(str);
		str.lIndex++;
		lLast=clFinder.m_sFound+1;
		b=FindPosNext(ch, clFinder);
	}
	if (str.Slice(*this, lLast, m_sLength).IsDefined()) clFunc(str.Last());
}
bool CRJStringByte::SplitResult(uint8_t ch, IRJStringByteSplitBool& clFunc) const {
	CRJChainFinder clFinder;
	CRJStringByteFind str;
	size_t lLast=0;
	bool b=FindPos(ch, clFinder);
	while (b) {
		str.Slice(*this, lLast, clFinder.m_sFound);
		if (!clFunc(str)) return false;
		str.lIndex++;
		lLast=clFinder.m_sFound+1;
		b=FindPosNext(ch, clFinder);
	}
	if (str.Slice(*this, lLast, m_sLength).IsDefined()) return clFunc(str.Last());
	return true;
}
bool CRJStringByte::Split(uint8_t ch, CRJStringByteFlex& l, CRJStringByteFlex& r) const {
	CRJChainFinder clFinder;
	if (FindPos(ch, clFinder)) {
		l.Slice(*this, 0, clFinder.m_sFound);
		r.Slice(*this, clFinder.m_sFound+1, m_sLength);
		return true;
	}
	return false;
}
bool CRJStringByte::InBrackets(uint8_t l, uint8_t r) const {
	return m_sLength>1&&
		l==m_pHead[0]&&
		r==m_pHead[m_sLength-1];
}
bool CRJStringByte::IsFirstThat(uint8_t ch) const {
	return m_sLength>0&&
		ch==m_pHead[0];
}
bool CRJStringByte::IsFirstThat(const CRJStringByte& str) const {
	return m_sLength>0&&
		str.Contain(m_pHead[0]);
}
bool CRJStringByte::IsLastThat(uint8_t ch) const {
	return m_sLength>0&&
		ch==m_pHead[m_sLength-1];
}
bool CRJStringByte::IsLastThat(const CRJStringByte& str) const {
	return m_sLength>0&&
		str.Contain(m_pHead[m_sLength-1]);
}
CRJStringUnicodeVar& CRJStringByte::ToUnicode(CRJStringUnicodeVar& w) const {
	return CRJEncoding::translateUtf8ToUnicode(*this, w);
}
CRJStringUnicodeVar& CRJStringByte::ToUnicode(size_t cp, CRJStringUnicodeVar& w) const {
	return CRJEncoding::translateCodePageToUnicode(*this, w, cp);
}
CRJStringByteVar& CRJStringByte::ToUtf8(size_t cp, CRJStringByteVar& v) const {
	return CRJEncoding::translateCodePageToUtf8(*this, v, cp);
}
CRJStringByteVar& CRJStringByte::ToCodePage(size_t cp, CRJStringByteVar& v) const {
	return CRJEncoding::translateUtf8ToCodePage(*this, v, cp);
}
bool CRJStringByte::AllSpaces() const {
	for (size_t i=0; i<m_sLength; i++)
	if (!::isspace(m_pHead[i])) return false;
	return true;
}
size_t CRJStringByte::ToBlockBytePacked(uint8_t* pData, size_t sLen) const {
	try {
		size_t l=GetLength()>>1;
		if (l>0) {
			if (l>sLen) l=sLen;
			CRJBytePacker::PackMachine(pData, GetHeadForRead(), l);
			if (l<sLen)::memset(& pData[l], 0x00, sLen-l);
			return l;
		}
	}
	catch(...) {}
	return 0;
}
CRJStringByteData::CRJStringByteData(const uint8_t* p, size_t lLen) {
	SetHeadAndLength(p, lLen);
}

