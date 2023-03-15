#include "stdafx.h"
#include <recurj/rjframework/libs/net/tasks/nettasksession.h>
using namespace rjNet;
CRJNetTaskSession::CRJNetTaskSession(CRJNetTaskAccepted& clTask):
	m_clTaskClient(clTask) {}
CRJNetTaskSession::~CRJNetTaskSession() {}

