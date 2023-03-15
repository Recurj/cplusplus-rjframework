#pragma once
#include <recurj/rjframework/libs/base/file/filewriter.h>
namespace rjBase {
	class CRJFileName;
	class CRJStringByte;
	class CRJStringByteVar;
	class CRJStringUnicode;
	class RJBASE_API CRJFile:public CRJFileWriter {
	public:
		CRJFile();
		virtual~CRJFile();
		bool Open(CRJFileName&,int flags=cFileDefOpenWriter);
		bool CreateTemp(CRJFileName&,int flags=cFileDefOpenWriter);
		size_t GetTextSize();
		bool Read(void*, size_t);
		bool ReadAvailable(void*, size_t&);
		bool ReadlnFormat(CRJStringByteVar&,bool bFormat=true);
	};
}

