#pragma once
#include <recurj/rjframework/libs/base/collections//chains/var.h>
 template<typename TData>
class CRJChainFlex:public CRJChainConstant<TData> {
public:
	CRJChainFlex() {}
	CRJChainFlex(const TData* p, size_t s):
		CRJChainConstant(p, s) {}
	CRJChainFlex(const CRJChainConstant& str) {
		Slice(str, 0);
	}
	CRJChainFlex(const CRJChainFlex& str) {
		Slice(str, 0);
	}
	CRJChainFlex(const CRJChainConstant& str, size_t lFrom, size_t lTo) {
		Slice(str, lFrom, lTo);
	}
	CRJChainFlex& Set(const CRJChainConstant& str) {
		SetHeadAndLength(str.GetHeadForRead(), str.GetLength());
		return *this;
	}
	CRJChainFlex& SetFromData(const TData* p, size_t s) {
		if (!p) return SetFlexUndefined();
		SetHeadAndLength(p, s);
		return *this;
	}
	CRJChainFlex& Slice(const CRJChainConstant& str, size_t lFrom, size_t lTo) {
		size_t s=str.GetLength();
		if ((lFrom<s)&&
			(lTo<=s)&&
			(lFrom<=lTo)) {
			SetHeadAndLength(str.GetHeadForRead()+lFrom, lTo-lFrom);
			return *this;
		}
		return SetFlexUndefined();
	}
	CRJChainFlex& Slice(const CRJChainConstant& str, size_t lFrom) {
		size_t s=str.GetLength();
		if (lFrom<s) {
			SetHeadAndLength(str.GetHeadForRead()+lFrom, s-lFrom);
			return *this;
		}
		return SetFlexUndefined();
	}
	CRJChainFlex& TrimLeft(const TData& ch) {
		CRJChainFinder st;
		if (!FindPosNotSameLeft(ch, st)) return SetEmpty();
		if (st.m_sFound>0) {
			m_pHead+=st.m_sFound;
			return SetLengthSafed(m_sLength-st.m_sFound);
		}
		return *this;
	}
	CRJChainFlex& TrimRight(const TData& ch) {
		CRJChainFinder st;
		if (!FindPosNotSameRight(ch, st)) return SetEmpty();
		return SetLengthSafed(st.m_sFound+1);
	}
	CRJChainFlex& Trim(const TData& ch) {
		return TrimRight(ch).TrimLeft(ch);
	}
	CRJChainFlex& TrimLeft(const CRJChainConstant& s) {
		size_t r=0;
		while (r<m_sLength) {
			if (s.Contain(operator[](r))) r++;
			else break;
		}
		return TruncLeft(r);
	}
	CRJChainFlex& TrimRight(const CRJChainConstant& s) {
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
	CRJChainFlex& Trim(const CRJChainConstant& s) {
		return TrimRight(s).TrimLeft(s);
	}
	CRJChainFlex& TruncLeft(size_t lOffset) {
		if (lOffset<=m_sLength) {
			m_pHead+=lOffset;
			return SetLengthSafed(m_sLength-lOffset);
		}
		else if (lOffset==m_sLength) return SetEmpty();
		return SetFlexUndefined();
	}
	CRJChainFlex& TruncRight(size_t s) {
		if (m_sLength>=s) return SetLengthSafed(m_sLength-s);
		return SetFlexUndefined();
	}
	CRJChainFlex& SetEmpty() {
		m_pHead=CRJChainFlex::s_uEmpty;
		return SetLengthSafed(0);
	}
	CRJChainFlex& SetLengthSafed(size_t s) {
		m_sLength=s;
		return *this;
	}
	bool popFirst(TData& clElem) {
		if (m_sLength>0) {
			clElem=m_pHead[0];
			m_pHead++;
			SetLengthSafed(m_sLength-1);
			return true;
		}
		return false;
	}
	bool popLast(TData& clElem) {
		if (m_sLength>0) {
			SetLengthSafed(m_sLength-1);
			clElem=m_pHead[m_sLength];
			return true;
		}
		return false;
	}
	bool WasFoundTopAndMovedHead2Var(const TData& ch, CRJChainVar<TData>& clSplit) {
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
	bool WasFoundTopAndMovedTail2Var(const TData& ch, CRJChainVar<TData>& clSplit) {
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
	bool WasFoundTopAndMovedHead(const TData& ch, CRJChainFlex& clSplit) {
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
	bool WasFoundTopAndMovedTail(const TData& ch, CRJChainFlex& clSplit) {
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
	bool WasFoundBackAndMovedTail2Var(const TData& ch, CRJChainVar<TData>& clSplit) {
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
	bool WasFoundBackAndMovedHead2Var(const TData& ch, CRJChainVar<TData>& clSplit) {
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
	bool WasFoundBackAndMovedTail(const TData& ch, CRJChainFlex& clSplit) {
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
	bool WasFoundBackAndMovedHead(const TData& ch, CRJChainFlex& clSplit) {
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
	bool HasField(const TData& sep, CRJChainFlex& elem) {
		if (IsEmpty()) return false;
		if (WasFoundTopAndMovedHead(sep, elem)) return true;
		elem.Set(*this);
		SetEmpty();
		return true;
	}
	CRJChainFlex& SetFlexUndefined() {
		SetConstantUndefined();
		return *this;
	}
	static const TData s_uEmpty[1];
};
template<typename TData>const TData CRJChainFlex<TData>::s_uEmpty[1];
template<typename TData>
class CRJChainFind:public CRJChainFlex<TData> {
public:
	using CRJChainFlex::CRJChainFlex;
	CRJChainFind& Last() {
		bLast=true;
		return *this;
	}
	size_t lIndex=0;
	bool bLast=false;
};

