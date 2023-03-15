#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <recurj/rjframework/modules/modbase.h>
namespace rjBase {
	class RJBASE_API CRJFileBase {
	public:
		CRJFileBase();
		virtual~CRJFileBase();
		void Swap(CRJFileBase&);
		bool IsOpened() const {
			return m_hFile>=0;
		}
		bool Close();
		bool Flush();
		bool GetLength(filepos_t&);
		bool GetPos(filepos_t&);
		bool SetPos(filepos_t,int mode=SEEK_SET);
	protected:
		int m_hFile;
	};
}

