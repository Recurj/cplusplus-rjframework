#pragma once
#include <recurj/rjframework/libs/base/logger/writers/fileapp.h>
#include <recurj/rjframework/libs/base/datatools/sharedsize.h>
namespace rjBase {
	class RJBASE_API CRJLogWriterFileModule:public CRJLogWriterFile {
	public:
		CRJLogWriterFileModule();
		virtual~CRJLogWriterFileModule();
		bool LogPrivate(const CRJStringByte&) override;
		void SetStorageLimit(size_t s) {
			m_sLimit.Set(s);
		}
		virtual void Restart()=0;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJLogWriterFileModule)
	protected:
		void checkLimit();
	protected:
		CRJSharedSize m_sLimit;
	};
}

