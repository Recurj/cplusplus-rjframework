#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
namespace rjBase {
	constexpr size_t CRJCompareEQ=0;
	constexpr size_t CRJCompareNE=1;
	constexpr size_t CRJCompareGT=2;
	constexpr size_t CRJCompareLT=3;
	constexpr size_t CRJCompareGE=4;
	constexpr size_t CRJCompareLE=5;
	constexpr size_t CRJCompareError=7;
	class RJBASE_API CRJStringDictionary {
	public:
		static CRJStringByte s_wordTemp;
		static CRJStringByte s_compareEQ;
		static CRJStringByte s_compareNE;
		static CRJStringByte s_compareGT;
		static CRJStringByte s_compareLT;
		static CRJStringByte s_compareGE;
		static CRJStringByte s_compareLE;
		static CRJStringByte s_functionNOT;
		static CRJStringByte s_functionOR;
		static CRJStringByte s_functionAND;
		static CRJStringByte s_functionXOR;
		static CRJStringByte s_functionEQ;
		static CRJStringByte s_functionNE1;
		static CRJStringByte s_functionNE2;
		static CRJStringByte s_functionGT;
		static CRJStringByte s_functionLT;
		static CRJStringByte s_functionGE;
		static CRJStringByte s_functionLE;
	};
}

