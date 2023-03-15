#pragma once
#include <recurj/rjframework/modules/modbase.h>
namespace rjBase {
	class RJBASE_API CRJBytePacker {
		friend class CRJByteConvertor;
	public:
		static void UnpackReadable(uint8_t*,const uint8_t*, size_t);
		static void UnpackMachine(uint8_t*,const uint8_t*, size_t);
		static void UnpackWithHiddenReadable(uint8_t*,const uint8_t*, size_t);
		static void UnpackWithHiddenMachine(uint8_t*,const uint8_t*, size_t);
		static void PackReadable(uint8_t*,const uint8_t*, size_t);
		static void PackMachine(uint8_t*,const uint8_t*, size_t);
		static void PackWithHiddenReadable(uint8_t*,const uint8_t*, size_t);
		static void PackWithHiddenMachine(uint8_t*,const uint8_t*, size_t);
		static size_t Bin2Str(char*, size_t,const uint8_t*, size_t);
		static size_t ByteToHex(uint8_t*, uint8_t,bool bAdd=false,char ch=' ');
		static void HexStringLong(size_t, uint8_t*);
		static void HexStringLongLong(size_t, uint8_t*);
	protected:
		static const uint16_t s_clMask;
		static const uint8_t s_uHiBytes[256];
		static const uint8_t s_uLowBytes[256];
		static const char* s_clChars[256];
	};
}

