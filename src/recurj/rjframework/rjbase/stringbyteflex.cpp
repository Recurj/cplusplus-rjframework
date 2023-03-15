#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
using namespace rjBase;
CRJStringByteFlex::CRJStringByteFlex()noexcept {}
CRJStringByteFlex::CRJStringByteFlex(const CRJStringByte& str) {
	SetHeadAndLength(str.GetHeadForRead(), str.GetLength());
}
CRJStringByteFlex::CRJStringByteFlex(const CRJStringByteFlex& str) {
	SetHeadAndLength(str.GetHeadForRead(), str.GetLength());
}
CRJStringByteFlex::CRJStringByteFlex(const uint8_t* p, size_t s) {
	SetFromData(p, s);
}
CRJStringByteFlex::CRJStringByteFlex(const CRJStringByte& str, size_t lFrom, size_t lTo) {
	Slice(str, lFrom, lTo);
}
CRJStringByteFlex& CRJStringByteFlex::operator=(const CRJStringByte& s) {
	if (this != &s) return Set(s);
	return *this;
}
CRJStringByteFlex& CRJStringByteFlex::Set(const CRJStringByte& str) {
	SetHead(str.GetHeadForRead());
	return SetLengthSafed(str.GetLength());
}
CRJStringByteFlex& CRJStringByteFlex::SetFromDataAsLabel(uint8_t* p, size_t lVol) {
	p[lVol - 1] = 0x00;
	return SetFromData(p, strlen((char*)p));
}
CRJStringByteFlex& CRJStringByteFlex::SetFromDataAsLabelWithLength(const uint8_t* p, size_t sVol, size_t sLen) {
	if (sLen > sVol) sLen = sVol;
	return SetFromData(p, sLen);
}
CRJStringByteFlex& CRJStringByteFlex::SetFromData(const uint8_t* p, size_t s) {
	if (!p) return SetFlexUndefined();
	SetHead(p);
	return SetLengthSafed(s);
}
CRJStringByteFlex& CRJStringByteFlex::Slice(const CRJStringByte& str, size_t lFrom, size_t lTo) {
	size_t s = str.GetLength();
	if ((lFrom < s) &&
		(lTo <= s) &&
		(lFrom <= lTo)) {
		SetHead(str.GetHeadForRead() + lFrom);
		return SetLengthSafed(lTo - lFrom);
	}
	return SetEmpty();
}
CRJStringByteFlex& CRJStringByteFlex::SliceSafed(const CRJStringByte& str, size_t lFrom, size_t lTo) {
	SetHead(str.GetHeadForRead() + lFrom);
	return SetLengthSafed(lTo - lFrom);
}
CRJStringByteFlex& CRJStringByteFlex::Slice(const CRJStringByte& str, size_t lFrom) {
	size_t s = str.GetLength();
	if (lFrom < s) {
		SetHead(str.GetHeadForRead() + lFrom);
		return SetLengthSafed(s - lFrom);
	}
	return SetEmpty();
}
bool CRJStringByteFlex::IsFlexUndefined() const {
	return GetHeadForRead() == CRJStringByte::s_uUnDef;
}
CRJStringByteFlex& CRJStringByteFlex::SetLengthSafed(size_t s) {
	m_sLength = s;
	return *this;
}
CRJStringByteFlex& CRJStringByteFlex::SetEmpty() {
	SetHead(CRJStringByte::s_uUnDef);
	return SetLengthSafed(0);
}
CRJStringByteFlex& CRJStringByteFlex::TrimLeft(uint8_t ch) {
	CRJChainFinder st;
	if (!FindPosNotSameLeft(ch, st)) return SetEmpty();
	if (st.m_sFound > 0) {
		m_pHead += st.m_sFound;
		return SetLengthSafed(m_sLength - st.m_sFound);
	}
	return *this;
}
CRJStringByteFlex& CRJStringByteFlex::TrimRight(uint8_t ch) {
	CRJChainFinder st;
	if (!FindPosNotSameRight(ch, st)) return SetEmpty();
	return SetLengthSafed(st.m_sFound + 1);
}
CRJStringByteFlex& CRJStringByteFlex::Trim(uint8_t ch) {
	return TrimRight(ch).TrimLeft(ch);
}
CRJStringByteFlex& CRJStringByteFlex::TrimLeft(const CRJStringByte& s) {
	size_t r = 0;
	while (r < m_sLength) {
		if (s.Contain(operator[](r))) r++;
		else break;
	}
	return TruncLeft(r);
}
CRJStringByteFlex& CRJStringByteFlex::TrimRight(const CRJStringByte& s) {
	size_t r = m_sLength;
	while (r > 0) {
		r--;
		if (s.Contain(operator[](r))) {
			m_sLength--;
		}
		else break;
	}
	return *this;
}
CRJStringByteFlex& CRJStringByteFlex::Trim(const CRJStringByte& s) {
	return TrimRight(s).TrimLeft(s);
}
CRJStringByteFlex& CRJStringByteFlex::Grow() {
	m_sLength++;
	return *this;
}
CRJStringByteFlex& CRJStringByteFlex::ForwardSafed(size_t s) {
	m_pHead += s;
	m_sLength -= s;
	return *this;
}
bool CRJStringByteFlex::WasFoundTopAndRemovedHead(uint8_t ch) {
	if (m_sLength > 0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			st.m_sFound++;
			SetFromData(GetHeadForRead() + st.m_sFound, m_sLength - st.m_sFound);
			return true;
		}
	}
	return false;
}
bool CRJStringByteFlex::WasFoundTopAndRemovedTail(uint8_t ch) {
	if (m_sLength > 0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	return false;
}
bool CRJStringByteFlex::WasFoundTopAndMovedHead(uint8_t ch, CRJStringByteFlex& clSplit) {
	if (m_sLength > 0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			clSplit.SetFromData(GetHeadForRead(), st.m_sFound);
			st.m_sFound++;
			SetFromData(GetHeadForRead() + st.m_sFound, m_sLength - st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringByteFlex::WasFoundTopAndMovedTail(uint8_t ch, CRJStringByteFlex& clSplit) {
	if (m_sLength > 0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			size_t l = st.m_sFound + 1;
			clSplit.SetFromData(GetHeadForRead() + l, m_sLength - l);
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringByteFlex::WasFoundTopAndMovedHead2Var(uint8_t ch, CRJStringByteVar& clSplit) {
	if (m_sLength > 0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			clSplit.SetData(GetHeadForRead(), st.m_sFound);
			st.m_sFound++;
			SetFromData(GetHeadForRead() + st.m_sFound, m_sLength - st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringByteFlex::WasFoundTopAndMovedTail2Var(uint8_t ch, CRJStringByteVar& clSplit) {
	if (m_sLength > 0) {
		CRJChainFinder st;
		if (FindPos(ch, st)) {
			size_t l = st.m_sFound + 1;
			clSplit.SetData(GetHeadForRead() + l, m_sLength - l);
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringByteFlex::WasFoundBackAndRemovedHead(uint8_t ch) {
	if (m_sLength > 0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			st.m_sFound++;
			SetFromData(GetHeadForRead() + st.m_sFound, m_sLength - st.m_sFound);
			return true;
		}
	}
	return false;
}
bool CRJStringByteFlex::WasFoundBackAndRemovedTail(uint8_t ch) {
	if (m_sLength > 0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	return false;
}
bool CRJStringByteFlex::WasFoundBackAndMovedTail2Var(uint8_t ch, CRJStringByteVar& clSplit) {
	if (m_sLength > 0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			size_t l = st.m_sFound + 1;
			clSplit.SetData(GetHeadForRead() + l, m_sLength - l);
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringByteFlex::WasFoundBackAndMovedHead2Var(uint8_t ch, CRJStringByteVar& clSplit) {
	if (m_sLength > 0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			clSplit.SetData(GetHeadForRead(), st.m_sFound);
			st.m_sFound++;
			SetFromData(GetHeadForRead() + st.m_sFound, m_sLength - st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringByteFlex::WasFoundBackAndMovedTail(uint8_t ch, CRJStringByteFlex& clSplit) {
	if (m_sLength > 0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			size_t l = st.m_sFound + 1;
			clSplit.SetFromData(GetHeadForRead() + l, m_sLength - l);
			SetLengthSafed(st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
bool CRJStringByteFlex::WasFoundBackAndMovedHead(uint8_t ch, CRJStringByteFlex& clSplit) {
	if (m_sLength > 0) {
		CRJChainFinder st;
		if (FindPosBack(ch, st)) {
			clSplit.SetFromData(GetHeadForRead(), st.m_sFound);
			st.m_sFound++;
			SetFromData(GetHeadForRead() + st.m_sFound, m_sLength - st.m_sFound);
			return true;
		}
	}
	clSplit.SetEmpty();
	return false;
}
CRJStringByteFlex& CRJStringByteFlex::FindTopAndMoveHead(uint8_t ch, CRJStringByteFlex& clSplit) {
	if (!WasFoundTopAndMovedHead(ch, clSplit)) {
		clSplit.Set(*this);
		SetEmpty();
	}
	return clSplit;
}
CRJStringByteFlex& CRJStringByteFlex::TruncLeft(size_t lOffset) {
	if (lOffset <= m_sLength) {
		m_pHead += lOffset;
		return SetLengthSafed(m_sLength - lOffset);
	}
	else if (lOffset == m_sLength) return SetEmpty();
	return SetFlexUndefined();
}
CRJStringByteFlex& CRJStringByteFlex::TruncRight(size_t s) {
	if (m_sLength >= s) return SetLengthSafed(m_sLength - s);
	return SetFlexUndefined();
}
bool CRJStringByteFlex::HasField(uint8_t sep, CRJStringByteFlex& elem) {
	if (IsEmpty()) return false;
	if (WasFoundTopAndMovedHead(sep, elem)) return true;
	elem.Set(*this);
	SetEmpty();
	return true;
}

