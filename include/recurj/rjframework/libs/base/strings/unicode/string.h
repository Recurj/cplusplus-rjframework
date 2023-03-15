#pragma once
#include <recurj/rjframework/libs/base/datatools/chain.h>
#include <recurj/rjframework/libs/base/iterators/rangeloop.h>
#include <recurj/rjframework/functors.h>
#include <functional>
#include <string>
#include <iostream>
namespace rjBase {
	class CRJStringUnicodeFind;
	class CRJStringUnicodeFlex;
	using IRJStringUnicodeSplit=IRJFunctor<CRJStringUnicodeFind&>;
	using IRJStringUnicodeSplitBool=IRJFunctorBool<CRJStringUnicodeFind&>;
	template<typename TFunc>
	using CRJStringUnicodeFunctorSplit=CRJFunctor<IRJStringUnicodeSplit, TFunc, CRJStringUnicodeFind&>;
	template<typename TFunc>
	using CRJStringUnicodeFunctorSplitBool=CRJFunctorBool<IRJStringUnicodeSplitBool, TFunc,
	CRJStringUnicodeFind&>;
	class CRJStringByteVar;
	class RJBASE_API CRJStringUnicode:public CRJChain {
		friend class CRJStringUnicodeFlexConst;
		friend class CRJStringUnicodeFlex;
	public:
		friend RJBASE_API std::ostream&operator<<(ostream& os,const CRJStringUnicode& s) {
			size_t l=s.GetLength();
			const wchar_t* p=s.GetHeadForRead();
			while (l>0) {
				l--;
				std::wcout << *p;
				p++;
			}
			return os;
		}
		CRJStringUnicode(const wchar_t*, size_t lLen=0);
		virtual~CRJStringUnicode();
		inline const wchar_t* GetHeadForRead() const {
			return m_pHead;
		}
		inline const wchar_t* GetTail() const {
			return m_pHead+m_sLength;
		}
		inline bool IsUndefined() const {
			return m_pHead==& s_uUnDef[0];
		}
		inline bool IsDefined() const {
			return m_pHead!=& s_uUnDef[0];
		}
		inline bool IsDefinedEmpty() const {
			return m_pHead!=& s_uEmpty[0];
		}
		bool operator==(const CRJStringUnicode& str) const {
			return Compare(str)==0;
		}
		bool operator!=(const CRJStringUnicode& str) const {
			return Compare(str)!=0;
		}
		bool operator>(const CRJStringUnicode& str) const {
			return Compare(str)>0;
		}
		bool operator>=(const CRJStringUnicode& str) const {
			return Compare(str)>=0;
		}
		bool operator<(const CRJStringUnicode& str) const {
			return Compare(str)<0;
		}
		bool operator<=(const CRJStringUnicode& str) const {
			return Compare(str)<=0;
		};
		wchar_t operator[](size_t lPos) const {
			return m_pHead[lPos];
		}
		operator const wchar_t*() const {
			return (const wchar_t*) m_pHead;
		}
		CRJStringUnicode& GetAt(size_t,wchar_t&);
		bool Contain(wchar_t) const;
		bool FindPos(wchar_t, CRJChainFinder&) const;
		bool FindPosNext(wchar_t, CRJChainFinder&) const;
		bool FindPosBack(wchar_t, CRJChainFinder&) const;
		bool FindPosNextBack(wchar_t, CRJChainFinder&) const;
		bool FindPosNotSameLeft(wchar_t, CRJChainFinder&) const;
		bool FindPosNotSameLeftNext(wchar_t, CRJChainFinder&) const;
		bool FindPosNotSameRight(wchar_t, CRJChainFinder&) const;
		bool FindPosNotSameRightNext(wchar_t, CRJChainFinder&) const;
		bool FindPosInSet(const CRJStringUnicode&, CRJChainFinderSet&) const;
		bool FindPosInSetNext(const CRJStringUnicode&, CRJChainFinderSet&) const;
		bool FindPosInSetBack(const CRJStringUnicode&, CRJChainFinderSet&) const;
		bool FindPosInSetNextBack(const CRJStringUnicode&, CRJChainFinderSet&) const;
		bool FindSubStr(const CRJStringUnicode&, CRJChainFinder&, size_t) const;
		bool FindClosingBracket(wchar_t,wchar_t, size_t&) const;
		bool InBrackets(wchar_t,wchar_t) const;
		bool IsFirstThat(wchar_t) const;
		bool IsLastThat(wchar_t) const;
		bool IsFirstThat(const CRJStringUnicode&) const;
		int64_t Compare(const CRJStringUnicode&) const;
		int64_t Compare(const wchar_t*, size_t) const;
		bool GetC(wchar_t*, size_t) const;
		bool Like(const CRJStringUnicode&) const;
		void Split(wchar_t, IRJStringUnicodeSplit&) const;
		bool SplitResult(wchar_t, IRJStringUnicodeSplitBool&) const;
		bool Split(wchar_t, CRJStringUnicodeFlex&, CRJStringUnicodeFlex&) const;
		bool IsValidInt64(int64_t&) const;
		bool IsValidSize(size_t&) const;
		bool IsValidDouble(double&) const;
		bool IsValidBool(bool&) const;
		size_t ToSizeDef(size_t s=0) const;
		int64_t ToInt64Def(int64_t s=0) const;
		double ToDoubleDef(double d=0) const;
		bool ToBoolDef(bool b=false) const;
		std::wstring& ToStdString(std::wstring&);
		CRJStringByteVar& ToUtf8(CRJStringByteVar&) const;
		CRJStringByteVar& ToCodePage(size_t, CRJStringByteVar&) const;
		bool AllSpaces() const;
		static bool IsGroupSpace(wchar_t);
		static bool IsGroupLineBreak(wchar_t);
		static bool IsGroupControl(wchar_t);
		static size_t GetElemSize() {
			return sizeof(wchar_t);
		}
		static const wchar_t* FindTopThat(const wchar_t*, size_t,wchar_t);
		static const wchar_t* FindTopNotThat(const wchar_t*, size_t,wchar_t);
		static bool FindPosSame(const wchar_t*, size_t,wchar_t, size_t, CRJChainFinder&);
		static bool FindPosNotSame(const wchar_t*, size_t,wchar_t, size_t, CRJChainFinder&);
		static bool FindPosSameBack(const wchar_t*, size_t,wchar_t, CRJChainFinder&);
		static bool FindPosNotSameBack(const wchar_t*, size_t,wchar_t, CRJChainFinder&);
		static bool Like(const wchar_t*, size_t,const wchar_t*, size_t);
		static CRJStringUnicode s_spaceGroup;
		static CRJStringUnicode s_lineBreakGroup;
		static CRJStringUnicode s_controlGroup;
		static CRJStringUnicode s_forwardSlash;
		static CRJStringUnicode s_backSlash;
		static CRJStringUnicode s_asterisk;
		static CRJStringUnicode s_apostrophe;
		static CRJStringUnicode s_questionMark;
		static CRJStringUnicode s_WordYES;
		static CRJStringUnicode s_WordTRUE;
		static CRJStringUnicode s_WordNO;
		static CRJStringUnicode s_WordFALSE;
		static CRJStringUnicode s_Word1;
		static CRJStringUnicode s_Word0;
		static CRJStringUnicode s_WordN;
		static CRJStringUnicode s_WordY;
		static CRJStringUnicode s_LineNext;
		static CRJStringUnicode s_LineFeed;
		static CRJStringUnicode s_CRLF;
	public:
		using iterator=CRJRangeLoop<wchar_t>;
		iterator begin() const {
			return iterator(m_pHead, 0);
		}
		iterator end() const {
			return iterator(m_pHead, m_sLength);
		}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringUnicode)
	protected:
		CRJStringUnicode();
		void SetHead(const wchar_t* p) {
			m_pHead=p;
		}
		void SetHeadAndLength(const wchar_t* p, size_t s) {
			m_pHead=p;
			m_sLength=s;
		}
		CRJStringUnicode& SetConstantUndefined() {
			m_pHead=s_uUnDef;
			m_sLength=0;
			return *this;
		}
		CRJStringUnicode& SetConstantDefinedEmpty() {
			m_pHead=s_uEmpty;
			m_sLength=0;
			return *this;
		}
	protected:
		static const wchar_t s_uUnDef[1];
		static const wchar_t s_uEmpty[1];
	private:
		const wchar_t* m_pHead;
	};
}

