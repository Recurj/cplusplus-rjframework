#pragma once
#include <recurj/rjframework/libs/base/logger/writers/fileapp.h>
#include <recurj/rjframework/libs/base/tasks/tasklogger.h>
namespace rjBase {
	class CRJTaskLogger;
	class RJBASE_API CRJLogWriterTask:public CRJLoggerWriter {
	public:
		CRJLogWriterTask(CRJLogWriterFile&);
		virtual~CRJLogWriterTask();
		bool LogPrivate(const CRJStringByte&) override;
		bool LogPublic(const CRJStringByte&) override;
		bool Start(IRJLogWriterManager&) override;
		void Stop() override;
		void QueueReset();
		inline void SetQueueLimit(size_t s) {m_clTask.SetQueueLimit(s);}
		bool IsReady() override {return m_clTask.TaskIsRunning();}
	protected:
		CRJTaskLogger m_clTask;
	};
}

