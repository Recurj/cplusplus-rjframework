#pragma once
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/base/messages/message.h>
#include <recurj/rjframework/libs/base/datatools/atomicnumber.h>
#include <recurj/rjframework/libs/base/os/hlistener.h>
namespace rjBase {
	constexpr size_t cTaskTypeLog=0;
	constexpr size_t cTaskTypeBasic=1;
	constexpr size_t cTaskTypeApp=1024;
	class CRJTask;
	class ITaskManager {
	public:
		virtual void ITaskManagerCompleted(CRJTask&)=0;
		virtual bool ITaskManagerPrepare(CRJTask&)=0;
	};
	const size_t cTaskWaitComplete=30000;
	class RJBASE_API CRJTask:public CRJEventHandler {
		friend class CRJTaskDummy;
		friend class CRJTaskQueue;
	public:
		using funcTaskCleanup=void(*)(CRJTask&);
		CRJTask(size_t t=cTaskTypeBasic);
		virtual~CRJTask();
		virtual CRJMessage* SendNormal(CRJMessage&) const;
		virtual CRJMessage* SendUrgent(CRJMessage&) const;
		virtual bool SendList(CRJListMessages&) const;
		virtual bool TaskIsDummy() const;
		virtual bool TaskIsReadyForMsgs(CRJTask&, size_t, size_t);
		virtual bool TaskIsReadyToUnload();
		virtual bool TaskStart();
		virtual bool TaskStop(size_t dw=cTaskWaitComplete);
		virtual void TaskBreak();
		virtual void TaskRun(bool);
		virtual funcTaskCleanup TaskThreadStart();
		virtual void TaskThreadDone();
		bool TaskIsBreakOn(size_t);
		bool TaskWait();
		bool TaskWaitTimed(size_t);
		bool TaskForeground();
		CRJTask& TaskLogSetWriter(CRJLoggerWriter&);
		CRJTask& TaskLogSetLevels(const SRJLoggerLevels&);
		CRJLoggerWriter& TaskLogGetWriter();
		inline int64_t TaskIsRunning() const {return m_clRunning.anGet();}
		inline size_t TaskGetType() const {	return m_sTaskType;}
		inline CRJTask& TaskGetMe() {	return *this;}
		inline CRJEventListener& TaskGetListener() {	return m_clListener;}
		static uint64_t ThreadGetId();
		static uint32_t ThreadSleep(uint32_t);
		static void ThreadYield();
		static void TaskSetManager(ITaskManager&);
		static ITaskManager& TaskGetManager();
		static void TaskCleanup(CRJTask&);
	protected:
		class CTaskManager:public ITaskManager {
		public:
			void ITaskManagerCompleted(CRJTask&) override {}
			bool ITaskManagerPrepare(CRJTask&) override {
				return true;
			}
			virtual ~CTaskManager() {}
		};
		virtual bool TaskRunReady();
		virtual void TaskRunBody();
		virtual void TaskRunFailed();
		virtual void TaskRunDone(bool);
		virtual funcTaskCleanup TaskGetCleanup();
		virtual int64_t TaskGetStackSize(int64_t);
	protected:
		CRJLogger m_clTaskLog;
		CRJLatch m_clRunning;
		RJEvent m_clEventNeedBreak, m_clEventStopped;
		CRJEventListener m_clListener;
		size_t m_sTaskType;
	private:
		static CTaskManager s_TaskManager;
		static ITaskManager* s_pTaskManager;
	};
	class RJBASE_API CRJTaskDummy:public CRJTask {
	public:
		CRJTaskDummy() {}
		virtual~CRJTaskDummy() {}
		bool TaskIsDummy() const override {
			return true;
		}
		CRJStringByteVar& GetName(CRJStringByteVar&) override;
		static CRJTask& GetTaskDummy() {
			return CRJTaskDummy::s_taskDummy;
		}
		static CRJTaskDummy s_taskDummy;
	};
	struct SRJTaskTicket {
		CRJTask* m_pTaskSource=nullptr;
		size_t m_sIndex=0;
		size_t m_sLifeSeq=0;
	};
}
template<typename T>void TaskStopAndFree(rjBase::CRJTask* p) {
	if (!p) return ;
	try {
		T* q=(T*) p;
		p->TaskStop();
		delete q;
	}
	catch(...) {}
}
template<typename T,typename...Types>bool TaskMsgGetAndSend(const rjBase::CRJTask& clTask, Types...args) {
	T* pMsg=nullptr;
	try {
		pMsg=T::Get(args...);
		if (pMsg&&
			clTask.SendNormal(* pMsg)==nullptr) return true;
	}
	catch(...) {}
	rjBase::CRJMessage::Done(pMsg);
	return false;
}
template<typename T,typename...Types>bool TaskMsgNewAndSend(const rjBase::CRJTask& clTask, Types...args) {
	T* pMsg=nullptr;
	try {
		pMsg=new T(args...);
		if (pMsg&&clTask.SendNormal(* pMsg)==nullptr) return true;
	}
	catch(...) {}
	rjBase::CRJMessage::Done(pMsg);
	return false;
}

