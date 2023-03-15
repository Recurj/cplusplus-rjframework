#pragma once
#include <recurj/rjframework/libs/base/tasks/taskqueue.h>
namespace rjBase {
	const size_t cTaskQueueBasicEvents=2;
	class RJBASE_API CRJTaskQueueBasic:public CRJTaskQueue {
	public:
		CRJTaskQueueBasic(size_t t=cTaskTypeBasic);
		virtual~CRJTaskQueueBasic();
	protected:
		RJListenerMapData<cTaskQueueBasicEvents>m_stListenerData;
	};
}

