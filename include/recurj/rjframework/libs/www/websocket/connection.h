#pragma once
#include <recurj/rjframework/libs/base/datatools/bytestorage.h>
#include <recurj/rjframework/libs/www/websocket/datain.h>
namespace rjWWW {
	class RJWWW_API CWebSocketConnection:public CRJByteStorage {
		CWebSocketConnection();
		virtual~CWebSocketConnection();
		void OnCWebSocketRecv();
		virtual void WebSocketReset();
		virtual void WebSocketRelease();
		virtual bool WebSocketSend(const CRJStringByte&);
		bool WebSocketClose(int16_t,const CRJStringByte&);
	protected:
		virtual void OnWebSocketText();
		virtual void OnWebSocketBinary();
		bool GetNetMessage();
		void OnWebSocketData(uint8_t,const uint8_t*, size_t);
		void OnWebSocketClose(const uint8_t*, size_t);
		void OnWebSocketPing(const uint8_t*, size_t);
		void UnMask(uint8_t*, size_t);
	protected:
		CWebSocketDataIn m_clMessage;
		bool m_bLocalClosed, m_bRemoteClosed;
	};
	template<size_t N>
	class CWebSocketReceiver:public CWebSocketConnection {
	public:
		CWebSocketReceiver() {
			Restart(m_bDataBlock,sizeof(m_bDataBlock));
		}
		virtual~CWebSocketReceiver() {}
	protected:
		uint8_t m_bDataBlock[N];
	};
}

