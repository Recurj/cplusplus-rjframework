#include "stdafx.h"
#include <recurj/rjframework/libs/www/websocket/datain.h>
using namespace rjWWW;
CWebSocketDataIn::CWebSocketDataIn() {
	WebSocketReset(CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodeContinue);
}
CWebSocketDataIn::~CWebSocketDataIn() {}
void CWebSocketDataIn::WebSocketReset(uint8_t ch) {
	m_bOpCode=ch;
	SetEmpty();
}
void CWebSocketDataIn::AddWebSocketData(const uint8_t* pData, size_t l) {
	if (m_bOpCode!=CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodeContinue) {
		CatData(pData, l);
	}
}
void CWebSocketDataIn::OnWebSocketText(const uint8_t* pData, size_t l) {
	WebSocketReset(CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodeText);
	SetData(pData, l);
}
void CWebSocketDataIn::OnWebSocketBinary(const uint8_t* pData, size_t l) {
	WebSocketReset(CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodeBinary);
	SetData(pData, l);
}

