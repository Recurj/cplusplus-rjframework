#pragma once
#include <recurj/rjframework/libs/net/tasks/nettaskaccepted.h>
#include <recurj/rjframework/libs/base/tasks/taskqueuebasic.h>
namespace rjNet {
	class RJNET_API CRJNetTaskSession:public CRJTaskQueueBasic {
	public:
		using funcInit=bool(*)(CRJFileName&);
		using funcLoader=CRJNetTaskSession*(*)(CRJNetTaskAccepted&,const CRJStringByte&);
		using funcRelease=CRJNetTaskSession*(*)(CRJNetTaskSession*);
	public:
		CRJNetTaskSession(CRJNetTaskAccepted&);
		virtual~CRJNetTaskSession();
		virtual void Ping()=0;
		virtual void OnNetRecv(CRJStringByteFlex&)=0;
		inline CRJNetTaskAccepted& GetTaskAccepted() {
			return m_clTaskClient;
		}
	protected:
		CRJNetTaskAccepted& m_clTaskClient;
	};
}

