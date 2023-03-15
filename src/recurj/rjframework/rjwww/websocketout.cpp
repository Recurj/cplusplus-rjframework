#include "stdafx.h"
#include <recurj/rjframework/libs/www/websocket/dataout.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
using namespace rjWWW;
CWebSocketDataOut::CWebSocketDataOut() {}
CWebSocketDataOut::CWebSocketDataOut(uint8_t ch,const uint8_t* pSrc, size_t l) {
	Build(ch, pSrc, l,true);
}
CWebSocketDataOut::~CWebSocketDataOut() {}
CJSONBuilder& CWebSocketDataOut::Open() {
	SetLength(10);
	return *this;
}
CRJStringByte& CWebSocketDataOut::Close(uint8_t ch) {
	size_t l=GetLength();
	uint8_t* pData=(uint8_t*) GetHeadForRead();
	if (l<10) return m_clTemp.SetEmpty();
	l-=10;
	ch|=0x80;
	if (l<126) {
		pData+=8;
		pData[0]=ch;
		pData[1]=(uint8_t) l;
		l+=2;
	}
	else if (l>0x00FFFF) {
		pData[0]=ch;
		pData[1]=(uint8_t) 127;
		*((size_t*)& pData[2])=CRJByteConvertor::BigEndianSet64(l);
		l+=10;
	}
	else {
		pData+=6;
		pData[0]=ch;
		pData[1]=(uint8_t) 126;
		*((uint16_t*)& pData[2])=CRJByteConvertor::BigEndianSet16((uint16_t) l);
		l+=4;
	}
	return m_clTemp.SetFromData(pData, l);
}
bool CWebSocketDataOut::Build(uint8_t ch,const CRJStringByte& s,bool bFin) {
	return Build(ch, s.GetHeadForRead(), s.GetLength(), bFin);
}
bool CWebSocketDataOut::Build(uint8_t ch,const uint8_t* pSrc, size_t l,bool bFin) {
	if (bFin) ch|=0x80;
	uint8_t* pData=GetHeadForUpdate();
	pData[0]=ch;
	if (l<126) {
		pData[1]=(uint8_t) l;
		SetLengthSafed(2);
	}
	else if (l>216) {
		pData[1]=(uint8_t) 127;
		*((uint64_t*)& pData[2])=CRJByteConvertor::BigEndianSet64(l);
		SetLengthSafed(10);
	}
	else {
		pData[1]=(uint8_t) 126;
		*((uint16_t*)& pData[2])=CRJByteConvertor::BigEndianSet16((uint16_t) l);
		SetLengthSafed(4);
	}
	return CatData(pSrc, l).OK();
}
bool CWebSocketDataOut::BuildCloseData(int16_t lReason,const CRJStringByte& s) {
	uint8_t* pData=GetHeadForUpdate();
	*((uint16_t*) pData)=CRJByteConvertor::BigEndianSet16((uint16_t) lReason);
	return SetLengthSafed(2).Cat(s).OK();
}

