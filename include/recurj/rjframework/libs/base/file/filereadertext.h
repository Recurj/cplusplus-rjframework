#pragma once
#include <recurj/rjframework/libs/base/file/filereader.h>
#include <functional>
namespace rjBase {
	class CRJStringByteVar;
	class RJBASE_API CRJFileReaderText:public CRJFileReader {
	public:
		CRJFileReaderText();
		virtual~CRJFileReaderText();
		bool Scan(CRJStringByteVar&,bool, IRJFileReader&);
		bool ReadlnFormat(CRJStringByteVar&,bool bFormat=true);
		bool ReadlnData(CRJStringByteVar&);
	protected:
		bool OffsetCurrentRead(filepos_t);
	};
	class RJBASE_API CRJFileReaderTextShared:public CRJFileReaderText {
	public:
		CRJFileReaderTextShared(CRJFileBase& clParent):
			m_clParent(clParent) {
			Swap(m_clParent);
		}
		virtual~CRJFileReaderTextShared() {
			m_clParent.Swap(*this);
		}
	protected:
		CRJFileBase& m_clParent;
	};
}

