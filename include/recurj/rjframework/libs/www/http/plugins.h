#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/www/http/server.h>
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/base/application/application.h>
namespace rjWWW {
	class CHTTPPlugin {
	public:
		enum eUHttpPluginResult:int64_t {
			OK=0, NotLoaded=1, Error=2, BadFormat=3, BadContent=4, MustReLogin=5, ReplySystemErr=6,
			ModuleErrors=1024,
		};
		using funcDo=int64_t(*)(CHTTPServer&);
		using funcInit=bool(*)(CRJApplicationLoggerTask&,const CRJStringByte&,const uint8_t*);
	};
}

