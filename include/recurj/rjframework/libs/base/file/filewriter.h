#pragma once
#include <recurj/rjframework/libs/base/file/filebase.h>
#include <recurj/rjframework/libs/base/file/fileencode.h>
namespace rjBase {
	class CRJStringByteFlex;
	class CRJFileName;
	class CRJStringByte;
	class CRJStringUnicode;
	class RJBASE_API CRJFileWriter:public CRJFileBase {
	public:
		CRJFileWriter();
		CRJFileWriter(CRJFileBase&);
		virtual~CRJFileWriter();
		operator bool() const {	return m_bOK;}
		CRJFileWriter& SetEncoding(CRJFileEncode::eRJFileEncoding);
		CRJFileWriter& Update(CRJFileName&,int flags=cFileDefOpenWriter,int mode=cFileDefPermission);
		CRJFileWriter& Append(CRJFileName&,int flags=cFileDefOpenWriter,int mode=cFileDefPermission);
		CRJFileWriter& Backup(CRJFileName&, CRJFileEncode::eRJFileEncoding=CRJFileEncode::eRJFileEncoding::ASCII);
		CRJFileWriter& Create(CRJFileName&,int flags=cFileDefOpenWriter,int mode=cFileDefPermission);
		CRJFileWriter& Writeln();
		CRJFileWriter& Writelf();
		CRJFileWriter& Write(const uint8_t*, size_t);
		CRJFileWriter& Write(const CRJStringByte&);
		CRJFileWriter& Writeln(const CRJStringByte&);
		CRJFileWriter& Writelf(const CRJStringByte&);
		CRJFileWriter& Write(const CRJStringUnicode&);
		CRJFileWriter& Writeln(const CRJStringUnicode&);
		CRJFileWriter& Writelf(const CRJStringUnicode&);
		CRJFileWriter& WriteLabel(const char*);
		CRJFileWriter& WriteChar(char);
		inline void MarkError() {m_bOK=false;}
		inline CRJFileWriter& GetFileWriterMe() {return *this;}
		inline bool Done() {return Flush()&&Close();}
	protected:
		bool m_bOK;
	};
}

