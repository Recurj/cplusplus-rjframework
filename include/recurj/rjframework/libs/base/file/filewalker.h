#pragma once
#include <recurj/rjframework/modules/modbase.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
namespace rjBase {
	class RJBASE_API CRJFileWalker {
	public:
		CRJFileWalker();
		virtual~CRJFileWalker();
		virtual bool OnDirectory(const fs::directory_entry&);
		virtual bool OnFile(const fs::directory_entry&);
		virtual bool OnLink(const fs::directory_entry&);
		bool ScanLocal(const CRJFileName&);
		bool ScanTotal(const CRJFileName&);
		bool ScanCurrent(const CRJFileName&);
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJFileWalker)
	};
}

