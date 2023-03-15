#include "stdafx.h"
#include <recurj/rjframework/libs/www/http/formdata.h>
using namespace rjWWW;
CHTTPFormData::CHTTPFormData() {}
CHTTPFormData::~CHTTPFormData() {}
bool CHTTPFormData::Parse(const CRJStringByte& s) {
	auto call=[this](const CRJStringByteFind& p)->bool {
		CRJStringByteFlex k, v;
		return p.Split('=', k, v)?
			emplace(k, v).second:false;
	};
	CRJStringByteFunctorSplitBool<decltype(call)>cb(call);
	return s.SplitResult('&', cb);
}
bool CHTTPFormData::Get(const CRJStringByte& clKey, CRJStringByteFlex& clData) {
	auto ret=find(clKey);
	if (ret!=end()) {
		clData.Set(ret->second);
		return true;
	}
	return false;
}

