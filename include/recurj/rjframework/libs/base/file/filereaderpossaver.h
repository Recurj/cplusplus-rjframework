#pragma once
#include <recurj/rjframework/modules/modbase.h>
#include <recurj/rjframework/libs/base/file/filebase.h>
namespace rjBase {
	class RJBASE_API CRJFileSaveReaderPosSaver {
	public:
		CRJFileSaveReaderPosSaver(CRJFileBase& clFile):
			m_clFile(clFile) {
			clFile.GetPos(m_lPos);
		}
		CRJFileSaveReaderPosSaver(CRJFileBase& clFile, filepos_t s,int mode):
			m_clFile(clFile) {
			clFile.GetPos(m_lPos);
			clFile.SetPos(s, mode);
		}
		~CRJFileSaveReaderPosSaver() {
			m_clFile.SetPos(m_lPos);
		}
		CRJFileBase& m_clFile;
		filepos_t m_lPos;
	};
}

