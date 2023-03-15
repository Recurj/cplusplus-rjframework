#pragma once
#include <recurj/rjframework/libs/base/datatools/chain.h>
#include <recurj/rjframework/libs/base/iterators/rangeloop.h>
#include <recurj/rjframework/functors.h>
 template<typename TData>
class CRJChainFlex;
template<typename TData>
class CRJChainFind;
template<typename TData>
using IRJChainSplit=IRJFunctor<CRJChainFind<TData>&>;
template<typename TData>
using IRJChainSplitBool=IRJFunctorBool<CRJChainFind<TData>&>;
template<typename TData,typename TFunc>
using CRJChainFunctorSplit=CRJFunctor<IRJChainSplit<TData>, TFunc, CRJChainFind<TData>&>;
template<typename TData,typename TFunc>
using CRJChainFunctorSplitBool=CRJFunctorBool<IRJChainSplitBool<TData>, TFunc, CRJChainFind<TData>&>;
template<typename TData>
class CRJChainConstant:public rjBase::CRJChain {
	friend class CRJChainFlex<TData>;
public:
	CRJChainConstant() {
		SetConstantUndefined();
	}
	CRJChainConstant(const TData* p, size_t lLen) {
		SetHeadAndLength(p, lLen);
	}
	virtual~CRJChainConstant() {}
	inline bool IsUndefined() const {
		return m_pHead==& s_uUnDef[0];
	}
	inline bool IsDefined() const {
		return m_pHead!=& s_uUnDef[0];
	}
	inline const TData* GetHeadForRead() const {
		return m_pHead;
	}
	inline const TData* GetTailForRead() const {
		return m_pHead+m_sLength;
	}
	bool operator==(const CRJChainConstant& clChain) const {
		return Compare(clChain)==0;
	}
	bool operator!=(const CRJChainConstant& clChain) const {
		return Compare(clChain)!=0;
	}
	bool operator>(const CRJChainConstant& clChain) const {
		return Compare(clChain)>0;
	}
	bool operator>=(const CRJChainConstant& clChain) const {
		return Compare(clChain)>=0;
	}
	bool operator<(const CRJChainConstant& clChain) const {
		return Compare(clChain)<0;
	}
	bool operator<=(const CRJChainConstant& clChain) const {
		return Compare(clChain)<=0;
	};
	const TData&operator[](size_t lPos) const {
		return m_pHead[lPos];
	}
	CRJChainConstant& GetAt(size_t lPos, TData& ch) {
		if ((m_sLength>0)&&
			(lPos<m_sLength)) ch=m_pHead[lPos];
		return *this;
	}
	bool Contain(const TData& ch) const {
		const TData* p=m_pHead;
		for (size_t i=0; i<m_sLength; i++)
		if (p[i]==ch) return true;
		return false;
	}
	bool FindPos(const TData& ch, CRJChainFinder& clFound) const {
		return FindPosSame(m_pHead, m_sLength, ch, 0, clFound);
	}
	bool FindPosNext(const TData& ch, CRJChainFinder& clFound) const {
		size_t lLast=clFound.m_sFound+1;
		return (m_sLength>lLast)?
			FindPosSame(m_pHead+lLast, m_sLength-lLast, ch, lLast, clFound):
			clFound.NotFoundPos();
	}
	bool FindPosBack(const TData& ch, CRJChainFinder& clFound) const {
		return FindPosSameBack(GetTailForRead(), m_sLength, ch, clFound);
	}
	bool FindPosNextBack(const TData& ch, CRJChainFinder& clFound) const {
		return ((clFound.m_sFound>0)&&
			(clFound.m_sFound<m_sLength))?
			FindPosSameBack(m_pHead+clFound.m_sFound, clFound.m_sFound, ch, clFound):
			clFound.NotFoundPos();
	}
	bool FindPosNotSameLeft(const TData& ch, CRJChainFinder& clFound) const {
		return FindPosNotSame(m_pHead, m_sLength, ch, 0, clFound);
	}
	bool FindPosNotSameLeftNext(const TData& ch, CRJChainFinder& clFound) const {
		size_t lLast=clFound.m_sFound+1;
		return (m_sLength>lLast)?
			FindPosNotSame(m_pHead+lLast, m_sLength-lLast, ch, lLast, clFound):
			clFound.NotFoundPos();
	}
	bool FindPosNotSameRight(const TData& ch, CRJChainFinder& clFound) const {
		return FindPosNotSameBack(GetTailForRead(), m_sLength, ch, clFound);
	}
	bool FindPosNotSameRightNext(const TData& ch, CRJChainFinder& clFound) const {
		return ((clFound.m_sFound>0)&&
			(clFound.m_sFound<m_sLength))?
			FindPosNotSameBack(m_pHead+clFound.m_sFound, clFound.m_sFound, ch, clFound):
			clFound.NotFoundPos();
	}
	bool FindPosInSet(const CRJChainConstant& clMask, CRJChainFinderSet& clFound) const {
		CRJChainFinder st;
		const TData* pData=m_pHead;
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
	bool FindPosInSetNext(const CRJChainConstant& clMask, CRJChainFinderSet& clFound) const {
		size_t lLast=clFound.m_sFound+1;
		if (m_sLength>lLast) {
			CRJChainFinder st;
			size_t i, f=0, c=clMask.GetLength(),
			s=m_sLength-lLast;
			const TData* pData=m_pHead+lLast;
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
	bool FindPosInSetBack(const CRJChainConstant& clMask, CRJChainFinderSet& clFound) const {
		CRJChainFinder st;
		size_t i, f=0, c=clMask.GetLength(),
		s=m_sLength;
		const TData* pData=m_pHead;
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
	bool FindPosInSetNextBack(const CRJChainConstant& clMask, CRJChainFinderSet& clFound) const {
		if ((clFound.m_sFound>0)&&
			(m_sLength>clFound.m_sFound)) {
			CRJChainFinder st;
			size_t i, f=0, c=clMask.GetLength(),
			s=clFound.m_sFound;
			const TData* pData=m_pHead;
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
	bool FindClosingBracket(const TData& chOpen,const TData& chClose, size_t& lPos) const {
		const TData* pData=m_pHead;
		size_t c=1, l=GetLength();
		for (lPos=0; lPos<l; lPos++, pData++) {
			if (pData[0]==chOpen) c++;
			else if (pData[0]==chClose) {
				c--;
				if (c==0) return true;
			}
		}
		return false;
	}
	bool IsSameData(const TData* p,const TData* q, size_t lLen) const {
		while (lLen>0) {
			if (* p!=* q) return false;
			p++;
			q++;
			lLen--;
		}
		return true;
	}
	bool FindSubStr(const CRJChainConstant& clSubStr, CRJChainFinder& clFound, size_t off) const {
		size_t sLen=clSubStr.GetLength();
		if ((m_sLength>off)&&
			(sLen<=(m_sLength-off))) {
			CRJChainFinder st;
			size_t sFirst=off, sLast=m_sLength-sLen+1;
			const TData* pData=m_pHead;
			const TData* pSubStr=clSubStr.m_pHead;
			TData chFirst=pSubStr[0];
			while (FindPosSame(pData+sFirst, sLast-sFirst, chFirst, sFirst, st)) {
				if (IsSameData(& pData[st.m_sFound], pSubStr, lLen)) return clFound.FoundPos(st.m_sFound);
				sFirst=st.m_sFound+1;
				if (sFirst>=sLast)break;
			}
		}
		return clFound.NotFoundPos();
	}
	bool InBrackets(const TData& l,const TData& r) const {
		return m_sLength>1&&
			l==m_pHead[0]&&
			r==m_pHead[m_sLength-1];
	}
	bool IsLastThat(const TData& clElem) const {
		return m_sLength>0&&
			clElem==m_pHead[m_sLength-1];
	}
	bool IsFirstThat(const TData& clElem) const {
		return m_sLength>0&&
			clElem==m_pHead[0];
	}
	bool IsFirstThat(const CRJChainConstant& str) const {
		return m_sLength>0&&
			str.Contain(m_pHead[0]);
	}
	int64_t Compare(const CRJChainConstant& str) const {
		int64_t rc=CRJDataTools::SizeCompare(m_sLength, str.GetLength());
		if (!rc) {
			const TData* p=m_pHead;
			const TData* q=str.m_pHead;
			for (size_t i=0; i<m_sLength; i++, p++, q++) {
				rc=p->Metric(* q);
				if (!rc)break;
			}
		}
		return rc;
	}
	void Split(const TData& ch, IRJChainSplit<TData>& clFunc) const {
		CRJChainFinder clFinder;
		CRJChainFind<TData>str;
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
	bool SplitResult(const TData& ch, IRJChainSplitBool<TData>& clFunc) const {
		CRJChainFinder clFinder;
		CRJChainFind<TData>str;
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
	bool Split(const TData& ch, CRJChainFlex<TData>& l, CRJChainFlex<TData>& r) const {
		CRJChainFinder clFinder;
		if (FindPos(ch, clFinder)) {
			l.Slice(*this, 0, clFinder.m_sFound);
			r.Slice(*this, clFinder.m_sFound+1, m_sLength);
			return true;
		}
		return false;
	}
	using iterator=CRJRangeLoop<TData>;
	iterator begin() const {
		return iterator(m_pHead, 0);
	}
	iterator end() const {
		return iterator(m_pHead, m_sLength);
	}
	_CLASS_COPY_AND_ASSIGN_DELETE(CRJChainConstant)
protected:
	bool FindPosSame(const TData* p, size_t sLen,const TData& ch, size_t sOffset, CRJChainFinder& clFound) const {
		for (size_t i=0; i<sLen; i++)
		if (ch==p[i]) return clFound.FoundPos(i+sOffset);
		return clFound.NotFoundPos();
	}
	bool FindPosNotSame(const TData* p, size_t lLen,const TData& ch, size_t lOffset, CRJChainFinder& clFound) const {
		for (size_t i=0; i<lLen; i++)
		if (p[i]!=ch) return clFound.FoundPos(i+lOffset);
		return clFound.NotFoundPos();
	}
	bool FindPosSameBack(const TData* p, size_t lLen,const TData& ch, CRJChainFinder& clFound) const {
		while (lLen>0) {
			lLen--;
			if (p[lLen]==ch) return clFound.FoundPos(lLen);
		}
		return clFound.NotFoundPos();
	}
	static bool FindPosNotSameBack(const TData* p, size_t lLen,const TData& ch, CRJChainFinder& clFound) {
		while (lLen>0) {
			lLen--;
			if (p[lLen]!=ch) return clFound.FoundPos(lLen);
		}
		return clFound.NotFoundPos();
	}
	void SetHead(const TData* p) {
		m_pHead=p;
	}
	void SetHeadAndLength(const TData* p, size_t lLen) {
		m_pHead=p;
		m_sLength=lLen;
	}
	CRJChainConstant& SetConstantUndefined() {
		m_pHead=s_uUnDef;
		m_sLength=0;
		return *this;
	}
private:
	const TData s_uUnDef[1]= {};
	const TData* m_pHead;
};

