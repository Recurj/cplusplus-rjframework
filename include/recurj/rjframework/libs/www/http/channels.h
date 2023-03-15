#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/www/http/server.h>
#include <recurj/rjframework/libs/base/application/application.h>
class CRJNetSocketEvent;
namespace rjWWW {
	class CHTTPChannel {
	public:
		enum eUHttpChannelResult:int64_t {
			OK=0, Error=-1, NotLoaded=-2
		};
		using funcCreate=CHTTPChannel*(*)(CRJApplicationLoggerTask&,const CRJStringByte&,const uint8_t*);
		using funcDelete=bool(*)(CHTTPChannel*);
		virtual bool Start()=0;
		virtual bool Call(CHTTPServer&, CRJNetSocketEvent&&
			)=0;
		virtual funcDelete GetEraser()=0;
	};
}

