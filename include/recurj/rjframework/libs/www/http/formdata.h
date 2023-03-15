#pragma once
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/modules/modwww.h>
#include <map>
namespace rjWWW {
	class RJWWW_API CHTTPData:public CRJStringByteFlex {
	public:
		CHTTPData() {}
		CHTTPData(const CHTTPData& s):
			CRJStringByteFlex((CRJStringByte&) s) {}
		CHTTPData(const CRJStringByte& s):
			CRJStringByteFlex(s) {}
	};
	class RJWWW_API CHTTPFormData:public std::map<CHTTPData, CHTTPData> {
	public:
		CHTTPFormData();
		virtual~CHTTPFormData();
		bool Parse(const CRJStringByte&);
		bool Get(const CRJStringByte&, CRJStringByteFlex&);
	};
}

