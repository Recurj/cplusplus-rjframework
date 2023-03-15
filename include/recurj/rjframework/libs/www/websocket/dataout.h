#pragma once
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/www/websocket/defs.h>
#include <recurj/rjframework/libs/www/json/builder.h>
namespace rjWWW {
	class RJWWW_API CWebSocketDataOut:public CJSONBuilder {
	public:
		CWebSocketDataOut();
		CWebSocketDataOut(uint8_t,const uint8_t*, size_t);
		virtual~CWebSocketDataOut();
		CJSONBuilder& Open();
		CRJStringByte& Close(uint8_t ch=CWebSocketDefs::eUWebSocketOpcodes::webSocketOpCodeText);
		bool Build(uint8_t,const uint8_t*, size_t,bool bFin=true);
		bool Build(uint8_t,const CRJStringByte&,bool bFin=true);
		bool BuildCloseData(int16_t,const CRJStringByte&);
		const CRJStringByte& GetNetData() const {
			return m_clTemp;
		}
	protected:
		CRJStringByteFlex m_clTemp;
	};
}

