#pragma once
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <recurj/rjframework/libs/base/file/filewriter.h>
#include <mutex>
namespace rjBase {
	constexpr size_t cLoggerFileLimitDefault=10000000;
	class RJBASE_API CRJLogWriterFile:public CRJLoggerWriter {
	public:
		bool LogPublic(const CRJStringByte&) override;
		bool LogPrivate(const CRJStringByte&) override;
		bool Start(IRJLogWriterManager&) override;
		void Stop() override;
		bool IsReady() override {return m_clFile.IsOpened();}
		bool Open(CRJFileName&);
		inline int64_t IsOpened() const {return m_clFile.IsOpened();}
		IRJLoggerWriter& GetWriter() {return *this;}
		static const CRJStringByte s_appLogPath;
		static const CRJStringByte s_appLogExt;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJLogWriterFile)
	protected:
		CRJLogWriterFile();
		virtual~CRJLogWriterFile();
		static bool Console(const CRJStringByte&);
	protected:
		static const CRJStringByte s_logger;
		static std::mutex s_clConsoleCritical;
		CRJFileWriter m_clFile;
	};
	class RJBASE_API CRJLogWriterFileApp:public CRJLogWriterFile {
	public:
		CRJLogWriterFileApp();
		virtual~CRJLogWriterFileApp();
		bool LogPublic(const CRJStringByte&) override;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJLogWriterFileApp)
	protected:
		std::mutex m_clLogCritical;
	};
}

