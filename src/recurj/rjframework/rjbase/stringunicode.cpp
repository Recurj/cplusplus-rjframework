#include "stdafx.h"
#include <wchar.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/strings/unicode/string.h>
#include <recurj/rjframework/libs/base/strings/unicode/string.h>
#include <recurj/rjframework/libs/base/strings/unicode/var.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
using namespace rjBase;
CRJStringUnicode CRJStringUnicode::s_spaceGroup(L"\u0020\u00A0\u0009\u1680\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200A\u202F\u205F\u3000");
CRJStringUnicode CRJStringUnicode::s_lineBreakGroup(L"\u000A\u0085\u2028\u2029");
CRJStringUnicode CRJStringUnicode::s_controlGroup(L"\u0000\u0001\u0002\u0003\u0004\u0005\u0006\u0007"
L"\u0008\u0009\u000A\u000B\u000C\u000D\u000E\u000F"
L"\u0010\u0011\u0012\u0013\u0014\u0015\u0016\u0017"
L"\u0018\u0019\u001A\u001B\u001C\u001D\u001E\u001F"
L"\u007F\u0080\u0081\u0082\u0083\u0084\u0085\u0086"
L"\u0087\u0088\u0089\u008A\u008B\u008C\u008D\u008E"
L"\u008F\u0090\u0091\u0092\u0093\u0094\u0095\u0096"
L"\u0097\u0098\u0099\u009A\u009B\u009C\u009D\u009E"
L"\u009F");
CRJStringUnicode CRJStringUnicode::s_forwardSlash(L"\u002F\u01C0\u0338\u2044\u2215");
CRJStringUnicode CRJStringUnicode::s_backSlash(L"\u005C\u20E5\u2216");
CRJStringUnicode CRJStringUnicode::s_asterisk(L"\u002A\u066D\u204E\u2217\u26B9\u2731");
CRJStringUnicode CRJStringUnicode::s_apostrophe(L"\u0027\u2019\u02B9\u02BC\u02C8\u0301\u05F3\u2032\uA78C");
CRJStringUnicode CRJStringUnicode::s_questionMark(L"\u0022\u02BA\u030B\u030E\u05F4\u2033\u3003");
CRJStringUnicode CRJStringUnicode::s_WordYES(L"YES", 3);
CRJStringUnicode CRJStringUnicode::s_WordTRUE(L"TRUE", 4);
CRJStringUnicode CRJStringUnicode::s_WordNO(L"NO", 2);
CRJStringUnicode CRJStringUnicode::s_WordFALSE(L"FALSE", 5);
CRJStringUnicode CRJStringUnicode::s_Word1(L"1", 1);
CRJStringUnicode CRJStringUnicode::s_Word0(L"0", 1);
CRJStringUnicode CRJStringUnicode::s_WordN(L"N", 1);
CRJStringUnicode CRJStringUnicode::s_WordY(L"Y", 1);
#ifdef _rjOS_FAMILY_UNIX
CRJStringUnicode CRJStringUnicode::s_LineNext(L"\x0A", 1);
#endif 
#ifdef _rjOS_WIN
CRJStringUnicode CRJStringUnicode::s_LineNext(L"\x0A", 1);
CRJStringUnicode CRJStringUnicode::s_LineFeed(L"\x0D\x0A", 2);
#endif 
CRJStringUnicode CRJStringUnicode::s_CRLF(L"\x0D\x0A", 2);
const wchar_t CRJStringUnicode::s_uUnDef[1]= {
	0x00
};
const wchar_t CRJStringUnicode::s_uEmpty[1]= {
	0x00
};
const wchar_t* CRJStringUnicode::FindTopThat(const wchar_t* p, size_t lLen,wchar_t ch) {
	for (size_t i=0; i<lLen; i++)
	if (p[i]==ch) return p+i;
	return nullptr;
}
const wchar_t* CRJStringUnicode::FindTopNotThat(const wchar_t* p, size_t lLen,wchar_t ch) {
	for (size_t i=0; i<lLen; i++)
	if (p[i]!=ch) return p+i;
	return nullptr;
}
CRJStringUnicode::CRJStringUnicode() {
	m_pHead=CRJStringUnicode::s_uUnDef;
}
CRJStringUnicode::CRJStringUnicode(const wchar_t* p, size_t lLen) {
	if (p) {
		if (!lLen) lLen=wcslen(p);
		SetHeadAndLength(p, lLen);
	}
	else SetConstantUndefined();
}
CRJStringUnicode::~CRJStringUnicode() {}
//	               CRJStringUnicode& CRJStringUnicode::Update(const CRJStringUnicode& s)
//	               {
//	                if (this != &s) SetHeadAndLength(s.GetHeadForRead(), s.GetLength());
//	                return *this;
//	               }
CRJStringUnicode& CRJStringUnicode::GetAt(size_t lPos,wchar_t& ch) {
	if ((m_sLength>0)&&
		(lPos<m_sLength)) ch=GetHeadForRead()[lPos];
	else ch=0x00;
	return *this;
}
bool CRJStringUnicode::FindPos(wchar_t ch, CRJChainFinder& clFound) const {
	return FindPosSame(GetHeadForRead(), m_sLength, ch, 0, clFound);
}
bool CRJStringUnicode::FindPosNext(wchar_t ch, CRJChainFinder& clFound) const {
	size_t lLast=clFound.m_sFound+1;
	return (m_sLength>lLast)?
		FindPosSame(GetHeadForRead()+lLast, m_sLength-lLast, ch, lLast, clFound):
		clFound.NotFoundPos();
}
bool CRJStringUnicode::FindPosBack(wchar_t ch, CRJChainFinder& clFound) const {
	return FindPosSameBack(GetTail(), m_sLength, ch, clFound);
}
bool CRJStringUnicode::FindPosNextBack(wchar_t ch, CRJChainFinder& clFound) const {
	return ((clFound.m_sFound>0)&&
		(clFound.m_sFound<m_sLength))?
		FindPosSameBack(GetHeadForRead()+clFound.m_sFound, clFound.m_sFound, ch, clFound):
		clFound.NotFoundPos();
}
bool CRJStringUnicode::FindPosNotSameLeft(wchar_t ch, CRJChainFinder& clFound) const {
	return FindPosNotSame(GetHeadForRead(), m_sLength, ch, 0, clFound);
}
bool CRJStringUnicode::FindPosNotSameLeftNext(wchar_t ch, CRJChainFinder& clFound) const {
	size_t lLast=clFound.m_sFound+1;
	return (m_sLength>lLast)?
		FindPosNotSame(GetHeadForRead()+lLast, m_sLength-lLast, ch, lLast, clFound):
		clFound.NotFoundPos();
}
bool CRJStringUnicode::FindPosNotSameRight(wchar_t ch, CRJChainFinder& clFound) const {
	return FindPosNotSameBack(GetTail(), m_sLength, ch, clFound);
}
bool CRJStringUnicode::FindPosNotSameRightNext(wchar_t ch, CRJChainFinder& clFound) const {
	return ((clFound.m_sFound>0)&&
		(clFound.m_sFound<m_sLength))?
		FindPosNotSameBack(GetHeadForRead()+clFound.m_sFound, clFound.m_sFound, ch, clFound):
		clFound.NotFoundPos();
}
bool CRJStringUnicode::FindPosInSet(const CRJStringUnicode& clMask, CRJChainFinderSet& clFound) const {
	CRJChainFinder st;
	const wchar_t* pData=GetHeadForRead();
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
bool CRJStringUnicode::FindPosInSetNext(const CRJStringUnicode& clMask, CRJChainFinderSet& clFound) const {
	size_t lLast=clFound.m_sFound+1;
	if (m_sLength>lLast) {
		CRJChainFinder st;
		size_t i, f=0, c=clMask.GetLength(),
		s=m_sLength-lLast;
		const wchar_t* pData=GetHeadForRead()+lLast;
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
bool CRJStringUnicode::FindPosInSetBack(const CRJStringUnicode& clMask, CRJChainFinderSet& clFound) const {
	CRJChainFinder st;
	size_t i, f=0, c=clMask.GetLength(),
	s=m_sLength;
	const wchar_t* pData=GetHeadForRead();
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
bool CRJStringUnicode::FindPosInSetNextBack(const CRJStringUnicode& clMask, CRJChainFinderSet& clFound) const {
	if ((clFound.m_sFound>0)&&
		(m_sLength>clFound.m_sFound)) {
		CRJChainFinder st;
		size_t i, f=0, c=clMask.GetLength(),
		s=clFound.m_sFound;
		const wchar_t* pData=GetHeadForRead();
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
bool CRJStringUnicode::FindSubStr(const CRJStringUnicode& clSubStr, CRJChainFinder& clFound, size_t off) const {
	size_t sLen=clSubStr.GetLength();
	if ((m_sLength>off)&&
		(sLen<=(m_sLength-off))) {
		CRJChainFinder st;
		size_t sFirst=off, sLast=m_sLength-sLen+1;
		const wchar_t* pData=GetHeadForRead();
		const wchar_t* pSubStr=clSubStr.GetHeadForRead();
		wchar_t chFirst=pSubStr[0];
		while (FindPosSame(pData+sFirst, sLast-sFirst, chFirst, sFirst, st)) {
			if (!_MEMCOMPARE_UNICODE_POINTER(& pData[st.m_sFound], pSubStr, sLen)) return clFound.FoundPos(st.m_sFound);
			sFirst=st.m_sFound+1;
			if (sFirst>=sLast)break;
		}
	}
	return clFound.NotFoundPos();
}
bool CRJStringUnicode::FindPosSame(const wchar_t* p, size_t lLen,wchar_t ch, size_t lOffset, CRJChainFinder& clFound) {
	for (size_t i=0; i<lLen; i++)
	if (ch==p[i]) return clFound.FoundPos(i+lOffset);
	return clFound.NotFoundPos();
}
bool CRJStringUnicode::FindPosNotSame(const wchar_t* p, size_t lLen,wchar_t ch, size_t lOffset, CRJChainFinder& clFound) {
	for (size_t i=0; i<lLen; i++)
	if (p[i]!=ch) return clFound.FoundPos(i+lOffset);
	return clFound.NotFoundPos();
}
bool CRJStringUnicode::FindPosSameBack(const wchar_t* p, size_t lLen,wchar_t ch, CRJChainFinder& clFound) {
	while (lLen>0) {
		lLen--;
		p--;
		if (* p==ch) return clFound.FoundPos(lLen);
	}
	return clFound.NotFoundPos();
}
bool CRJStringUnicode::FindPosNotSameBack(const wchar_t* p, size_t lLen,wchar_t ch, CRJChainFinder& clFound) {
	while (lLen>0) {
		lLen--;
		p--;
		if (* p!=ch) return clFound.FoundPos(lLen);
	}
	return clFound.NotFoundPos();
}
bool CRJStringUnicode::FindClosingBracket(wchar_t chOpen,wchar_t chClose, size_t& lPos) const {
	wchar_t ch;
	const wchar_t* pData=GetHeadForRead();
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
int64_t CRJStringUnicode::Compare(const CRJStringUnicode& str) const {
	size_t lLen=str.GetLength();
	int64_t rc=rjcompare(m_sLength, lLen);
	if (!rc) rc=_MEMCOMPARE_UNICODE_POINTER(GetHeadForRead(), str.GetHeadForRead(), lLen);
	else if (rc>0) {
		if (lLen>0) {
			int64_t lLocal=_MEMCOMPARE_UNICODE_POINTER(GetHeadForRead(), str.GetHeadForRead(), lLen);
			if (lLocal) rc=lLocal;
		}
	}
	else if (m_sLength>0) {
		int64_t lLocal=_MEMCOMPARE_UNICODE_POINTER(GetHeadForRead(), str.GetHeadForRead(), m_sLength);
		if (lLocal) rc=lLocal;
	}
	return rc;
}
int64_t CRJStringUnicode::Compare(const wchar_t* pBuf, size_t lLen) const {
	CRJStringUnicode s(pBuf, lLen);
	return Compare(s);
}
bool CRJStringUnicode::Contain(wchar_t ch) const {
	return _MEMFIND_UNICODE_POINTER(GetHeadForRead(), ch, m_sLength)!=nullptr;
}
bool CRJStringUnicode::IsGroupSpace(wchar_t w) {
	return CRJStringUnicode::s_spaceGroup.Contain(w);
}
bool CRJStringUnicode::IsGroupLineBreak(wchar_t w) {
	return CRJStringUnicode::s_lineBreakGroup.Contain(w);
}
bool CRJStringUnicode::IsGroupControl(wchar_t w) {
	return CRJStringUnicode::s_controlGroup.Contain(w);
}
bool CRJStringUnicode::GetC(wchar_t* p, size_t s) const {
	if ((s<=m_sLength)||
		(!p)) return false;
	_MEMMOVE_UNICODE_POINTER(p, GetHeadForRead(), m_sLength);
	p[m_sLength]=0;
	return true;
}
bool CRJStringUnicode::Like(const CRJStringUnicode& clTemplate) const {
	return CRJStringUnicode::Like(clTemplate.GetHeadForRead(), clTemplate.GetLength(), GetHeadForRead(), m_sLength);
}
bool CRJStringUnicode::Like(const wchar_t* pTemplate, size_t lTemplate,const wchar_t* pWork, size_t lWork) {
	try {
		const wchar_t* q;
		wchar_t ch;
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
					q=_MEMFIND_UNICODE_POINTER(pWork, ch, lWork);
					while (q) {
						l=lWork-(size_t)(q-pWork);
						if (CRJStringUnicode::Like(pTemplate+1, lTemplate-1, q+1, l-1)) return true;
						pWork=q+1;
						lWork=l-1;
						q=_MEMFIND_UNICODE_POINTER(pWork, ch, lWork);
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
		if (lTemplate>0) return CRJStringUnicode::FindTopNotThat(pTemplate, lTemplate,L'*')==nullptr;
		else if (lWork>0) return bWildcard;
		else return true;
	}
	catch(...) {}
	return false;
}
bool CRJStringUnicode::InBrackets(wchar_t l,wchar_t r) const {
	return m_sLength>1&&
		l==m_pHead[0]&&
		r==m_pHead[m_sLength-1];
}
bool CRJStringUnicode::IsFirstThat(wchar_t ch) const {
	return m_sLength>0&&
		ch==m_pHead[0];
}
bool CRJStringUnicode::IsFirstThat(const CRJStringUnicode& str) const {
	return m_sLength>0&&
		str.Contain(m_pHead[0]);
}
bool CRJStringUnicode::IsLastThat(wchar_t ch) const {
	return m_sLength>0&&
		ch==m_pHead[m_sLength-1];
}
void CRJStringUnicode::Split(wchar_t ch, IRJStringUnicodeSplit& clFunc) const {
	CRJChainFinder clFinder;
	CRJStringUnicodeFind str;
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
bool CRJStringUnicode::SplitResult(wchar_t ch, IRJStringUnicodeSplitBool& clFunc) const {
	CRJChainFinder clFinder;
	CRJStringUnicodeFind str;
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
bool CRJStringUnicode::Split(wchar_t ch, CRJStringUnicodeFlex& l, CRJStringUnicodeFlex& r) const {
	CRJChainFinder clFinder;
	if (FindPos(ch, clFinder)) {
		l.Slice(*this, 0, clFinder.m_sFound);
		r.Slice(*this, clFinder.m_sFound+1, m_sLength);
		return true;
	}
	return false;
}
bool CRJStringUnicode::IsValidInt64(int64_t& v) const {
	CRJStringByteDefault s;
	CRJEncoding::translateUnicodeToUtf8(*this, s);
	return s.OK()&&
		s.IsValidInt64(v);
}
bool CRJStringUnicode::IsValidSize(size_t& v) const {
	CRJStringByteDefault s;
	CRJEncoding::translateUnicodeToUtf8(*this, s);
	return s.OK()&&
		s.IsValidSize(v);
}
bool CRJStringUnicode::IsValidDouble(double& v) const {
	CRJStringByteDefault s;
	CRJEncoding::translateUnicodeToUtf8(*this, s);
	return s.OK()&&
		s.IsValidDouble(v);
}
bool CRJStringUnicode::IsValidBool(bool& b) const {
	CRJStringByteDefault s;
	CRJEncoding::translateUnicodeToUtf8(*this, s);
	return s.OK()&&
		s.IsValidBool(b);
}
size_t CRJStringUnicode::ToSizeDef(size_t v) const {
	CRJStringByteDefault s;
	CRJEncoding::translateUnicodeToUtf8(*this, s);
	return s.OK()?
		s.ToSizeDef(v):
		v;
}
int64_t CRJStringUnicode::ToInt64Def(int64_t v) const {
	CRJStringByteDefault s;
	CRJEncoding::translateUnicodeToUtf8(*this, s);
	return s.OK()?
		s.ToInt64Def(v):
		v;
}
double CRJStringUnicode::ToDoubleDef(double d) const {
	CRJStringByteDefault s;
	CRJEncoding::translateUnicodeToUtf8(*this, s);
	return s.OK()?
		s.ToDoubleDef(d):
		d;
}
bool CRJStringUnicode::ToBoolDef(bool b) const {
	CRJStringByteDefault s;
	CRJEncoding::translateUnicodeToUtf8(*this, s);
	return s.OK()?
		s.ToBoolDef(b):
		b;
}
wstring& CRJStringUnicode::ToStdString(wstring& str) {
	str.resize(m_sLength);
	return str.assign(GetHeadForRead(), m_sLength);
}
CRJStringByteVar& CRJStringUnicode::ToUtf8(CRJStringByteVar& w) const {
	return CRJEncoding::translateUnicodeToUtf8(*this, w);
}
CRJStringByteVar& CRJStringUnicode::ToCodePage(size_t cp, CRJStringByteVar& v) const {
	return CRJEncoding::translateUnicodeToCodePage(*this, v, cp);
}
bool CRJStringUnicode::AllSpaces() const {
	for (size_t i=0; i<m_sLength; i++)
	if (!::iswspace(m_pHead[i])) return false;
	return true;
}

