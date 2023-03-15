#pragma once
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/logger/logger.h>
namespace rjBase {
	class RJBASE_API CRJLoggerWriterMemory:public CRJLoggerWriter {
	public:
		CRJLoggerWriterMemory() {}
		bool LogPublic(const CRJStringByte& s) override {
			return m_clLog.Cat(s).CatLN().OK();
		}
		bool LogPrivate(const CRJStringByte& s) override {
			return LogPublic(s);
		}
		bool Start(IRJLogWriterManager&) override {
			return true;
		}
		void Stop() override {}
		bool IsReady() override {
			return true;
		}
		inline const CRJStringByte& GetLogData() const {
			return m_clLog;
		}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJLoggerWriterMemory)
	protected:
		CRJStringByteExpanded m_clLog;
	};
}

