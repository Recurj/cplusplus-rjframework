#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/byte/amount.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <locale>
using namespace rjBase;
using namespace rjBase;
CRJStringByteAmount::CRJStringByteAmount() {}
CRJStringByteAmount::~CRJStringByteAmount() {}
CRJStringByteVar& CRJStringByteAmount::FormatCounter(CRJStringByteVar& s) {
	if (s.GetLength()>0) {
		locale loc("");
		uint8_t ch=s[0];
		char chSep=std::use_facet<std::numpunct<char>>(loc).thousands_sep();
		if (ch=='-') {
			CRJStringByteFlex str;
			if (str.Slice(s, 1).TruncLeft(0).IsEmpty()) s.SetSingle('0');
			else s.SetSingle(ch).Cat(FormatCounterNoSign(str, str.GetLength(), chSep));
		}
		else if (ch=='+') {
			CRJStringByteFlex str;
			if (str.Slice(s, 1).TruncLeft(0).IsEmpty()) s.SetSingle('0');
			else s.Set(FormatCounterNoSign(str, str.GetLength(), chSep));
		}
		else s.Set(FormatCounterNoSign(s, s.GetLength(), chSep));
	}
	else s.SetSingle('0');
	return s;
}
CRJStringByteVar& CRJStringByteAmount::FormatDecimal(CRJStringByteVar& s, size_t lPrecision) {
	locale loc("");
	auto& facet=std::use_facet<std::numpunct<char>>(loc);
	char chSepThousands=facet.thousands_sep(),
	chSepPoint=facet.decimal_point();
	if (s.GetLength()>0) {
		uint8_t ch=s[0];
		if (ch=='-') {
			CRJStringByteFlex str;
			if (str.Slice(s, 1).TruncLeft(0).IsEmpty()) s.Set(FormatDecimalNoSign(s.SetEmpty(), chSepThousands, chSepPoint, lPrecision));
			else s.SetSingle(ch).Cat(FormatDecimalNoSign(str, chSepThousands, chSepPoint, lPrecision));
		}
		else if (ch=='+') {
			CRJStringByteFlex str;
			if (str.Slice(s, 1).TruncLeft(0).IsEmpty()) s.Set(FormatDecimalNoSign(s.SetEmpty(), chSepThousands, chSepPoint, lPrecision));
			else s.Set(FormatDecimalNoSign(str, chSepThousands, chSepPoint, lPrecision));
		}
		else s.Set(FormatDecimalNoSign(s, chSepThousands, chSepPoint, lPrecision));
	}
	else s.Set(FormatDecimalNoSign(s, chSepThousands, chSepPoint, lPrecision));
	return s;
}
CRJStringByteVar& CRJStringByteAmount::FormatCounterMe() {
	CRJStringByteAmount am;
	return am.FormatCounter(*this);
}
CRJStringByteVar& CRJStringByteAmount::FormatDecimalMe(size_t lPrecision) {
	CRJStringByteAmount am;
	return am.FormatDecimal(*this, lPrecision);
}
CRJStringByteVar& CRJStringByteAmount::FormatDecimalNoSign(const CRJStringByte& clData,char chSepThousands,char chSepPoint, size_t lPrecision) {
	auto l=clData.GetLength();
	lPrecision&=0x3F;
	if (l>lPrecision) {
		l-=lPrecision;
		return FormatCounterNoSign(clData, l, chSepThousands).CatSingle(chSepPoint).CatData(clData.GetHeadForRead()+l, lPrecision);
	}
	else if (l<lPrecision) return SetSingle('0').CatSingle(chSepPoint).CatFill(lPrecision-l).CatData(clData.GetHeadForRead(), l);
	return SetSingle('0').CatSingle(chSepPoint).CatData(clData.GetHeadForRead(), l);
}
CRJStringByteVar& CRJStringByteAmount::FormatCounterNoSign(const CRJStringByte& clData, size_t l,char chSep) {
	if (!l) return SetSingle('0');
	size_t t=0, k1=l/ 3;
	const uint8_t* pSrc=clData.GetHeadForRead();
	uint8_t* pDst=GetHeadForUpdate();
	if ((l+k1+3)>=GetSize()) return SetLabel("NaN");
	size_t i, k2=l% 3;
	if (k2==1) {
		* pDst=* pSrc;
		pDst[1]=chSep;
		t=2;
		pDst+=2;
		pSrc++;
	}
	else if (k2==2) {
		*((uint16_t*) pDst)=*((uint16_t*) pSrc);
		pDst[2]=chSep;
		t=3;
		pDst+=3;
		pSrc+=2;
	}
	for (i=0; i<k1; i++) {
		*((int32_t*) pDst)=*((int32_t*) pSrc);
		pDst[3]=chSep;
		t+=4;
		pDst+=4;
		pSrc+=3;
	}
	SetLength(t-1);
	return *this;
}

