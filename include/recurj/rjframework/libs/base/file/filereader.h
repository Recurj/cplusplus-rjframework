#pragma once
#include <recurj/rjframework/libs/base/file/filebase.h>
#include <recurj/rjframework/libs/base/file/fileencode.h>
#include <recurj/rjframework/functors.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
using namespace std;
namespace rjBase {
	class CRJFileName;
	class CRJStringByteVar;
	using IRJFileReader=IRJFunctorBool<CRJStringByteVar&>;
	template<typename TFunc>
	using CRJFileReaderFunctorBool=CRJFunctorBool<IRJFileReader, TFunc, CRJStringByteVar&>;
	class RJBASE_API CRJFileReader:public CRJFileBase {
	public:
		CRJFileReader();
		virtual~CRJFileReader();
		bool OpenEncoding(CRJFileName&,int flags=cFileDefOpenReader);
		bool OpenBinary(CRJFileName&,int flags=cFileDefOpenReader);
		bool LoadFile(CRJStringByteVar&);
		bool Read(CRJStringByteVar&, size_t);
		bool Read(void*, size_t);
		bool ReadAvailable(void*, size_t&);
		inline size_t GetSize() const {return m_sSize;};
		inline const CRJFileEncode &GetEncode() const {return m_clEncode;}
		static bool Load(CRJFileName&, CRJStringByteVar&);
	protected:
		CRJFileEncode m_clEncode;
		size_t m_sSize;
	};
	class RJBASE_API CRJFileReaderShared:public CRJFileReader {
	public:
		CRJFileReaderShared(CRJFileBase& clParent):
			m_clParent(clParent) {
			Swap(m_clParent);
		}
		virtual~CRJFileReaderShared() {
			m_clParent.Swap(*this);
		}
	protected:
		CRJFileBase& m_clParent;
	};
	class RJBASE_API CRJFileCRC32:public IRJBlockReader {
	public:
		enum 
		class Result {
			OK, NOFILE, NOREAD
		};
		CRJFileCRC32(CRJFileName&);
		bool GetBlock(CRJStringByteFlex&) override;
		bool OK();
		Result GetResult();
	protected:
		CRJFileReader m_clFile;
		CRJStringByteLong m_clData;
		size_t m_sSize;
		Result m_eResult;
	};
}

