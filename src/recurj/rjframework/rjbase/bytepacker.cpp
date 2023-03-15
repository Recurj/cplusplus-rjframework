#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/bytepacker.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
using namespace rjBase;
const uint16_t CRJBytePacker::s_clMask=0x2A2A;
const uint8_t CRJBytePacker::s_uHiBytes[256]= {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0x00, 0x10, 0x20,
	0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90,0,0,0, 0x0D0,0,0,0, 0x0A0, 0x0B0, 0x0C0,
	0x0D0, 0x0E0, 0x0F0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0, 0x0A0, 0x0B0, 0x0C0, 0x0D0, 0x0E0, 0x0F0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
};
const uint8_t CRJBytePacker::s_uLowBytes[256]= {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0x00, 0x01, 0x02,
	0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,0,0,0, 0x0D,0,0,0, 0x0A, 0x0B, 0x0C,
	0x0D, 0x0E, 0x0F,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0
};
const char* CRJBytePacker::s_clChars[256]= {
	"00","01","02","03","04","05","06","07","08","09","0A","0B","0C","0D","0E","0F","10",
	"11","12","13","14","15","16","17","18","19","1A","1B","1C","1D","1E","1F","20","21",
	"22","23","24","25","26","27","28","29","2A","2B","2C","2D","2E","2F","30","31","32",
	"33","34","35","36","37","38","39","3A","3B","3C","3D","3E","3F","40","41","42","43",
	"44","45","46","47","48","49","4A","4B","4C","4D","4E","4F","50","51","52","53","54",
	"55","56","57","58","59","5A","5B","5C","5D","5E","5F","60","61","62","63","64","65",
	"66","67","68","69","6A","6B","6C","6D","6E","6F","70","71","72","73","74","75","76",
	"77","78","79","7A","7B","7C","7D","7E","7F","80","81","82","83","84","85","86","87",
	"88","89","8A","8B","8C","8D","8E","8F","90","91","92","93","94","95","96","97","98",
	"99","9A","9B","9C","9D","9E","9F","A0","A1","A2","A3","A4","A5","A6","A7","A8","A9",
	"AA","AB","AC","AD","AE","AF","B0","B1","B2","B3","B4","B5","B6","B7","B8","B9","BA",
	"BB","BC","BD","BE","BF","C0","C1","C2","C3","C4","C5","C6","C7","C8","C9","CA","CB",
	"CC","CD","CE","CF","D0","D1","D2","D3","D4","D5","D6","D7","D8","D9","DA","DB","DC",
	"DD","DE","DF","E0","E1","E2","E3","E4","E5","E6","E7","E8","E9","EA","EB","EC","ED",
	"EE","EF","F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","FA","FB","FC","FD","FE",
	"FF"
};
#define _HEXPAIR {ind=lValue&0xFF;*((uint16_t *)pDst)=*((uint16_t *)CRJBytePacker::s_clChars[ind]);pDst-=2;lValue>>=8;}
#define _HEXPAIR_LAST {ind=lValue&0xFF;*((uint16_t *)pDst)=*((uint16_t *)CRJBytePacker::s_clChars[ind]);}
#define _PACKBYTE ch = CRJBytePacker::s_uHiBytes[*src];\
	src++;\
	*dst = ch | CRJBytePacker::s_uLowBytes[(*src)];

 void CRJBytePacker::HexStringLong(size_t lValue, uint8_t* pDst) {
	size_t ind;
	pDst+=6;
	_HEXPAIR;
	_HEXPAIR;
	_HEXPAIR;
	_HEXPAIR_LAST;
}
void CRJBytePacker::HexStringLongLong(size_t lValue, uint8_t* pDst) {
	size_t ind;
	pDst+=14;
	_HEXPAIR;
	_HEXPAIR;
	_HEXPAIR;
	_HEXPAIR;
	_HEXPAIR;
	_HEXPAIR;
	_HEXPAIR;
	_HEXPAIR_LAST;
}
void CRJBytePacker::UnpackReadable(uint8_t* dst,const uint8_t* src, size_t l) {
	while (l>0) {
		*((uint16_t*) dst)=*((uint16_t*) CRJBytePacker::s_clChars[* src]);
		dst[2]=' ';
		dst+=3;
		l--;
		src++;
	}
}
void CRJBytePacker::UnpackMachine(uint8_t* dst,const uint8_t* src, size_t l) {
	while (l>0) {
		*((uint16_t*) dst)=*((uint16_t*) CRJBytePacker::s_clChars[* src]);
		dst+=2;
		l--;
		src++;
	}
}
void CRJBytePacker::UnpackWithHiddenReadable(uint8_t* dst,const uint8_t* src, size_t l) {
	while (l>0) {
		if (* src=='*')*((uint16_t*) dst)=CRJBytePacker::s_clMask;
		else *((uint16_t*) dst)=*((uint16_t*) CRJBytePacker::s_clChars[* src]);
		dst[2]=' ';
		dst+=3;
		l--;
		src++;
	}
}
void CRJBytePacker::UnpackWithHiddenMachine(uint8_t* dst,const uint8_t* src, size_t l) {
	while (l>0) {
		if (* src=='*')*((uint16_t*) dst)=CRJBytePacker::s_clMask;
		else *((uint16_t*) dst)=*((uint16_t*) CRJBytePacker::s_clChars[* src]);
		dst+=2;
		l--;
		src++;
	}
}
void CRJBytePacker::PackReadable(uint8_t* dst,const uint8_t* src, size_t l) {

	uint8_t ch;
	for (size_t i = 0; i < l; i++, dst++, src++) {
		_PACKBYTE; 
		src++;
	}
}
void CRJBytePacker::PackMachine(uint8_t* dst,const uint8_t* src, size_t l) {
	uint8_t ch;
	for (size_t i=0; i<l; i++, dst++, src++) {
		_PACKBYTE;
	}
}
void CRJBytePacker::PackWithHiddenReadable(uint8_t* dst,const uint8_t* src, size_t l) {
	uint8_t ch;
	for (size_t i=0; i<l; i++, dst++, src++) {
		if ((*(uint16_t*) src)==CRJBytePacker::s_clMask) {* dst='*';src++;}
		else {
			_PACKBYTE;
		}
		src++;
	}
}
void CRJBytePacker::PackWithHiddenMachine(uint8_t* dst,const uint8_t* src, size_t l) {
	uint8_t ch;
	for (size_t i=0; i<l; i++, dst++, src++) {
		if ((*(uint16_t*) src)==CRJBytePacker::s_clMask) { *dst = '*'; src++; }
		else {
			_PACKBYTE;
		}
	}
}
size_t CRJBytePacker::Bin2Str(char* pDst, size_t lMax,const uint8_t* pData, size_t lLen) {
	try {
		size_t h=lMax/ 2;
		size_t i=(lLen>=h)?
			(h-1):
			lLen;
		if (pData) UnpackMachine((uint8_t*) pDst,(uint8_t*) pData, i);
		else _MEMCLR_BYTE_POINTER(pDst,(2* i));
		i*=2;
		pDst[i]=0x00;
	}
	catch(...) {}
	return 1;
}
size_t CRJBytePacker::ByteToHex(uint8_t* pDst, uint8_t bInd,bool bAdd,char ch) {
	size_t rc=2;
	try {
		*((uint16_t*) pDst)=*((uint16_t*) CRJBytePacker::s_clChars[bInd]);
		if (bAdd) {
			pDst+=2;
			* pDst=ch;
			rc++;
		}
	}
	catch(...) {}
	return rc;
}

