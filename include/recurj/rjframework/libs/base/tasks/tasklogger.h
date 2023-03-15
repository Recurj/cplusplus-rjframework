#pragma once
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/strings/dtime.h>
#include <recurj/rjframework/libs/base/tasks/taskqueuebasic.h>
namespace rjBase {
	constexpr size_t cLoggerTaskQueueLimit=1024;
	constexpr int64_t cLoggerTaskConfigureFileSize=0;
	constexpr int64_t cLoggerTaskConfigureQueueLimit=1;
	class RJBASE_API CRJTaskLogger:public CRJTaskQueueBasic {
	public:
		CRJTaskLogger(CRJLoggerWriter&);
		virtual~CRJTaskLogger();
		CRJMessage* SendNormal(CRJMessage&) const override;
		void SetQueueLimit(size_t);
		CRJStringByteVar& GetName(CRJStringByteVar&) override;
	protected:
		CRJMessage* OnTaskQueueMsg(CRJMessage&) override;
		void TaskRunDone(bool) override;
		CRJMessage* Out(CRJMessage& clMsg);
	protected:
		CRJSharedSize m_sQueueLimit;
	};
}

