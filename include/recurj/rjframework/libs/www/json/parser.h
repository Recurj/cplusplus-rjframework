#pragma once
#include <recurj/rjframework/functors.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/modules/modwww.h>
namespace rjWWW {
	using IJSONParser=IRJFunctorBool<CRJStringByteFlex&>;
	using IJSONVar=IRJFunctorBool<CRJStringByteFlex&, CRJStringByteFlex&>;
	using IJSONArray=IRJFunctorBool<size_t, CRJStringByteFlex&>;
	template<typename TFunc>
	using CJSONFunctorParser=CRJFunctorBool<IJSONParser, TFunc, CRJStringByteFlex&>;
	template<typename TFunc>
	using CJSONFunctorVar=CRJFunctorBool<IJSONVar, TFunc, CRJStringByteFlex&, CRJStringByteFlex&>;
	template<typename TFunc>
	using CJSONFunctorArray=CRJFunctorBool<IJSONArray, TFunc, size_t&, CRJStringByteFlex&>;
	class RJWWW_API CJSONParser:public CRJStringByteFlex {
	public:
		CJSONParser(const CRJStringByte&);
		virtual~CJSONParser();
		static bool IsVar(const CRJStringByte&, IJSONVar&);
		static bool IsArray(const CRJStringByte&, IJSONArray&);
		bool ParseVar(IJSONVar&);
		bool ParseArray(IJSONArray&);
	protected:
		bool Parse(IJSONParser&);
		static CRJStringByte s_JSONSeparators;
	};
}

