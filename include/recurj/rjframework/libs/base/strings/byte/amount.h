#pragma once
#include <recurj/rjframework/libs/base/strings/byte/var.h>
namespace rjBase {
	class RJBASE_API CRJStringByteAmount:public CRJStringByteVarBuffer<cSizeAmount> {
	public:
		CRJStringByteAmount();
		virtual~CRJStringByteAmount();
		CRJStringByteVar& FormatCounter(CRJStringByteVar&);
		CRJStringByteVar& FormatDecimal(CRJStringByteVar&, size_t lPrecision=2);
		CRJStringByteVar& FormatCounterMe();
		CRJStringByteVar& FormatDecimalMe(size_t lPrecision=2);
		CRJStringByteVar& FormatCounterNoSign(const CRJStringByte&, size_t,char);
		CRJStringByteVar& FormatDecimalNoSign(const CRJStringByte&,char,char, size_t lPrecision=2);
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringByteAmount)
	};
}

