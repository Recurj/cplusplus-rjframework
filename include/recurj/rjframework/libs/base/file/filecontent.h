#pragma once
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/file/fileencode.h>
using namespace std;
namespace rjBase {
	class RJBASE_API CRJFileContent:public CRJStringByteExpanded {
	public:
		CRJFileContent();
		virtual~CRJFileContent();
		bool Load(CRJFileName&);
		bool Save(CRJFileName&);
		inline const CRJFileEncode& GetEncode() const {
			return encode;
		}
	protected:
		CRJFileEncode encode;
	};
}

