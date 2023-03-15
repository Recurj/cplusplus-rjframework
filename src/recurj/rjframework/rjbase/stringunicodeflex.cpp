#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/unicode/flex.h>
#include <recurj/rjframework/libs/base/strings/unicode/var.h>
using namespace rjBase;
CRJStringUnicodeFlex::CRJStringUnicodeFlex():
	CRJStringUnicode(CRJStringUnicode::s_uUnDef, 0) {}
CRJStringUnicodeFlex::CRJStringUnicodeFlex(const wchar_t* p, size_t s):
	CRJStringUnicode(CRJStringUnicode::s_uUnDef, 0) {
	SetFromData(p, s);
}
CRJStringUnicodeFlex::CRJStringUnicodeFlex(const CRJStringUnicode& str) {
	SetHeadAndLength(str.GetHeadForRead(), str.GetLength());
}
CRJStringUnicodeFlex::CRJStringUnicodeFlex(const CRJStringUnicodeFlex& str) {
	SetHeadAndLength(str.GetHeadForRead(), str.GetLength());
}
CRJStringUnicodeFlex::CRJStringUnicodeFlex(const CRJStringUnicode& str, size_t lFrom, size_t lTo):
	CRJStringUnicode((const wchar_t*) CRJStringUnicode::s_uUnDef, 0) {
	Slice(str, lFrom, lTo);
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::operator=(const CRJStringUnicodeFlex& s) {
	if (this!=& s) return Set(s);
	return *this;
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::SetLengthSafed(size_t s) {
	m_sLength=s;
	return *this;
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::Set(const CRJStringUnicode& str) {
	SetHead(str.GetHeadForRead());
	return SetLengthSafed(str.GetLength());
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::SetFromLabel(const wchar_t* p) {
	if (!p) return SetFlexUndefined();
	SetHead(p);
	return SetLengthSafed(::wcslen(p));
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::SetFromData(const wchar_t* p, size_t s) {
	if (!p) return SetFlexUndefined();
	SetHead(p);
	return SetLengthSafed(s);
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::Slice(const CRJStringUnicode& str, size_t lFrom, size_t lTo) {
	size_t s=str.GetLength();
	if ((lFrom<s)&&
		(lTo<=s)&&
		(lFrom<=lTo)) {
		SetHead(str.GetHeadForRead()+lFrom);
		return SetLengthSafed(lTo-lFrom);
	}
	return SetFlexUndefined();
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::Slice(const CRJStringUnicode& str, size_t lFrom) {
	size_t s=str.GetLength();
	if (lFrom<s) {
		SetHead(str.GetHeadForRead()+lFrom);
		return SetLengthSafed(s-lFrom);
	}
	return SetFlexUndefined();
}
bool CRJStringUnicodeFlex::IsFlexUndefined() const {
	return GetHeadForRead()==CRJStringUnicode::s_uUnDef;
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::SetEmpty() {
	SetHead(CRJStringUnicode::s_uUnDef);
	return (CRJStringUnicodeFlex&) SetLengthSafed(0);
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::TrimLeft(wchar_t ch) {
	CRJChainFinder st;
	if (!FindPosNotSameLeft(ch, st)) return SetEmpty();
	if (st.m_sFound>0) {
		m_pHead+=st.m_sFound;
		return SetLengthSafed(m_sLength-st.m_sFound);
	}
	return *this;
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::TrimRight(wchar_t ch) {
	CRJChainFinder st;
	if (!FindPosNotSameRight(ch, st)) return SetEmpty();
	return SetLengthSafed(st.m_sFound+1);
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::Trim(wchar_t ch) {
	return TrimRight(ch).TrimLeft(ch);
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::TrimLeft(const CRJStringUnicode& s) {
	size_t r=0;
	while (r<m_sLength) {
		if (s.Contain(operator[](r))) r++;
		else break;
	}
	return TruncLeft(r);
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::TrimRight(const CRJStringUnicode& s) {
	size_t r=m_sLength;
	while (r>0) {
		r--;
		if (s.Contain(operator[](r))) {
			m_sLength--;
		}
		else break;
	}
	return *this;
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::Trim(const CRJStringUnicode& s) {
	return TrimRight(s).TrimLeft(s);
}
bool CRJStringUnicodeFlex::WasFoundTopAndMovedHead(wchar_t ch, CRJStringUnicodeFlex& clSplit) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			clSplit.SetFromData(GetHeadForRead(), st.m_sFound);
			st.m_sFound++;
			SetFromData(GetHeadForRead()+st.m_sFound, m_sLength-st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringUnicodeFlex::WasFoundTopAndMovedTail(wchar_t ch, CRJStringUnicodeFlex& clSplit) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			size_t l=st.m_sFound+1;
			clSplit.SetFromData(GetHeadForRead()+l, m_sLength-l);
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringUnicodeFlex::WasFoundTopAndMovedTail(const CRJStringUnicode& clMask, CRJStringUnicodeFlex& clSplit, CRJChainFinderSet& st) {
	if (m_sLength>0) {
		if (FindPosInSet(clMask, st)) {
			size_t l=st.m_sFound+1;
			clSplit.SetFromData(GetHeadForRead()+l, m_sLength-l);
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringUnicodeFlex::WasFoundTopAndMovedHead2Var(wchar_t ch, CRJStringUnicodeVar& clSplit) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			clSplit.SetData(GetHeadForRead(), st.m_sFound);
			st.m_sFound++;
			SetFromData(GetHeadForRead()+st.m_sFound, m_sLength-st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringUnicodeFlex::WasFoundTopAndMovedTail2Var(wchar_t ch, CRJStringUnicodeVar& clSplit) {
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
bool CRJStringUnicodeFlex::WasFoundBackAndMovedTail2Var(wchar_t ch, CRJStringUnicodeVar& clSplit) {
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
bool CRJStringUnicodeFlex::WasFoundBackAndMovedHead2Var(wchar_t ch, CRJStringUnicodeVar& clSplit) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			clSplit.SetData(GetHeadForRead(), st.m_sFound);
			st.m_sFound++;
			SetFromData(GetHeadForRead()+st.m_sFound, m_sLength-st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringUnicodeFlex::WasFoundBackAndMovedTail(wchar_t ch, CRJStringUnicodeFlex& clSplit) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			size_t l=st.m_sFound+1;
			clSplit.SetFromData(GetHeadForRead()+l, m_sLength-l);
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringUnicodeFlex::WasFoundBackAndMovedHead(wchar_t ch, CRJStringUnicodeFlex& clSplit) {
	if (m_sLength>0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			clSplit.SetFromData(GetHeadForRead(), st.m_sFound);
			st.m_sFound++;
			SetFromData(GetHeadForRead()+st.m_sFound, m_sLength-st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::TruncLeft(size_t lOffset) {
	if (lOffset<=m_sLength) {
		m_pHead+=lOffset;
		return SetLengthSafed(m_sLength-lOffset);
	}
	else if (lOffset==m_sLength) return SetEmpty();
	return SetFlexUndefined();
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::TruncRight(size_t s) {
	if (m_sLength>=s) return SetLengthSafed(m_sLength-s);
	return SetFlexUndefined();
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::Grow(size_t l) {
	m_sLength+=l;
	return *this;
}
CRJStringUnicodeFlex& CRJStringUnicodeFlex::FindTopAndMoveHead(wchar_t ch, CRJStringUnicodeFlex& clSplit) {
	if (!WasFoundTopAndMovedHead(ch, clSplit)) {
		clSplit.Set(*this);
		SetEmpty();
	}
	return clSplit;
}

