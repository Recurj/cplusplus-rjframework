#pragma once
#include <recurj/rjframework/libs/base/strings/byte/flex.h>

namespace rjBase {
	constexpr uint8_t CODE_STX = 0x02;
	constexpr uint8_t CODE_ETX = 0x03;
	constexpr int64_t cCompareEQ = 0;
	constexpr int64_t cCompareLess = -1;
	constexpr int64_t cCompareMore = 1;
	class CRJStringByte;
	class CRJStringByteVar;
	class CRJStringByteFlex;
	class IRJBlockReader {
	public:
		virtual bool GetBlock(CRJStringByteFlex&) = 0;
	};
	template <typename T> int64_t rjcompare(T val1,T val2) {
		if (val1==val2) return cCompareEQ;
		return  (val1 < val2)? cCompareLess:cCompareMore;
	}
	class RJBASE_API CRJDataTools {
	public:
		static void DebugBreak();
		static size_t SizeAlignPower(size_t, size_t);
		static size_t SizeAlignDefault(size_t);
		static uint32_t CRC32(IRJBlockReader&);
		static int64_t GetLastError();
		static int64_t GetErrorText(CRJStringByteVar&, int64_t);
		static int32_t ValidRangeInt32(int32_t, int32_t);
		static size_t ValidRangeSize(size_t, size_t);
		static size_t MultiplyByThousand(size_t);
		static size_t MultiplyByHundred(size_t);
		static size_t MultiplyByTen(size_t);
		static bool AreSame(const uint8_t*, const uint8_t*, size_t);
		static void MoveBytes(uint8_t*, const uint8_t*, size_t);
		static size_t MoveDataBlockSafed(uint8_t*, size_t, const uint8_t*, size_t);
		static size_t MoveDataBlockZero(uint8_t*, size_t, const uint8_t*, size_t);
		static void PaddingLeft(uint8_t*, size_t, uint8_t);
		static size_t Fields(const CRJStringByte&, uint8_t, CRJStringByteFlex[], size_t);
		static CRJStringByte s_szVersion;
		static const uint8_t s_uByteBits[8];
		static const size_t s_shiftRight[16];
		static const size_t s_shiftLeft[16];
		static const uint32_t s_sCRC32Table[256];
		static const uint8_t s_uBitFlags[8];
	};
	class RJBASE_API CRJByteCRC32 :public CRJStringByteFlex, public IRJBlockReader {
	public:
		using CRJStringByteFlex::CRJStringByteFlex;
		bool GetBlock(CRJStringByteFlex&) override;
	};
}
template<typename TParent, typename TChild>bool DLL_ENTRY UWLFreeFunctionDLL(TParent& cl) {
	try {
		TChild* p = (TChild*)&cl;
		delete p;
		return true;
	}
	catch (...) {}
	return false;
}
template<typename T>bool DLL_ENTRY UWLFreeFunctionDLLBase(T& cl) {
	try {
		T* p = &cl;
		delete p;
		return true;
	}
	catch (...) {}
	return false;
}

