#pragma once
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/errorcodes.h>
namespace rjBase {
	class RJBASE_API CRJStringError:public CRJStringByteExpanded {
	public:
		CRJStringError();
		virtual~CRJStringError();
		CRJStringByte& OS(int64_t, int64_t);
		CRJStringByte& Warning(const CRJStringByte&);
		CRJStringByte& Error(const CRJStringByte&);
		CRJStringByte& Error(int64_t);
		CRJStringByte& ErrorDetails(int64_t,const CRJStringByte&);
		static const CRJStringByte s_Warning;
		static const CRJStringByte s_Error;
		static const CRJStringByte s_ErrorOs;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringError)
	};
}

