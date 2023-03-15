#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/www/websocket/defs.h>
#include <recurj/rjframework/libs/www/websocket/connection.h>
#include <recurj/rjframework/libs/www/websocket/dataout.h>
using namespace rjWWW;
CWebSocketConnection::CWebSocketConnection() {
	WebSocketReset();
}
CWebSocketConnection::~CWebSocketConnection() {}
void CWebSocketConnection::WebSocketReset() {
	m_bLocalClosed=m_bRemoteClosed=false;
	m_clMessage.WebSocketReset();
}
void CWebSocketConnection::WebSocketRelease() {}
bool CWebSocketConnection::WebSocketSend(const CRJStringByte&) {
	return false;
}
bool CWebSocketConnection::WebSocketClose(int16_t lReason,const CRJStringByte& s) {
	if (m_bLocalClosed) return true;
	CWebSocketDataOut clOut, clData;
	if (!clData.BuildCloseData(lReason, s)) return false;
	if (clOut.Build(CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodeClose, clData)) m_bLocalClosed=WebSocketSend(clOut);
	return m_bLocalClosed;
}
void CWebSocketConnection::OnCWebSocketRecv() {
	CRJByteStorageClearner clCleaner(*this);
	while (GetNetMessage());
}
bool CWebSocketConnection::GetNetMessage() {
	size_t w=GetNotProcessedSize();
	if (w>1) {
		uint8_t* bData=(uint8_t*) GetNotProcessedHead();
		size_t lHeader=2;
		size_t lLen=(size_t)(bData[1]& 0x7F);
		if (lLen<126);
		else if (lLen==126) {
			if (w<4) return false;
			lLen=CRJByteConvertor::BigEndianSet16(*((uint16_t*)& bData[2]));
			lHeader+=2;
		}
		else if (w<10) return false;
		else {
			lLen=*((size_t*)& bData[2]);
			lLen=CRJByteConvertor::BigEndianSet64(lLen);
			lHeader+=8;
		}
		if (bData[1]& 0x80) {
			size_t s=lHeader+lLen+4;
			if (w<s) return false;
			UnMask(& bData[lHeader], lLen);
			Processed(s);
			OnWebSocketData(bData[0],& bData[lHeader+4], lLen);
			return true;
		}
		else {
			size_t s=lHeader+lLen;
			if (w<s) return false;
			Processed(s);
			OnWebSocketData(bData[0],& bData[lHeader+4], lLen);
			return true;
		}
	}
	return false;
}
void CWebSocketConnection::UnMask(uint8_t* bData, size_t lLen) {
	const uint8_t* bMask=bData;
	size_t f=lLen>>2;
	bData+=4;
	while (f>0) {
		f--;
		*((uint32_t*) bData)^=*((uint32_t*) bMask);
		bData+=4;
	}
	f=lLen& 3;
	while (f>0) {
		f--;
		* bData^=* bMask;
		bData++;
		bMask++;
	}
}
void CWebSocketConnection::OnWebSocketText() {}
void CWebSocketConnection::OnWebSocketBinary() {}
void CWebSocketConnection::OnWebSocketData(uint8_t bCode,const uint8_t* bData, size_t lLen) {
	switch (bCode& 0x0F) {
	case CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodeContinue:m_clMessage.AddWebSocketData(bData, lLen);
		break;
	case CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodeText:m_clMessage.OnWebSocketText(bData, lLen);
		break;
	case CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodeBinary:m_clMessage.OnWebSocketBinary(bData, lLen);
		break;
	case CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodePing:OnWebSocketPing(bData, lLen);
		return ;
	case CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodePong: return ;
	case CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodeClose:OnWebSocketClose(bData, lLen);
		return ;
	default:
		return ;
	}
	if (bCode& 0x80&&
		m_clMessage.OK()) {
		if (m_bLocalClosed||
			m_bRemoteClosed);
		else if (m_clMessage.IsBinary()) OnWebSocketBinary();
		else OnWebSocketText();
	}
}
void CWebSocketConnection::OnWebSocketClose(const uint8_t* bData, size_t lLen) {
	m_bRemoteClosed=true;
	if (m_bLocalClosed) WebSocketRelease();
	else {
		CWebSocketDataOut clOut(CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodeClose, bData, lLen);
		if (clOut.OK()) WebSocketSend(clOut);
	}
}
void CWebSocketConnection::OnWebSocketPing(const uint8_t* bData, size_t lLen) {
	CWebSocketDataOut clOut(CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodePong, bData, lLen);
	if (clOut.OK()) WebSocketSend(clOut);
}

