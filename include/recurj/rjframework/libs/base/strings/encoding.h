#pragma once
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/strings/unicode/var.h>
namespace rjBase {
	const size_t CP_CYRILLIC1251=1251;
	const size_t cUCYRILLIC1251CounntOfLetters=66;
	const size_t cUCodePage1251First=0xC0;
	const size_t cUCodePage1251SpecialLowerCase=0xB8;
	const size_t cUCodePage1251SpecialUpperCase=0xA8;
	const size_t cUUnicode1251First=0x410;
	const size_t cUUnicode1251Last=0x44F;
	const size_t cUUnicode1251SpecialLowerCase=0x401;
	const size_t cUUnicode1251SpecialUpperCase=0x451;
	class RJBASE_API CRJEncoding {
		using funcUtf8Pack=wchar_t(*)(const uint8_t*);
	public:
		static bool translateUtf8ToCodePage(const CRJStringByteFlex&, CRJStringByteFlex&, size_t);
		static CRJStringByteVar& translateUtf8ToCodePage(const CRJStringByte&, CRJStringByteVar&, size_t lPage=CP_CYRILLIC1251);
		static CRJStringByteVar& translateCodePageToUtf8(const CRJStringByte&, CRJStringByteVar&, size_t lPage=CP_CYRILLIC1251);
		static CRJStringByteVar& translateUnicodeToCodePage(const CRJStringUnicode&, CRJStringByteVar&, size_t lPage=CP_CYRILLIC1251);
		static CRJStringUnicodeVar& translateCodePageToUnicode(const CRJStringByte&, CRJStringUnicodeVar&, size_t lPage=CP_CYRILLIC1251);
		static CRJStringUnicodeVar& translateUtf8ToUnicode(const uint8_t*, size_t, CRJStringUnicodeVar&);
		static CRJStringByteVar& translateUnicodeToUtf8(const wchar_t*, size_t, CRJStringByteVar&);
		static wchar_t Utf8UnPack0(const uint8_t*);
		static wchar_t Utf8UnPack2(const uint8_t*);
		static wchar_t Utf8UnPack3(const uint8_t*);
		static wchar_t Utf8UnPack4(const uint8_t*);
		static wchar_t Utf8UnPack5(const uint8_t*);
		static wchar_t Utf8UnPack6(const uint8_t*);
		static CRJStringUnicodeVar& translateUtf8ToUnicode(const CRJStringByte&, CRJStringUnicodeVar&);
		static CRJStringByteVar& translateUnicodeToUtf8(const CRJStringUnicode&, CRJStringByteVar&);
		static size_t calcUnicodeToUtf8(const CRJStringUnicode&);
		static bool calcUtf8ToUnicode(const CRJStringByte&, size_t&);
	protected:
		static const wchar_t s_uUnicodTable1251[256];
		static const size_t s_tabUTF8Length[256];
		static funcUtf8Pack f_tabUtf8Unpack[8];
	};
}

