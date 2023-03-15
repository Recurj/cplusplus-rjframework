#pragma once
#include <recurj/rjframework/datalength.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#define _CBYTES_SAFED(_a) _a[sizeof(_a)-1]=0x00
#define _CBYTES_COPY_BLOCK(aDst,aSrc) CRJStringByteVar::CBytesCopyBlock(aDst,sizeof(aDst),aSrc)
#define _CBYTES_COPY_STRING(aDst,aSrc) CRJStringByteVar::CBytesCopyString(aDst,sizeof(aDst),aSrc)
#define _CBYTES_COPY_LABEL(aDst,aSrc) CRJStringByteVar::CBytesCopyLabel(aDst,sizeof(aDst),aSrc)
#define _STRING_BYTE_VARIABLE_DECLARE(name,buf) CRJStringByteVarAssigned name((uint8_t *)buf,sizeof(buf))
#define _STRING_BYTE_VARIABLE_SWITCH_BUFFER(name,buf) name.SetStorage((uint8_t *)buf,sizeof(buf))
#define _SETDATA_AS_LABEL(str,buf) str.SetDataAsLabel(buf,sizeof(buf))
namespace rjBase {
	class CRJStringUnicode;
	class RJBASE_API CRJStringByteVar:public CRJStringByte {
	public:
		inline size_t GetSize() const {return m_sSize;};
		inline size_t GetCapacity() const {return m_sSize-m_sLength;}
		inline bool HasSpace() const {return m_sSize > m_sLength;}
		virtual bool HasNeededSize(size_t);
		uint8_t* GetHeadForUpdate() const;
		char* GetHeadForUpdateC() const;
		uint8_t&operator[](size_t lPos) {
			return GetHeadForUpdate()[lPos];
		}
		inline bool OK() const {
			return m_clResult.OK();
		}
		inline CRJStringByteVar& SetVarRegion(size_t r) {
			m_clResult.Region(r);
			return *this;
		}
		inline CRJStringByteVar& SetVarRegionError(int64_t e) {
			m_clResult.Result(e);
			return *this;
		}
		inline CRJStringByteVar& SetVarError(int64_t e, size_t r=DataResutRegionChain) {
			m_clResult.SetError(r, e);
			return *this;
		}
		inline void ResetVarError() {
			m_clResult.ResetError();
		}
		inline bool HasVarError() const {
			return m_clResult.HasError();
		}
		CRJStringByteVar& SetEmpty();
		CRJStringByteVar& SetAt(size_t, uint8_t);
		CRJStringByteVar& Set(const CRJStringUnicode&);
		CRJStringByteVar& Set(const CRJStringByte&);
		CRJStringByteVar& Set(const std::string&);
		CRJStringByteVar& SetData(const uint8_t*, size_t);
		CRJStringByteVar& SetDataCheckLength(const uint8_t*, size_t);
		CRJStringByteVar& SetDataSafed(const uint8_t*, size_t);
		CRJStringByteVar& SetDataAsLabel(uint8_t*, size_t);
		CRJStringByteVar& SetSingle(uint8_t);
		CRJStringByteVar& SetLabel(const char*);
		CRJStringByteVar& SetLabel(const wchar_t*);
		CRJStringByteVar& SetLabel(const char*, size_t);
		CRJStringByteVar& SetInt64(int64_t);
		CRJStringByteVar& SetInt64PaddingLeft(int64_t, size_t, uint8_t ch='0');
		CRJStringByteVar& SetInt64Formatted(_In_z_ const char*, int64_t);
		CRJStringByteVar& SetSizeFormatted(_In_z_ const char*, size_t);
		CRJStringByteVar& SetDouble(double);
		CRJStringByteVar& SetDoubleFormatted(_In_z_ const char*,double);
		CRJStringByteVar& SetSize(size_t);
		CRJStringByteVar& SetSizePaddingLeft(size_t, size_t, uint8_t ch='0');
		CRJStringByteVar& SetHex(size_t);
		CRJStringByteVar& SetBool(bool);
		CRJStringByteVar& SetDTime(time_t);
		CRJStringByteVar& SetLast(uint8_t);
		CRJStringByteVar& SetFill(size_t, uint8_t='0');
		CRJStringByteVar& CatSingle(uint8_t);
		CRJStringByteVar& Cat(const CRJStringByte&);
		CRJStringByteVar& Cat(const CRJStringUnicode&);
		CRJStringByteVar& Cat(const std::string&);
		CRJStringByteVar& CatDataAsLabel(uint8_t*, size_t);
		CRJStringByteVar& CatData(const uint8_t*, size_t);
		CRJStringByteVar& CatDataCheckLength(const uint8_t*, size_t);
		CRJStringByteVar& CatDataSafed(const uint8_t*, size_t);
		CRJStringByteVar& CatLabel(const char*);
		CRJStringByteVar& CatLabel(const char*, size_t);
		CRJStringByteVar& CatLabel(const wchar_t*);
		CRJStringByteVar& CatInt64(int64_t);
		CRJStringByteVar& CatSize(size_t);
		CRJStringByteVar& CatDouble(double);
		CRJStringByteVar& CatFill(size_t, uint8_t='0');
		CRJStringByteVar& CatInt64Fmt(int64_t, size_t, uint8_t f='0');
		CRJStringByteVar& CatSizeFmt(size_t, size_t, uint8_t f='0');
		CRJStringByteVar& CatPath();
		CRJStringByteVar& SubString(const CRJStringByte&, size_t);
		CRJStringByteVar& SubString(const CRJStringByte&, size_t, size_t);
		CRJStringByteVar& RemoveAndTailLeft(size_t);
		CRJStringByteVar& Replace(uint8_t, uint8_t);
		CRJStringByteVar& ReplaceSet(const CRJStringByte&, uint8_t);
		CRJStringByteVar& DeleteChar(uint8_t);
		CRJStringByteVar& DeleteHexSymbols();
		CRJStringByteVar& BuildHexSymbols(const CRJStringByte&);
		CRJStringByteVar& BuildHexSymbols(const uint8_t*, size_t);
		CRJStringByteVar& TrimLeft(uint8_t);
		CRJStringByteVar& TrimRight(uint8_t);
		CRJStringByteVar& Trim(uint8_t);
		CRJStringByteVar& TruncLeft(size_t);
		CRJStringByteVar& TruncRight(size_t);
		CRJStringByteVar& UpperCase();
		CRJStringByteVar& LowerCase();
		CRJStringByteVar& Clean();
		CRJStringByteVar& FormatAlignedWidth(CRJStringByteVar&, size_t,bool);
		bool WasFoundTopAndMovedTail(uint8_t, CRJStringByteVar&);
		bool WasFoundTopAndMovedHead(uint8_t, CRJStringByteVar&);
		bool WasFoundBackAndMovedTail(uint8_t, CRJStringByteVar&);
		bool WasFoundBackAndMovedHead(uint8_t, CRJStringByteVar&);
		bool WasFoundTopAndMovedTail(uint8_t, CRJStringByteFlex&);
		bool WasFoundTopAndMovedHead(uint8_t, CRJStringByteFlex&);
		bool WasFoundBackAndMovedTail(uint8_t, CRJStringByteFlex&);
		bool WasFoundBackAndMovedHead(uint8_t, CRJStringByteFlex&);
		bool WasFoundTopAndRemovedHead(uint8_t);
		bool WasFoundTopAndRemovedTail(uint8_t);
		bool WasFoundBackAndRemovedHead(uint8_t);
		bool WasFoundBackAndRemovedTail(uint8_t);
		CRJStringByteVar& CatCRLF();
		CRJStringByteVar& CatLN();
		CRJStringByteVar& CatLF();
		CRJStringByteVar& SetLength(size_t);
		CRJStringByteVar& SetLengthSafed(size_t);
		CRJStringByteVar& CalcCLength();
		CRJStringByteVar& MakeCLabel();
		CRJStringByteVar& DecodeNum(const CRJStringUnicode&);
		CRJStringByteVar& SetStringVarError(eUChainErrors e);
		CRJStringByteVar& SetVarUndefined() {
			SetConstantUndefined();
			m_sSize=0;
			return *this;
		}	
		inline void GrowSafed(size_t s) { m_sLength += s; }
		inline void ExpandedOutside(size_t s) {m_sLength+=s;}
		static size_t CBytesCopyLabel(uint8_t*, size_t,const char*, size_t lSrc=0);
		static size_t CBytesCopyString(uint8_t*, size_t,const CRJStringByte&);
		static void CBytesCopyBlock(uint8_t*, size_t,const uint8_t*);
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringByteVar)
	protected:
		CRJStringByteVar();
		CRJStringByteVar(size_t);
		virtual~CRJStringByteVar();

		CRJResult m_clResult;
		size_t m_sSize;
	};
	class RJBASE_API CRJStringByteVarAssigned:public CRJStringByteVar {
	public:
		CRJStringByteVarAssigned();
		CRJStringByteVarAssigned(uint8_t*, size_t);
		CRJStringByteVarAssigned(const CRJStringByte&);
		CRJStringByteVarAssigned& SetStorageWithData(const CRJStringByte&);
		CRJStringByteVarAssigned& SetStorageWithData(uint8_t*, size_t, size_t);
		CRJStringByteVarAssigned& SetStorageWithData(uint8_t*, size_t);
		CRJStringByteVarAssigned& SetStorage(uint8_t*, size_t);
		inline void SetStorage(CRJStringByteVar& s) { SetStorage(s.GetHeadForUpdate(), s.GetSize()); }
		CRJStringByteVarAssigned& Move(size_t);
		CRJStringByteVarAssigned& Back(size_t);
		CRJStringByteVarAssigned& Fill(uint8_t ch = 0x00);
	protected:
		CRJStringByteVarAssigned(size_t);
		void InitBuffer(uint8_t*, size_t);
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringByteVarAssigned)
	};
	class RJBASE_API CRJStringByteVarAssignedValue:public CRJStringByteVarAssigned {
	public:
		CRJStringByteVarAssignedValue(const CRJStringByteVar&);
		CRJStringByteVarAssignedValue(uint8_t*, size_t, size_t);
		CRJStringByteVarAssignedValue(uint8_t*, size_t);
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringByteVarAssignedValue)
	};
	template<size_t VOLUME>
	class CRJStringByteVarBuffer:public CRJStringByteVar {
	public:
		CRJStringByteVarBuffer()noexcept:
		CRJStringByteVar(VOLUME) {
			SetHead(m_szBuffer);
		}
		CRJStringByteVarBuffer(const CRJStringByte& clStr):
			CRJStringByteVar(VOLUME) {
			SetHead(m_szBuffer);
			Set(clStr);
		}
		CRJStringByteVarBuffer(const char* s):
			CRJStringByteVar(VOLUME) {
			SetHead(m_szBuffer);
			SetLabel(s);
		}
		virtual~CRJStringByteVarBuffer() {}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringByteVarBuffer)
	protected:
		uint8_t m_szBuffer[VOLUME];
	};
	RJBASE_TEMPLATE template 
	class RJBASE_API CRJStringByteVarBuffer<cSizeChainShort>;
	RJBASE_TEMPLATE template 
	class RJBASE_API CRJStringByteVarBuffer<cSizeChainDefault>;
	RJBASE_TEMPLATE template 
	class RJBASE_API CRJStringByteVarBuffer<cSizeChainLong>;
	using CRJStringByteShort=RJBASE_API CRJStringByteVarBuffer<cSizeChainShort>;
	using CRJStringByteDefault=RJBASE_API CRJStringByteVarBuffer<cSizeChainDefault>;
	using CRJStringByteLong=RJBASE_API CRJStringByteVarBuffer<cSizeChainLong>;
}

