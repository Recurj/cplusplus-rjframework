#pragma once
#include <recurj/rjframework/libs/base/collections//chains/constant.h>
 template<typename TData>
class CRJChainVar:public CRJChainConstant<TData> {
public:
	inline size_t GetSize() const {
		return m_lSize;
	};
	inline size_t GetCapacity() const {
		return m_lSize-m_sLength;
	}
	inline bool OK() const {
		return m_clResult.OK();
	}
	inline CRJChainVar& SetVarError(int64_t e, size_t r=DataResutRegionChain) {
		m_clResult.SetError(r, e);
		return *this;
	}
	inline void ResetVarError() {
		m_clResult.ResetError();
	}
	inline bool HasVarError() const {
		return m_clResult.HasError();
	}
	virtual bool HasNeededSize(size_t s) {
		return m_lSize>=s;
	}
	TData* GetHeadForUpdate() {
		return const_cast<TData*>(GetHeadForRead());
	}
	TData&operator[](size_t lPos) {
		return GetHeadForUpdate()[lPos];
	}
	CRJChainVar& SetEmpty() {
		return SetLengthSafed(0);
	}
	CRJChainVar& SetAt(size_t s,const TData& v) {
		if (s<m_sLength) GetHeadForUpdate()[s]=v;
		else return SetChainVarError(eUChainErrors::BadPos);
		return *this;
	}
	CRJChainVar& Set(const CRJChainConstant<TData>& clStr) {
		return SetDataCheckLength(clStr.GetHeadForRead(), clStr.GetLength());
	}
	CRJChainVar& SetSingle(const TData& ch) {
		return SetDataCheckLength(& ch, 1);
	}
	CRJChainVar& SetData(const TData* p, size_t l) {
		return (p)?
			SetDataCheckLength(p, l):
			SetChainVarError(eUChainErrors::NoPointer);
	}
	CRJChainVar& SetDataCheckLength(const TData* p, size_t l) {
		if (HasNeededSize(l)) {
			if (l<=0) return SetEmpty();
			else return SetDataSafed(p, l);
		}
		return SetChainVarError(eUChainErrors::NoMemory);
	}
	CRJChainVar& SetDataSafed(const TData* p, size_t l) {
		for (size_t i=0; i<l; i++, p++)operator[](i)=* p;
		return SetLengthSafed(l);
	}
	CRJChainVar& CatSingle(const TData& ch) {
		return CatDataCheckLength(& ch, 1);
	}
	CRJChainVar& Cat(const CRJChainConstant<TData>& clStr) {
		return CatDataCheckLength(clStr.GetHeadForRead(), clStr.GetLength());
	}
	CRJChainVar& CatData(const TData* p, size_t l) {
		return (p)?
			CatDataCheckLength(p, l):
			SetChainVarError(eUChainErrors::NoPointer);
	}
	CRJChainVar& CatDataCheckLength(const TData* p, size_t l) {
		if (HasNeededSize(l+m_sLength)) return CatDataSafed(p, l);
		return SetChainVarError(eUChainErrors::NoMemory);
	}
	CRJChainVar& CatDataSafed(const TData* p, size_t l) {
		while (l>0) {
			operator[](m_sLength)=* p;
			p++;
			m_sLength++;
			l--;
		}
		return *this;
	}
	CRJChainVar& SubChain(const CRJChainConstant<TData>& str, size_t lFrom) {
		size_t lCount=str.GetLength();
		if (lFrom<lCount) return SetDataCheckLength(str.GetHeadForRead()+lFrom, lCount-lFrom);
		return SetChainVarError(eUChainErrors::BadPos);
	}
	CRJChainVar& SubChain(const CRJChainConstant<TData>& str, size_t lFrom, size_t lTo) {
		if ((lFrom<=lTo)&&
			(lTo<=str.GetLength())) return SetDataCheckLength(str.GetHeadForRead()+lFrom, lTo-lFrom);
		return SetChainVarError(eUChainErrors::BadPos);
	}
	CRJChainVar& RemoveAndTailLeft(size_t lOffset) {
		if (lOffset>=m_sLength) return SetChainVarError(eUChainErrors::BadPos);
		TData* pData=GetHeadForUpdate()+lOffset;
		m_sLength--;
		lOffset=m_sLength-lOffset;
		while (lOffset>0) {
			* pData=pData[1];
			pData++;
			lOffset--;
		}
		return *this;
	}
	CRJChainVar& Replace(const TData& chFind,const TData& chReplace) {
		if (m_sLength>0) {
			CRJChainFinder st;
			TData* pData=GetHeadForUpdate();
			bool b=FindPos(chFind, st);
			while (b) {
				pData[st.m_sFound]=chReplace;
				b=FindPosNext(chFind, st);
			}
		}
		return *this;
	}
	CRJChainVar& ReplaceSet(const CRJChainConstant<TData>& set,const TData& chReplace) {
		const TData* pData=set.GetHeadForRead();
		size_t l=set.GetLength();
		while (l>0) {
			Replace(* pData, chReplace);
			l--;
			pData++;
		}
		return *this;
	}
	CRJChainVar& DeleteChar(const TData& ch) {
		CRJChainFinder st;
		while (FindPos(ch, st)) {
			RemoveAndTailLeft(st.m_sFound);
		}
		return *this;
	}
	CRJChainVar& TrimLeft(const TData& ch) {
		CRJChainFinder st;
		if ((FindPosNotSameLeft(ch, st))&&
			(st.m_sFound>0)) return SetDataSafed(GetHeadForRead()+st.m_sFound, m_sLength-st.m_sFound);
		return *this;
	}
	CRJChainVar& TrimRight(const TData& ch) {
		CRJChainFinder st;
		if (FindPosNotSameRight(ch, st)) return SetLengthSafed(st.m_sFound+1);
		return *this;
	}
	CRJChainVar& Trim(const TData& ch) {
		return TrimRight(ch).TrimLeft(ch);
	}
	CRJChainVar& TruncLeft(size_t s) {
		if (s<=m_sLength) {
			TData* pData=GetHeadForUpdate();
			m_sLength-=s;
			for (size_t i=0; i<m_sLength; i++, pData++)* pData=pData[s];
		}
		else m_sLength=0;
		return *this;
	}
	CRJChainVar& TruncRight(size_t s) {
		if (m_sLength>=s) m_sLength-=s;
		else m_sLength=0;
		return *this;
	}
	bool WasFoundTopAndMovedTail(const TData& ch, CRJChainVar& clSplit) {
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
	bool WasFoundTopAndMovedHead(const TData& ch, CRJChainVar& clSplit) {
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
	bool WasFoundBackAndMovedTail(const TData& ch, CRJChainVar& clSplit) {
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
	bool TakeOffLast(TData& clElem) {
		if (m_sLength>0) {
			SetLengthSafed(m_sLength-1);
			clElem=GetHeadForRead()[m_sLength];
			return true;
		}
		return false;
	}
	bool WasFoundBackAndMovedHead(const TData& ch, CRJChainVar& clSplit) {
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
	CRJChainVar& SetLength(size_t s) {
		return (m_lSize>=s)?
			SetLengthSafed(s):
			SetChainVarError(eUChainErrors::BadLength);
	}
	CRJChainVar& SetLengthSafed(size_t s) {
		m_sLength=s;
		return *this;
	}
	CRJChainVar& SetChainVarError(eUChainErrors e, size_t r=DataResutRegionChain) {
		SetVarError(e, r);
		return *this;
	}
	_CLASS_COPY_AND_ASSIGN_DELETE(CRJChainVar)
protected:
	CRJChainVar(size_t s):
		m_lSize(s) {
		SetConstantUndefined();
	}
	virtual~CRJChainVar() {}
	CRJChainVar& SetVarUndefined() {
		SetConstantUndefined();
		m_lSize=0;
		return *this;
	}
	CRJResult m_clResult;
	size_t m_lSize;
};
template<typename TData>
class CRJChainVarAssigned:public CRJChainVar<TData> {
public:
	CRJChainVarAssigned() {}
	explicit CRJChainVarAssigned(const CRJChainVarAssigned& s):
		CRJChainVar(s.GetLength()) {
		SetHeadAndLength(s.GetHeadForRead(), m_lSize);
	}
	explicit CRJChainVarAssigned(TData* p, size_t lSize, size_t lLen):
		CRJChainVar(lSize) {
		SetHeadAndLength(p, lLen);
	}
	explicit CRJChainVarAssigned(TData* p, size_t lSize):
		CRJChainVar(lSize) {
		SetHead(p);
	}
	CRJChainVar& CRJChainVarAssigned::SetBufferAndData(const CRJChainConstant<TData>& s) {
		m_lSize=s.GetLength();
		SetHeadAndLength(s.GetHeadForRead(), m_lSize);
		OnAssigned();
		return *this;
	}
	CRJChainVar& CRJChainVarAssigned::SetBuffer(TData* p, size_t s) {
		m_lSize=s;
		SetHeadAndLength(p, 0);
		OnAssigned();
		return *this;
	}
protected:
	virtual void OnAssigned() {}
};
template<typename TData, size_t VOLUME>
class CRJChainVarBuffer:public CRJChainVar<TData> {
public:
	CRJChainVarBuffer():
		CRJChainVar(VOLUME) {
		SetHead(m_clData);
	}
	CRJChainVarBuffer(const CRJChainConstant<TData>& clStr):
		CRJChainVar(VOLUME) {
		SetHead(m_clData);
		Set(clStr);
	}
	_CLASS_COPY_AND_ASSIGN_DELETE(CRJChainVarBuffer)
protected:
	TData m_clData[VOLUME];
};

