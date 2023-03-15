#pragma once
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/www/websocket/defs.h>
#include <recurj/rjframework/modules/modwww.h>
namespace rjWWW {
	class RJWWW_API CWebSocketDataIn:public CRJStringByteExpanded {
	public:
		CWebSocketDataIn();
		virtual~CWebSocketDataIn();
		void AddWebSocketData(const uint8_t*, size_t);
		void OnWebSocketText(const uint8_t*, size_t);
		void OnWebSocketBinary(const uint8_t*, size_t);
		void WebSocketReset(uint8_t ch=CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodeContinue);
		inline bool IsBinary() const {
			return m_bOpCode==CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodeBinary;
		}
	protected:
		uint8_t m_bOpCode;
	};
}

