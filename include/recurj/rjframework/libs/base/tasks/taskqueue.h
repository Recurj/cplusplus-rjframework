#pragma once
#include <recurj/rjframework/libs/base/messages/queue.h>
#include <recurj/rjframework/libs/base/datatools/sharedsize.h>
#include <recurj/rjframework/libs/base/tasks/task.h>
namespace rjBase {
	class RJBASE_API CRJTaskQueue:public CRJTask {
	public:
		using queueMsgDelegate=CRJMessage*(CRJTaskQueue::*)(CRJMessage&);
		CRJTaskQueue(size_t t=cTaskTypeBasic);
		virtual~CRJTaskQueue();
		CRJMessage* SendNormal(CRJMessage&) const override;
		bool SendList(CRJListMessages&) const override;
		CRJMessage* SendRestricted(CRJMessage&, size_t) const;
		bool TaskIsReadyForMsgs(CRJTask&, size_t, size_t);
		bool TaskPing(size_t c=100, size_t dw=100);
		void QueueReset();
		inline void SetTaskQueueWait(size_t t) {m_clQueueWait.Set(t);}
		static size_t TaskQueueWaitParameter(const CRJStringByte&, size_t);
	protected:
		bool TaskRunReady() override;
		void TaskRunDone(bool) override;
		void TaskRunBody() override;
		void OnEventHandlerError(int64_t) override;
		void OnWaitEventQueue(size_t);
		bool OnWaitEventBreak(size_t) {	NeedBreak();return false;}
		virtual void TaskQueueLoop();
		virtual void TaskQueueReady();
		virtual void TaskQueueDone();
		virtual CRJMessage* OnTaskQueueMsg(CRJMessage&);
		virtual CRJMessage* onMsgRegionBase(CRJMessage&);
		virtual void TaskQueueMessagesBegin(size_t);
		virtual void TaskQueueMessagesEnd();
		virtual void TaskQueueMsgProcessed();
		virtual bool TaskQueueSetListener();
		virtual size_t TaskQueueDefaultWait();
		virtual size_t TaskQueueCheckMinWait(size_t);
		void TaskQueueProcess(CRJQueueMessages&);
		void TaskQueueProcess(CRJListMessages&);
		inline void NeedBreak() {m_bNeedBreak=true;}
	protected:
		mutable CRJQueueMessages m_clQueue;
		CRJSharedSize m_clQueueWait;
		bool m_bNeedBreak;
	};
	template<size_t N>
	class CRJTaskQueueVariant:public CRJTaskQueue {
	public:
		CRJTaskQueueVariant(size_t t=cTaskTypeBasic):CRJTaskQueue(t),m_stListenerData(m_clListener) {}
		virtual~CRJTaskQueueVariant() {}
	protected:
		RJListenerMapData<N>m_stListenerData;
	};
}

