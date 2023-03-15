#pragma once
#include <recurj/rjframework/datalength.h>
#include <recurj/rjframework/libs/base/strings/unicode/string.h>
namespace rjBase {
	class CRJStringByte;
	class RJBASE_API CRJStringUnicodeVar:public CRJStringUnicode {
	public:
		inline size_t GetSize() const {
			return m_sSize;
		};
		inline size_t GetCapacity() const {
			return m_sSize-m_sLength;
		}
		virtual bool HasNeededSize(size_t);
		wchar_t* GetHeadForUpdate() const;
		wchar_t&operator[](size_t lPos) {
			return GetHeadForUpdate()[lPos];
		}
		inline bool OK() const {
			return m_clResult.OK();
		}
		inline CRJStringUnicodeVar& SetVarError(int64_t e, size_t r=DataResutRegionChain) {
			m_clResult.SetError(r, e);
			return *this;
		}
		inline CRJStringUnicodeVar& ResetVarError() {
			m_clResult.ResetError();
			return *this;
		}
		inline bool HasVarError() const {
			return m_clResult.HasError();
		}
		CRJStringUnicodeVar& SetEmpty();
		CRJStringUnicodeVar& SetAt(size_t,wchar_t);
		CRJStringUnicodeVar& Set(const CRJStringByte&);
		CRJStringUnicodeVar& Set(const CRJStringUnicode&);
		CRJStringUnicodeVar& Set(const std::wstring&);
		CRJStringUnicodeVar& SetLabel(const wchar_t*);
		CRJStringUnicodeVar& SetLabel(const char*);
		CRJStringUnicodeVar& SetSingle(wchar_t);
		CRJStringUnicodeVar& SetData(const wchar_t*, size_t);
		CRJStringUnicodeVar& SetDataCheckLength(const wchar_t*, size_t);
		CRJStringUnicodeVar& SetDataSafed(const wchar_t*, size_t);
		CRJStringUnicodeVar& SetDataAsLabel(wchar_t*, size_t);
		CRJStringUnicodeVar& SetDataAsLabel(uint8_t*, size_t);
		CRJStringUnicodeVar& SetInt64(int64_t);
		CRJStringUnicodeVar& SetSize(size_t);
		CRJStringUnicodeVar& SetHex(size_t);
		CRJStringUnicodeVar& SetBool(bool);
		CRJStringUnicodeVar& SetInt64PaddingLeft(int64_t, size_t,wchar_t f=L'0');
		CRJStringUnicodeVar& SetSizePaddingLeft(size_t, size_t,wchar_t f=L'0');
		CRJStringUnicodeVar& CatLabel(const wchar_t*);
		CRJStringUnicodeVar& CatLabel(const char*);
		CRJStringUnicodeVar& CatSingle(wchar_t);
		CRJStringUnicodeVar& Cat(const CRJStringUnicode&);
		CRJStringUnicodeVar& Cat(const CRJStringByte&);
		CRJStringUnicodeVar& Cat(const std::wstring&);
		CRJStringUnicodeVar& CatData(const wchar_t*, size_t);
		CRJStringUnicodeVar& CatDataCheckLength(const wchar_t*, size_t);
		CRJStringUnicodeVar& CatDataSafed(const wchar_t*, size_t);
		CRJStringUnicodeVar& CatInt64(int64_t);
		CRJStringUnicodeVar& CatSize(size_t);
		CRJStringUnicodeVar& CatInt64Fmt(int64_t, size_t,wchar_t f=L'0');
		CRJStringUnicodeVar& CatSizeFmt(size_t, size_t,wchar_t f=L'0');
		CRJStringUnicodeVar& SubString(const CRJStringUnicode&, size_t);
		CRJStringUnicodeVar& SubString(const CRJStringUnicode&, size_t, size_t);
		CRJStringUnicodeVar& RemoveAndTailLeft(size_t);
		CRJStringUnicodeVar& Replace(wchar_t,wchar_t);
		CRJStringUnicodeVar& ReplaceSet(const CRJStringUnicode&,wchar_t);
		CRJStringUnicodeVar& DeleteChar(wchar_t);
		CRJStringUnicodeVar& TrimLeft(wchar_t);
		CRJStringUnicodeVar& TrimRight(wchar_t);
		CRJStringUnicodeVar& Trim(wchar_t);
		CRJStringUnicodeVar& TruncLeft(size_t);
		CRJStringUnicodeVar& TruncRight(size_t);
		CRJStringUnicodeVar& UpperCase();
		CRJStringUnicodeVar& LowerCase();
		bool WasFoundTopAndMovedTail(wchar_t, CRJStringUnicodeVar&);
		bool WasFoundTopAndMovedHead(wchar_t, CRJStringUnicodeVar&);
		bool WasFoundBackAndMovedTail(wchar_t, CRJStringUnicodeVar&);
		bool WasFoundBackAndMovedHead(wchar_t, CRJStringUnicodeVar&);
		bool WasFoundTopAndMovedTail(wchar_t, CRJStringUnicodeFlex&);
		bool WasFoundTopAndMovedHead(wchar_t, CRJStringUnicodeFlex&);
		bool WasFoundBackAndMovedTail(wchar_t, CRJStringUnicodeFlex&);
		bool WasFoundBackAndMovedHead(wchar_t, CRJStringUnicodeFlex&);
		CRJStringUnicodeVar& CatCRLF();
		CRJStringUnicodeVar& SetLength(size_t);
		CRJStringUnicodeVar& SetLengthSafed(size_t);
		CRJStringUnicodeVar& CalcCLength();
		CRJStringUnicodeVar& DecodeNum(const CRJStringByte&);
		CRJStringUnicodeVar& SetStringVarError(eUChainErrors e);
		CRJStringUnicodeVar& MakeCLabel();
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringUnicodeVar)
	protected:
		CRJStringUnicodeVar();
		CRJStringUnicodeVar(size_t);
		virtual~CRJStringUnicodeVar();
		CRJStringUnicodeVar& SetVarUndefined() {
			SetConstantUndefined();
			m_sSize=0;
			return *this;
		}
		CRJResult m_clResult;
		size_t m_sSize;
	};
	class RJBASE_API CRJStringUnicodeVarAssigned:public CRJStringUnicodeVar {
	public:
		CRJStringUnicodeVarAssigned();
		explicit CRJStringUnicodeVarAssigned(wchar_t*, size_t);
		CRJStringUnicodeVarAssigned& SetStorage(wchar_t*, size_t);
		inline void SetStorage(CRJStringUnicodeVar& s) {
			SetStorage(s.GetHeadForUpdate(), s.GetSize());
		}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringUnicodeVarAssigned)
	protected:
		CRJStringUnicodeVarAssigned(size_t);
		void InitBuffer(wchar_t*, size_t);
	};
	class RJBASE_API CRJStringUnicodeVarAssignedValue:public CRJStringUnicodeVarAssigned {
	public:
		explicit CRJStringUnicodeVarAssignedValue(const CRJStringUnicodeVar&);
		explicit CRJStringUnicodeVarAssignedValue(wchar_t*, size_t, size_t);
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringUnicodeVarAssignedValue)
	protected:
	};
}
template<size_t VOLUME>
class CRJStringUnicodeVarBuffer:public rjBase::CRJStringUnicodeVar {
public:
	CRJStringUnicodeVarBuffer()noexcept:
	CRJStringUnicodeVar(VOLUME) {
		SetHead(m_szBuffer);
	}
	CRJStringUnicodeVarBuffer(const CRJStringUnicode& clStr):
		CRJStringUnicodeVar(VOLUME) {
		SetHead(m_szBuffer);
		Set(clStr);
	}
	virtual~CRJStringUnicodeVarBuffer() {}
	_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringUnicodeVarBuffer)
protected:
	wchar_t m_szBuffer[VOLUME];
};
namespace rjBase {
	RJBASE_TEMPLATE template 
	class RJBASE_API CRJStringUnicodeVarBuffer<cSizeChainShort>;
	RJBASE_TEMPLATE template 
	class RJBASE_API CRJStringUnicodeVarBuffer<cSizeChainDefault>;
	RJBASE_TEMPLATE template 
	class RJBASE_API CRJStringUnicodeVarBuffer<cSizeChainLong>;
	using CRJStringUnicodeShort=RJBASE_API CRJStringUnicodeVarBuffer<cSizeChainShort>;
	using CRJStringUnicodeDefault=RJBASE_API CRJStringUnicodeVarBuffer<cSizeChainDefault>;
	using CRJStringUnicodeLong=RJBASE_API CRJStringUnicodeVarBuffer<cSizeChainLong>;
}

