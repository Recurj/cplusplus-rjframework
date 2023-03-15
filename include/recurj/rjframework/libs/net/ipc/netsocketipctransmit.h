#pragma once
#include <recurj/rjframework/libs/net/ipc/netsocketipc.h>
namespace rjNet {
	class RJNET_API CRJNetSocketIPCTransmit {
	public:
		CRJNetSocketIPCTransmit();
		virtual~CRJNetSocketIPCTransmit();
		bool Load(CRJSocket&, uint64_t);
		bool Send(CRJSocket&, uint64_t,const CRJStringByte&);
	protected:
		bool SendParams(CRJSocket&,const CRJStringByte&);
#ifdef  _rjOS_FAMILY_UNIX
		int m_stInfo;
#endif 
#ifdef _rjOS_WIN
		SNetInfo m_stInfo;
#endif 
	};
}

