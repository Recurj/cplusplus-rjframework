#include "stdafx.h"
#include <recurj/rjframework/libs/base/tasks/taskqueuebasic.h>
using namespace rjBase;
CRJTaskQueueBasic::CRJTaskQueueBasic(size_t t):
	CRJTaskQueue(t),
m_stListenerData(m_clListener) {}
CRJTaskQueueBasic::~CRJTaskQueueBasic() {}

