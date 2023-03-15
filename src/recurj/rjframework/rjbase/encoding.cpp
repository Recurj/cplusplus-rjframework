#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/unicode/allocated.h>
#include <recurj/rjframework/libs/base/strings/unicode/flex.h>
using namespace rjBase;
const wchar_t CRJEncoding::s_uUnicodTable1251[256]= {
	0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A,
	0x000B, 0x000C, 0x000D, 0x000E, 0x000F, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015,
	0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F, 0x0020,
	0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B,
	0x002C, 0x002D, 0x002E, 0x002F, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036,
	0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F, 0x0040, 0x0041,
	0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C,
	0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
	0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F, 0x0060, 0x0061, 0x0062,
	0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D,
	0x006E, 0x006F, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078,
	0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F, 0x0402, 0x0403, 0x201A, 0x0453,
	0x201E, 0x2026, 0x2020, 0x2021, 0x20AC, 0x2030, 0x0409, 0x2039, 0x040A, 0x040C, 0x040B,
	0x040F, 0x0452, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x0098, 0x2122,
	0x0459, 0x203A, 0x045A, 0x045C, 0x045B, 0x045F, 0x00A0, 0x040E, 0x045E, 0x0408, 0x00A4,
	0x0490, 0x00A6, 0x00A7, 0x0401, 0x00A9, 0x0404, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x0407,
	0x00B0, 0x00B1, 0x0406, 0x0456, 0x0491, 0x00B5, 0x00B6, 0x00B7, 0x0451, 0x2116, 0x0454,
	0x00BB, 0x0458, 0x0405, 0x0455, 0x0457, 0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415,
	0x0416, 0x0417, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F, 0x0420,
	0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, 0x0428, 0x0429, 0x042A, 0x042B,
	0x042C, 0x042D, 0x042E, 0x042F, 0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436,
	0x0437, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F, 0x0440, 0x0441,
	0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, 0x0448, 0x0449, 0x044A, 0x044B, 0x044C,
	0x044D, 0x044E, 0x044F
};
const size_t CRJEncoding::s_tabUTF8Length[256]= {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5,
	6, 6, 0, 0,
};
CRJEncoding::funcUtf8Pack CRJEncoding::f_tabUtf8Unpack[8]= {
	& CRJEncoding::Utf8UnPack0,& CRJEncoding::Utf8UnPack0,& CRJEncoding::Utf8UnPack2,
	& CRJEncoding::Utf8UnPack3,& CRJEncoding::Utf8UnPack4,& CRJEncoding::Utf8UnPack5,
	& CRJEncoding::Utf8UnPack6,& CRJEncoding::Utf8UnPack0
};
wchar_t CRJEncoding::Utf8UnPack0(const uint8_t*) {
	return 0;
}
wchar_t CRJEncoding::Utf8UnPack2(const uint8_t* pData) {
	size_t f0=(size_t)((pData[1]& 0x3F)|((pData[0]<<6)& 0xC0));
	size_t f1=(size_t)((pData[0]>>2)& 0x07);
	f1=(f1<<8)& 0x0FF00;
	return (wchar_t)(f0| f1);
}
wchar_t CRJEncoding::Utf8UnPack3(const uint8_t* pData) {
	size_t f0=(size_t)((pData[2]& 0x3F)|((pData[1]<<6)& 0xC0));
	size_t f1=(size_t)(((pData[1]>>2)& 0x0F)|((pData[0]<<4)& 0xF0));
	f1=(f1<<8)& 0x0FF00;
	return (wchar_t)(f0| f1);
}
wchar_t CRJEncoding::Utf8UnPack4(const uint8_t* pData) {
	size_t f0=(size_t)((pData[3]& 0x3F)|((pData[2]<<6)& 0xC0));
	size_t f1=(size_t)((((pData[2]>>2)& 0x0F)|(pData[1]<<4)& 0xF0));
	size_t f2=(size_t)((((pData[1]>>4)& 0x03)|(pData[0]<<2)& 0x1C));
	f1=(f1<<8)& 0x0FF00;
	f2=(f2<<16)& 0x0FF0000;
	return (wchar_t)(f0| f1| f2);
}
wchar_t CRJEncoding::Utf8UnPack5(const uint8_t* pData) {
	size_t f0=(size_t)((pData[4]& 0x3F)|((pData[3]<<6)& 0xC0));
	size_t f1=(size_t)((((pData[3]>>2)& 0x0F)|(pData[2]<<4)& 0xF0));
	size_t f2=(size_t)((((pData[2]>>4)& 0x03)|(pData[1]<<2)& 0xFC));
	size_t f3=(size_t)(pData[0]& 0x03);
	f1=(f1<<8)& 0x0FF00;
	f2=(f2<<16)& 0x0FF0000;
	f3=(f3<<24)& 0x0FF000000;
	return (wchar_t)(f0| f1| f2| f3);
}
wchar_t CRJEncoding::Utf8UnPack6(const uint8_t* pData) {
	size_t f0=(size_t)((pData[5]& 0x3F)|((pData[4]<<6)& 0xC0));
	size_t f1=(size_t)((((pData[4]>>2)& 0x0F)|(pData[3]<<4)& 0xF0));
	size_t f2=(size_t)((((pData[3]>>4)& 0x03)|(pData[2]<<2)& 0xFC));
	size_t f3=(size_t)(((pData[1]& 0x03F)|(pData[0]<<6)& 0x40));
	f1=(f1<<8)& 0x0FF00;
	f2=(f2<<16)& 0x0FF0000;
	f3=(f3<<24)& 0x0FF000000;
	return (wchar_t)(f0| f1| f2| f3);
}
CRJStringUnicodeVar& CRJEncoding::translateCodePageToUnicode(const CRJStringByte& clSrc, CRJStringUnicodeVar& d, size_t lPage) {
	if (lPage!=CP_CYRILLIC1251) return d.SetEmpty().SetStringVarError(CRJChain::eUChainErrors::Decode);
	try {
		size_t l=clSrc.GetLength();
		if (l>0) {
			if (!d.HasNeededSize(l)) return d.SetEmpty().SetStringVarError(CRJChain::eUChainErrors::NoSpace);
			else {
				wchar_t* pOut=d.GetHeadForUpdate();
				const uint8_t* pIn=clSrc;
				for (size_t i=0; i<l; i++, pIn++, pOut++) {
					pOut[0]=CRJEncoding::s_uUnicodTable1251[pIn[0]& 0xFF];
				}
				d.SetLength(l);
			}
		}
		else d.SetEmpty();
	}
	catch(...) {}
	return d;
}
CRJStringByteVar& CRJEncoding::translateUnicodeToCodePage(const CRJStringUnicode& clSrc, CRJStringByteVar& d, size_t lPage) {
	if (lPage!=CP_CYRILLIC1251) return d.SetEmpty().SetStringVarError(CRJChain::eUChainErrors::Decode);
	try {
		size_t l=clSrc.GetLength();
		if (l>0) {
			if (!d.HasNeededSize(l)) return d.SetEmpty().SetStringVarError(CRJChain::eUChainErrors::NoSpace);
			wchar_t ch;
			uint8_t* pOut=d.GetHeadForUpdate();
			const wchar_t* pIn=clSrc;
			for (size_t i=0; i<l; i++, pIn++, pOut++) {
				ch=pIn[0];
				if (ch>=cUUnicode1251First&&
					ch<=cUUnicode1251Last) pOut[0]=(uint8_t)(ch-cUUnicode1251First+cUCodePage1251First);
				else if (ch==cUUnicode1251SpecialUpperCase) pOut[0]=cUCodePage1251SpecialUpperCase;
				else if (ch==cUUnicode1251SpecialLowerCase) pOut[0]=cUCodePage1251SpecialLowerCase;
				else pOut[0]=(uint8_t)::wctob(ch);
			}
			d.SetLength(l);
		}
		else d.SetEmpty();
	}
	catch(...) {}
	return d;
}
CRJStringByteVar& CRJEncoding::translateCodePageToUtf8(const CRJStringByte& clSrc, CRJStringByteVar& d, size_t lPage) {
	if (clSrc.GetLength()>0) {
		CRJStringUnicodeExpanded clTmp;
		if (CRJEncoding::translateCodePageToUnicode(clSrc, clTmp, lPage).OK()) return CRJEncoding::translateUnicodeToUtf8(clTmp, d);
	}
	return d.SetEmpty();
}
bool CRJEncoding::translateUtf8ToCodePage(const CRJStringByteFlex& stSrc, CRJStringByteFlex& stDst, size_t lPage) {
	try {
		CRJStringByteFlex clSrc(stSrc);
		CRJStringByteVarAssigned clDst(stDst);
		if (CRJEncoding::translateUtf8ToCodePage(clSrc, clDst, lPage).OK()) {
			stDst.Set(clDst);
			return true;
		}
	}
	catch(...) {}
	return false;
}
CRJStringByteVar& CRJEncoding::translateUtf8ToCodePage(const CRJStringByte& clSrc, CRJStringByteVar& d, size_t lPage) {
	if (clSrc.GetLength()>0) {
		CRJStringUnicodeExpanded clTmp;
		if (CRJEncoding::translateUtf8ToUnicode(clSrc, clTmp).OK()) return CRJEncoding::translateUnicodeToCodePage(clTmp, d, lPage);
	}
	return d.SetEmpty();
}
CRJStringUnicodeVar& CRJEncoding::translateUtf8ToUnicode(const uint8_t* pSrc, size_t lLen, CRJStringUnicodeVar& d) {
	CRJStringByteFlex clSrc(pSrc, lLen);
	return CRJEncoding::translateUtf8ToUnicode(clSrc, d);
}
CRJStringByteVar& CRJEncoding::translateUnicodeToUtf8(const wchar_t* pSrc, size_t lLen, CRJStringByteVar& d) {
	try {
		CRJStringUnicodeFlex clSrc(pSrc, lLen);
		return CRJEncoding::translateUnicodeToUtf8(clSrc, d);
	}
	catch(...) {}
	return d.SetStringVarError(CRJChain::eUChainErrors::Exception);
}
CRJStringUnicodeVar& CRJEncoding::translateUtf8ToUnicode(const CRJStringByte& s, CRJStringUnicodeVar& d) {
	size_t lSpace=0;
	if (!calcUtf8ToUnicode(s, lSpace)||
		!d.HasNeededSize(lSpace)) return d.SetStringVarError(CRJChain::eUChainErrors::Decode);
	size_t r, v, ls=s.GetLength(),
	ld=d.GetSize();
	const uint8_t* pSrc=s.GetHeadForRead();
	wchar_t* pDst=d.GetHeadForUpdate();
	while (ls>0&&
		ld>0) {
		v=pSrc[0];
		r=CRJEncoding::s_tabUTF8Length[v& 0x0FF];
		if (r==1)* pDst=(wchar_t) v;
		else if (!r||
			ls<r) return d.SetStringVarError(CRJChain::eUChainErrors::Decode);
		else * pDst=(* CRJEncoding::f_tabUtf8Unpack[r])(pSrc);
		pDst++;
		ld--;
		if (ls<r) return d.SetStringVarError(CRJChain::eUChainErrors::Decode);
		ls-=r;
		pSrc+=r;
	}
	return (!ls)?
		d.SetLengthSafed(d.GetSize()-ld):
		d.SetStringVarError(CRJChain::eUChainErrors::Decode);
}
CRJStringByteVar& CRJEncoding::translateUnicodeToUtf8(const CRJStringUnicode& s, CRJStringByteVar& d) {
	if (!d.HasNeededSize(calcUnicodeToUtf8(s))) return d.SetStringVarError(CRJChain::eUChainErrors::Decode);
	size_t r, ls=s.GetLength(),
	ld=d.GetSize();
	const wchar_t* pSrc=s.GetHeadForRead();
	uint8_t* pDst=d.GetHeadForUpdate();
	while (ls>0&&
		ld>0) {
		r=pSrc[0];
		pSrc++;
		ls--;
		if (r<0x7F) {
			* pDst=(uint8_t) r;
			pDst++;
			ld--;
		}
		else if (r<0x07FF) {
			if (ld<2) return d.SetStringVarError(CRJChain::eUChainErrors::Decode);
			ld-=2;
			* pDst=(uint8_t)(0b11000000|((r>>6)& 0x1F));
			pDst++;
			* pDst=(uint8_t)(0b10000000|(r& 0x3F));
			pDst++;
		}
		else if (r<0x0FFFF) {
			if (ld<3) return d.SetStringVarError(CRJChain::eUChainErrors::Decode);
			ld-=3;
			* pDst=(uint8_t)(0b11100000|((r>>12)& 0x0F));
			pDst++;
			* pDst=(uint8_t)(0b10000000|((r>>6)& 0x3F));
			pDst++;
			* pDst=(uint8_t)(0b10000000|(r& 0x3F));
			pDst++;
		}
		else if (r<0x01FFFFF) {
			if (ld<4) return d.SetStringVarError(CRJChain::eUChainErrors::Decode);
			ld-=4;
			* pDst=(uint8_t)(0b11110000|((r>>18)& 0x07));
			pDst++;
			* pDst=(uint8_t)(0b10000000|((r>>12)& 0x3F));
			pDst++;
			* pDst=(uint8_t)(0b10000000|((r>>6)& 0x3F));
			pDst++;
			* pDst=(uint8_t)(0b10000000|(r& 0x3F));
			pDst++;
		}
		else if (r<0x03FFFFFF) {
			if (ld<5) return d.SetStringVarError(CRJChain::eUChainErrors::Decode);
			ld-=5;
			* pDst=(uint8_t)(0b11111000|((r>>24)& 0x03));
			pDst++;
			* pDst=(uint8_t)(0b10000000|((r>>16)& 0x3F));
			pDst++;
			* pDst=(uint8_t)(0b10000000|((r>>12)& 0x3F));
			pDst++;
			* pDst=(uint8_t)(0b10000000|((r>>6)& 0x3F));
			pDst++;
			* pDst=(uint8_t)(0b10000000|(r& 0x3F));
			pDst++;
		}
		else {
			if (ld<6) return d.SetStringVarError(CRJChain::eUChainErrors::Decode);
			ld-=6;
			* pDst=(uint8_t)(0b11111100|((r>>30)& 0x01));
			pDst++;
			* pDst=(uint8_t)(0b10000000|((r>>24)& 0x3F));
			pDst++;
			* pDst=(uint8_t)(0b10000000|((r>>18)& 0x3F));
			pDst++;
			* pDst=(uint8_t)(0b10000000|((r>>12)& 0x3F));
			pDst++;
			* pDst=(uint8_t)(0b10000000|((r>>6)& 0x3F));
			pDst++;
			* pDst=(uint8_t)(0b10000000|(r& 0x3F));
			pDst++;
		}
	}
	return (!ls)?
		d.SetLengthSafed(d.GetSize()-ld):
		d.SetStringVarError(CRJChain::eUChainErrors::Decode);
}
size_t CRJEncoding::calcUnicodeToUtf8(const CRJStringUnicode& s) {
	size_t r, ls=s.GetLength(),
	c=0;
	const wchar_t* pSrc=s.GetHeadForRead();
	while (ls>0) {
		r=pSrc[0];
		pSrc++;
		ls--;
		if (r<0x7F) c++;
		else if (r<0x07FF) c+=2;
		else if (r<0x0FFFF) c+=3;
		else if (r<0x01FFFFF) c+=4;
		else if (r<0x03FFFFFF) c+=5;
		else c+=6;
	}
	return c;
}
bool CRJEncoding::calcUtf8ToUnicode(const CRJStringByte& s, size_t& space) {
	size_t c=0;
	size_t r, v, ls=s.GetLength();
	const uint8_t* pSrc=s.GetHeadForRead();
	while (ls>0) {
		v=pSrc[0];
		c++;
		r=CRJEncoding::s_tabUTF8Length[v& 0x0FF];
		if (r==0||
			ls<r) return false;
		ls-=r;
		pSrc+=r;
	}
	space=c;
	return true;
}

