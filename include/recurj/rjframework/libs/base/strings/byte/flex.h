#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#define _SETFLEX_AS_LABEL(str,buf) str.SetFromDataAsLabel(buf,sizeof(buf))
namespace rjBase {
	class CRJStringByteVar;
	class RJBASE_API CRJStringByteFlex:public CRJStringByte {
	public:
		CRJStringByteFlex()noexcept;
		CRJStringByteFlex(const uint8_t*, size_t);
		CRJStringByteFlex(const CRJStringByteFlex&);
		CRJStringByteFlex(const CRJStringByte&);
		CRJStringByteFlex(const CRJStringByte&, size_t, size_t);
		CRJStringByteFlex&operator=(const CRJStringByte&);
		CRJStringByteFlex& Set(const CRJStringByte&);
		CRJStringByteFlex& SetFromData(const uint8_t*, size_t);
		CRJStringByteFlex& SetFromDataAsLabel(uint8_t*, size_t);
		CRJStringByteFlex& SetFromDataAsLabelWithLength(const uint8_t*, size_t, size_t);
		CRJStringByteFlex& Slice(const CRJStringByte&, size_t, size_t);
		CRJStringByteFlex& SliceSafed(const CRJStringByte&, size_t, size_t);
		CRJStringByteFlex& Slice(const CRJStringByte&, size_t);
		CRJStringByteFlex& TrimLeft(uint8_t);
		CRJStringByteFlex& TrimRight(uint8_t);
		CRJStringByteFlex& Trim(uint8_t);
		CRJStringByteFlex& TrimLeft(const CRJStringByte&);
		CRJStringByteFlex& TrimRight(const CRJStringByte&);
		CRJStringByteFlex& Trim(const CRJStringByte&);
		CRJStringByteFlex& TruncLeft(size_t);
		CRJStringByteFlex& TruncRight(size_t);
		CRJStringByteFlex& SetEmpty();
		CRJStringByteFlex& Grow();
		CRJStringByteFlex& ForwardSafed(size_t);
		bool IsFlexUndefined() const;
		CRJStringByteFlex& SetLengthSafed(size_t);
		bool HasField(uint8_t, CRJStringByteFlex&);
		bool WasFoundTopAndMovedHead2Var(uint8_t, CRJStringByteVar&);
		bool WasFoundTopAndMovedTail2Var(uint8_t, CRJStringByteVar&);
		bool WasFoundTopAndMovedHead(uint8_t, CRJStringByteFlex&);
		CRJStringByteFlex& FindTopAndMoveHead(uint8_t, CRJStringByteFlex&);
		bool WasFoundTopAndMovedTail(uint8_t, CRJStringByteFlex&);
		bool WasFoundBackAndMovedTail2Var(uint8_t, CRJStringByteVar&);
		bool WasFoundBackAndMovedHead2Var(uint8_t, CRJStringByteVar&);
		bool WasFoundBackAndMovedTail(uint8_t, CRJStringByteFlex&);
		bool WasFoundBackAndMovedHead(uint8_t, CRJStringByteFlex&);
		bool WasFoundTopAndRemovedHead(uint8_t);
		bool WasFoundTopAndRemovedTail(uint8_t);
		bool WasFoundBackAndRemovedHead(uint8_t);
		bool WasFoundBackAndRemovedTail(uint8_t);
	protected:
		CRJStringByteFlex& SetFlexUndefined() {
			SetConstantUndefined();
			return *this;
		}
	};
	class RJBASE_API CRJStringByteFind:public CRJStringByteFlex {
	public:
		using CRJStringByteFlex::CRJStringByteFlex;
		CRJStringByteFind& Last() {
			bLast=true;
			return *this;
		}
		size_t lIndex=0;
		bool bLast=false;
	};
}

