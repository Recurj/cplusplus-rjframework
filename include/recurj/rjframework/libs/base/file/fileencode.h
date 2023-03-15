#pragma once
#include <recurj/rjframework/libs/base/file/filebase.h>
using namespace std;
namespace rjBase {
	class CRJFileName;
	class CRJFileReader;
	class CRJFileWriter;
	class RJBASE_API CRJFileEncode {
	public:
		enum class eRJFileEncoding {BINARY, UTF8, UTF16_BE, UTF16_LE, UTF32_BE, UTF32_LE, ASCII, UTF7, UTF1, UTF_EBCDIC,SCSU};
		CRJFileEncode();
		CRJFileEncode(eRJFileEncoding);
		virtual~CRJFileEncode();
		bool Save(CRJFileWriter&) const;
		bool Load(CRJFileName&);
		bool Load(CRJFileReader&, size_t&);
		bool IsUTF8(CRJFileReader&, size_t&);
		inline CRJFileEncode& SetEncoding(eRJFileEncoding encode=eRJFileEncoding::UTF8) {
			m_clEncoding=encode;
			return *this;
		}
		inline eRJFileEncoding GetEncoding() const {return m_clEncoding;}
		size_t HeaderSize() const;
		static uint8_t s_markerUTF8[3];
		static uint8_t s_markerUTF32_LE[4];
		static uint8_t s_markerUTF32_BE[4];
		static uint8_t s_markerUTF16_LE[2];
		static uint8_t s_markerUTF16_BE[2];
	private:
		static bool Read(CRJFileReader&, uint8_t*, size_t);
		static bool Write(CRJFileWriter&,const uint8_t*, size_t);
		eRJFileEncoding m_clEncoding;
	};
}

