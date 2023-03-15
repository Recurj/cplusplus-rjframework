#pragma once
#include <functional>
#include <string>
#include <iostream>
#include <recurj/rjframework/libs/base/datatools/chain.h>
#include <recurj/rjframework/libs/base/iterators/rangeloop.h>
#include <recurj/rjframework/functors.h>
namespace rjBase {
	class CRJStringByteFind;
	class CRJStringByteFlex;
	class CRJStringUnicodeVar;
	class CRJFileName;
	using IRJStringByteSplit=IRJFunctor<CRJStringByteFind&>;
	using IRJStringByteSplitBool=IRJFunctorBool<CRJStringByteFind&>;
	template<typename TFunc>
	using CRJStringByteFunctorSplit=CRJFunctor<IRJStringByteSplit, TFunc, CRJStringByteFind&>;
	template<typename TFunc>
	using CRJStringByteFunctorSplitBool=CRJFunctorBool<IRJStringByteSplitBool, TFunc,
	CRJStringByteFind&>;
	class RJBASE_API CRJStringByte:public CRJChain {
		friend class CRJStringByteFlex;
		friend class CRJStringByteVarAssigned;
	public:
		friend RJBASE_API std::ostream&operator<<(ostream& os,const CRJStringByte& s) {
			size_t l=s.GetLength();
			const char* p=s.GetHeadForReadC();
			while (l>0) {
				l--;
				std::cout<<* p;
				p++;
			}
			return os;
		}
		CRJStringByte(const char*, size_t lLen=0);
		virtual~CRJStringByte();
		inline const uint8_t* GetHeadForRead() const {
			return m_pHead;
		}
		inline const char* GetHeadForReadC() const {
			return (char*) m_pHead;
		}
		inline const uint8_t* GetTail() const {
			return m_pHead + m_sLength;
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
		CRJStringByteFlex CreateBlockByte() const;
		bool operator==(const CRJStringByte& str) const {
			return Compare(str)==0;
		}
		bool operator!=(const CRJStringByte& str) const {
			return Compare(str)!=0;
		}
		bool operator>(const CRJStringByte& str) const {
			return Compare(str)>0;
		}
		bool operator>=(const CRJStringByte& str) const {
			return Compare(str)>=0;
		}
		bool operator<(const CRJStringByte& str) const {
			return Compare(str)<0;
		}
		bool operator<=(const CRJStringByte& str) const {
			return Compare(str)<=0;
		};
		const uint8_t operator[](size_t lPos) const {
			return m_pHead[lPos];
		}
		operator const uint8_t*() const {
			return m_pHead;
		}
		const uint8_t* GetHeadByOffset(size_t) const;
		CRJStringByte& GetAt(size_t, uint8_t&);
		bool Contain(uint8_t) const;
		bool FindPos(uint8_t, CRJChainFinder&) const;
		bool FindPosNext(uint8_t, CRJChainFinder&) const;
		bool FindPosBack(uint8_t, CRJChainFinder&) const;
		bool FindPosNextBack(uint8_t, CRJChainFinder&) const;
		bool FindPosNotSameLeft(uint8_t, CRJChainFinder&) const;
		bool FindPosNotSameLeftNext(uint8_t, CRJChainFinder&) const;
		bool FindPosNotSameRight(uint8_t, CRJChainFinder&) const;
		bool FindPosNotSameRightNext(uint8_t, CRJChainFinder&) const;
		bool FindPosInSet(const CRJStringByte&, CRJChainFinderSet&) const;
		bool FindPosInSetNext(const CRJStringByte&, CRJChainFinderSet&) const;
		bool FindPosInSetBack(const CRJStringByte&, CRJChainFinderSet&) const;
		bool FindPosInSetNextBack(const CRJStringByte&, CRJChainFinderSet&) const;
		bool FindSubStr(const CRJStringByte&, CRJChainFinder&, size_t) const;
		bool FindClosingBracket(char,char, size_t&) const;
		int64_t Compare(const CRJStringByte&) const;
		int64_t Compare(const uint8_t*, size_t) const;
		bool GetC(uint8_t*, size_t) const;
		bool Like(const CRJStringByte&) const;
		void Split(uint8_t, IRJStringByteSplit&) const;
		bool SplitResult(uint8_t, IRJStringByteSplitBool&) const;
		bool Split(uint8_t, CRJStringByteFlex&, CRJStringByteFlex&) const;
		bool InBrackets(uint8_t, uint8_t) const;
		bool IsFirstThat(uint8_t) const;
		bool IsFirstThat(const CRJStringByte&) const;
		bool IsLastThat(uint8_t) const;
		bool IsLastThat(const CRJStringByte&) const;
		bool IsValidInt64(int64_t&) const;
		bool IsValidInt32(int32_t&) const;
		bool IsValidSize(size_t&) const;
		bool IsValidHexSize(size_t&) const;
		bool IsValidDouble(double&) const;
		bool IsValidBool(bool&) const;
		bool ToBoolCheck(bool&) const;
		size_t ToSizeDef(size_t s=0) const;
		int64_t ToInt64Def(int64_t s=0) const;
		int32_t ToInt32Def(int32_t s=0) const;
		double ToDoubleDef(double d=0) const;
		bool ToBoolDef(bool b=false) const;
		size_t ToBlockBytePacked(uint8_t*, size_t) const;
		std::string& ToStdString(std::string&);
		CRJStringUnicodeVar& ToUnicode(CRJStringUnicodeVar&) const;
		CRJStringUnicodeVar& ToUnicode(size_t, CRJStringUnicodeVar&) const;
		CRJStringByteVar& ToCodePage(size_t, CRJStringByteVar&) const;
		CRJStringByteVar& ToUtf8(size_t, CRJStringByteVar&) const;
		bool AllSpaces() const;
		static size_t GetElemSize() {return sizeof(uint8_t);}
		static const uint8_t* FindTopThat(const uint8_t*, size_t, uint8_t);
		static const uint8_t* FindTopNotThat(const uint8_t*, size_t, uint8_t);
		static bool FindPosSame(const uint8_t*, size_t, uint8_t, size_t, CRJChainFinder&);
		static bool FindPosNotSame(const uint8_t*, size_t, uint8_t, size_t, CRJChainFinder&);
		static bool FindPosSameBack(const uint8_t*, size_t, uint8_t, CRJChainFinder&);
		static bool FindPosNotSameBack(const uint8_t*, size_t, uint8_t, CRJChainFinder&);
		static bool Like(const uint8_t*, size_t,const uint8_t*, size_t);
		static const CRJStringByte& GetEmptyString() {return CRJStringByte::s_clEmptyStringChar;}
		static const CRJStringByte s_WordYES;
		static const CRJStringByte s_WordTRUE;
		static const CRJStringByte s_WordNO;
		static const CRJStringByte s_WordFALSE;
		static const CRJStringByte s_Word1;
		static const CRJStringByte s_Word0;
		static const CRJStringByte s_WordN;
		static const CRJStringByte s_WordY;
		static const CRJStringByte s_LineNext;
		static const CRJStringByte s_LineFeed;
		static const CRJStringByte s_CRLF;
		static const CRJStringByte s_clEmptyStringChar;
	public:
		using iterator=CRJRangeLoop<uint8_t>;
		iterator begin() const {
			return iterator(m_pHead, 0);
		}
		iterator end() const {
			return iterator(m_pHead, m_sLength);
		}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringByte)
	protected:
		CRJStringByte();
		void SetHead(const uint8_t* p) {
			m_pHead=p;
		}
		void SetHeadAndLength(const uint8_t* p, size_t s) {
			m_pHead=p;
			m_sLength=s;
		}
		CRJStringByte& SetConstantUndefined() {
			m_pHead=s_uUnDef;
			m_sLength=0;
			return *this;
		}
		CRJStringByte& SetConstantDefinedEmpty() {
			m_pHead=s_uEmpty;
			m_sLength=0;
			return *this;
		}
	protected:
		static const uint8_t s_uUnDef[1];
		static const uint8_t s_uEmpty[1];
	private:
		const uint8_t* m_pHead;
	};
	class RJBASE_API CRJStringByteData:public CRJStringByte {
	public:
		CRJStringByteData(const uint8_t*, size_t);
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringByteData)
	};
	using IRJStringByte=IRJFunctor<const CRJStringByte&>;
	using IRJStringByteBool=IRJFunctorBool<const CRJStringByte&>;
	template<typename TFunc>
	using CRJStringByteFunctor=CRJFunctor<IRJStringByte, TFunc,const CRJStringByte&>;
	template<typename TFunc>
	using CRJStringByteFunctorBool=CRJFunctorBool<IRJStringByteBool, TFunc,const CRJStringByte&>;
}

