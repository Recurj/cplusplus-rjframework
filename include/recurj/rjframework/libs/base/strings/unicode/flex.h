#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/strings/unicode/string.h>
namespace rjBase {
	class CRJStringUnicodeVar;
	class RJBASE_API CRJStringUnicodeFlex:public CRJStringUnicode {
	public:
		CRJStringUnicodeFlex();
		CRJStringUnicodeFlex(const wchar_t*, size_t);
		CRJStringUnicodeFlex(const CRJStringUnicode&);
		CRJStringUnicodeFlex(const CRJStringUnicodeFlex&);
		CRJStringUnicodeFlex(const CRJStringUnicode&, size_t, size_t);
		CRJStringUnicodeFlex&operator=(const CRJStringUnicodeFlex&);
		CRJStringUnicodeFlex& Set(const CRJStringUnicode&);
		CRJStringUnicodeFlex& SetFromData(const wchar_t*, size_t);
		CRJStringUnicodeFlex& Slice(const CRJStringUnicode&, size_t, size_t);
		CRJStringUnicodeFlex& Slice(const CRJStringUnicode&, size_t);
		CRJStringUnicodeFlex& SetFromLabel(const wchar_t*);
		bool IsFlexUndefined() const;
		CRJStringUnicodeFlex& TrimLeft(wchar_t);
		CRJStringUnicodeFlex& TrimRight(wchar_t);
		CRJStringUnicodeFlex& Trim(wchar_t);
		CRJStringUnicodeFlex& TrimLeft(const CRJStringUnicode&);
		CRJStringUnicodeFlex& TrimRight(const CRJStringUnicode&);
		CRJStringUnicodeFlex& Trim(const CRJStringUnicode&);
		CRJStringUnicodeFlex& TruncLeft(size_t);
		CRJStringUnicodeFlex& TruncRight(size_t);
		CRJStringUnicodeFlex& SetEmpty();
		CRJStringUnicodeFlex& SetLengthSafed(size_t);
		CRJStringUnicodeFlex& Grow(size_t l=1);
		bool WasFoundTopAndMovedHead2Var(wchar_t, CRJStringUnicodeVar&);
		bool WasFoundTopAndMovedTail2Var(wchar_t, CRJStringUnicodeVar&);
		bool WasFoundBackAndMovedTail2Var(wchar_t, CRJStringUnicodeVar&);
		bool WasFoundBackAndMovedHead2Var(wchar_t, CRJStringUnicodeVar&);
		bool WasFoundTopAndMovedHead(wchar_t, CRJStringUnicodeFlex&);
		bool WasFoundTopAndMovedTail(wchar_t, CRJStringUnicodeFlex&);
		bool WasFoundBackAndMovedTail(wchar_t, CRJStringUnicodeFlex&);
		bool WasFoundBackAndMovedHead(wchar_t, CRJStringUnicodeFlex&);
		bool WasFoundTopAndMovedTail(const CRJStringUnicode&, CRJStringUnicodeFlex&, CRJChainFinderSet&);
		CRJStringUnicodeFlex& FindTopAndMoveHead(wchar_t, CRJStringUnicodeFlex&);
	protected:
		CRJStringUnicodeFlex& SetFlexUndefined() {
			SetConstantUndefined();
			return *this;
		}
	};
	class RJBASE_API CRJStringUnicodeFind:public CRJStringUnicodeFlex {
	public:
		using CRJStringUnicodeFlex::CRJStringUnicodeFlex;
		CRJStringUnicodeFind& Last() {
			bLast=true;
			return *this;
		}
		size_t lIndex=0;
		bool bLast=false;
	};
}

