#pragma once
#include <recurj/rjframework/modules/modwww.h>
namespace rjWWW {
	class RJWWW_API CWebSocketDefs {
	public:
		enum eUWebSocketOpcodes:uint8_t {
			webSocketOpCodeContinue=0, webSocketOpCodeText=1, webSocketOpCodeBinary=2, webSocketOpCodeClose=8,
			webSocketOpCodePing=9, webSocketOpCodePong=10,
		};
		enum eUWebSocketCloseCodes:int32_t {
			webSocketCloseReasonNormal=1000, webSocketCloseReasonShutdown=1001, webSocketCloseReasonProtocolError=1002,
			webSocketCloseReasonBadData=1003, webSocketCloseReasonInvalidFrame=1007, webSocketCloseReasonBadPolice=1008,
			webSocketCloseReasonTooBig=1009, webSocketCloseReasonNonMandatory=1010, webSocketCloseReasonInternalError=1011,
			webSocketCloseReasonRestart=1012, webSocketCloseReasonTryAgain=1013,
		};
	};
}

