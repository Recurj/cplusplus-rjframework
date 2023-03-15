#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/byte/value.h>
#include <recurj/rjframework/libs/base/strings/unicode/allocated.h>
#include <recurj/rjframework/libs/base/strings/unicode/flex.h>
#include <recurj/rjframework/libs/base/strings/unicode/value.h>
namespace rjBase {
#ifdef _rjOS_FAMILY_UNIX
	using stdString=string;
	using CRJString=CRJStringByte;
	using CRJStringFlex=CRJStringByteFlex;
	using CRJStringVar=CRJStringByteVar;
	using CRJStringDefault=CRJStringByteDefault;
	using CRJStringShort=CRJStringByteShort;
	using CRJStringLong=CRJStringByteLong;
	using CRJStringValue=CRJStringByteValue;
	using CRJStringExpanded=CRJStringByteExpanded;
	using CRJStringAllocated=CRJStringByteAllocated;
	using CRJStringSymbol=uint8_t;
	using CRJStringLabel=char;
#endif 
#ifdef _rjOS_WIN
	using stdString=wstring;
	using CRJString=CRJStringUnicode;
	using CRJStringFlex=CRJStringUnicodeFlex;
	using CRJStringVar=CRJStringUnicodeVar;
	using CRJStringDefault=CRJStringUnicodeDefault;
	using CRJStringShort=CRJStringUnicodeShort;
	using CRJStringLong=CRJStringUnicodeLong;
	using CRJStringValue=CRJStringUnicodeValue;
	using CRJStringExpanded=CRJStringUnicodeExpanded;
	using CRJStringAllocated=CRJStringUnicodeAllocated;
	using CRJStringSymbol=wchar_t;
	using CRJStringLabel=wchar_t;
#endif 
	class RJBASE_API CRJStringNativeTools {
	public:
		static stdString& MakeStdString(const CRJString&, stdString&);
		static stdString& StdString(const CRJStringByte&, stdString&);
		static stdString& StdString(const CRJStringUnicode&, stdString&);
	};
}

