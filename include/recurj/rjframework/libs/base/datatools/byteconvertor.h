#pragma once
#include <recurj/rjframework/modules/modbase.h>
namespace rjBase {
	class CRJStringByte;
	class CRJStringByteVar;
	class RJBASE_API CRJByteConvertor {
	public:
		static const uint8_t* Int2PairDecDigits(size_t);
		static const uint8_t* Int2ThreeDecDigits(size_t);
		static size_t Int64ToBytes(int64_t, uint8_t*, size_t);
		static size_t SizeToBytes(size_t, uint8_t*, size_t);
		static size_t HexToBytes(size_t, uint8_t*, size_t);
		static size_t Int64ToUnicodes(int64_t,wchar_t*, size_t);
		static size_t SizeToUnicodes(size_t,wchar_t*, size_t);
		static size_t HexToUnicodes(size_t,wchar_t*, size_t);
		static bool Bytes2Size(const uint8_t*, size_t, size_t&);
		static bool Bytes2Int64(const uint8_t*, size_t, int64_t&);
		static int64_t PairChars2NumSafed(const uint8_t*);
		static int64_t HexPair2IntSafedCheck(const uint8_t*, int64_t);
		static int64_t HexStr2IntSafed(const uint8_t*, size_t);
		static int64_t DecStr2IntSafed(const uint8_t*, size_t);
		static int64_t DecPair2IntSafedNoCheck(const uint8_t*);
		static int64_t DecPair2IntSafedCheck(const uint8_t*, int64_t);
		static int64_t DecThree2IntSafedCheck(const uint8_t*, int64_t);
		static void Byte2PairHex(uint8_t*, uint8_t);
		static int64_t ByteValue2Int(uint8_t);
		static size_t ByteDigit2Size(uint8_t);
		static uint8_t PairHex2Byte(uint8_t, uint8_t);
		static uint16_t BigEndianSet16(uint16_t);
		static uint32_t BigEndianSet32(uint32_t);
		static uint64_t BigEndianSet64(uint64_t);
		static uint16_t BigEndianReverse16(uint16_t);
		static uint32_t BigEndianReverse32(uint32_t);
		static uint64_t BigEndianReverse64(uint64_t);
		static bool DefineByteOrder();
		static bool IsBigEndian() {
			return s_bBigEndian;
		}
		static bool IsDecDigit(uint8_t);
		static bool IsHexDigit(uint8_t);
		static bool Base64Encode(const CRJStringByte&, CRJStringByteVar&);
		static bool Base64DecodeInPlace(CRJStringByteVar&);
		static size_t Base94Encode(const uint8_t*, size_t,char*, size_t);
		static size_t Base94DecodeInPlace(uint8_t*, size_t);
	public:
		static bool s_bBigEndian;
		static const uint8_t s_uBytes2Int64[256];
		static const int64_t s_uPairChars[256];
		static const int8_t s_lHexNumbersFlag[256];
		static const uint8_t s_lByte2Digits[256];
		static const int64_t s_lTenDegrees[256];
		static const int64_t s_lNumbers[256];
		static const int64_t s_lPairChars[256];
		static const char* s_uThreeDigitsToString[1024];
		static const char* s_uPairDigitsToString[128];
		static const char s_Base64Encode[65];
		static const uint8_t s_Base64Decode[128];
		static const size_t s_Base64TailAdd[3];
	};
}

