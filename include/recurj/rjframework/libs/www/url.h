#pragma once
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/modules/modwww.h>
namespace rjWWW {
	class RJWWW_API CRJURL:public CRJStringByteExpanded {
	public:
		CRJURL();
		virtual~CRJURL();
		CRJStringByteVar& Decode();
	};
}

