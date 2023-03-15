#pragma once
#include <recurj/rjframework/modules/defs/moddefbase.h>
namespace rjBase {
	class CRJStringByteVar;
	class RJBASE_API IRJNamed {
	public:
		virtual CRJStringByteVar& GetName(CRJStringByteVar&)=0;
		IRJNamed& GetNamedMe() {
			return *this;
		}
	};
}

