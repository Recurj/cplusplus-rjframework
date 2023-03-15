#pragma once
#include <recurj/rjframework/libs/base/formats/simple/defs.h>
namespace rjBase {
	class RJBASE_API RJFormatParser {
	public:
		static int64_t Reply(const CRJStringByte&, IRJFormatReader&);
		static int64_t Request(const CRJStringByte&, IRJFormatReader&);
		static bool Lines(const CRJStringByte&, size_t, IRJFormatReader&);
		static int64_t Fields(const CRJStringByte&, size_t, IRJFormatReader&, size_t start=0);
		static int64_t onError(const CRJStringByte&);
	};
}

