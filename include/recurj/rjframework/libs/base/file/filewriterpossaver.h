#pragma once
#include <fstream>
#include <recurj/rjframework/modules/modbase.h>
using namespace std;
namespace rjBase {
	class RJBASE_API CRJFileSaveWriterPosSaver {
	public:
		CRJFileSaveWriterPosSaver(ostream& clFile):
			m_clFile(clFile) {
			m_lPos=clFile.tellp();
		}
		CRJFileSaveWriterPosSaver(ostream& clFile, streampos s, ios_base::seekdir mode):
			m_clFile(clFile) {
			m_lPos=clFile.tellp();
			clFile.seekp(s, mode);
		}
		~CRJFileSaveWriterPosSaver() {
			m_clFile.seekp(m_lPos, ios_base::beg);
		}
		ostream& m_clFile;
		streampos m_lPos;
	};
}

