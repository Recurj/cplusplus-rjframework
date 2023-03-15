#pragma once
#include <recurj/rjframework/libs/net/ipc/netsocketipc.h>
namespace rjNet {
	class RJNET_API CRJNetSocketIPCAccept {
	public:
		CRJNetSocketIPCAccept();
		virtual~CRJNetSocketIPCAccept();
		bool Load(CRJSocket&);
		bool Read(CRJSocket&);
		inline const CRJStringByte& GetParameters() const {
			return m_clParam;
		}
#ifdef  _rjOS_FAMILY_UNIX
		inline uint64_t GetId() const {return m_lId;}
#endif 
#ifdef _rjOS_WIN
		inline uint64_t GetId() const {return m_stInfo.id;}
#endif 
	protected:
		bool ReadParams(CRJSocket&);
	protected:
		CRJStringByteDefault m_clParam;
#ifdef  _rjOS_FAMILY_UNIX
		int m_stInfo;
		uint64_t m_lId;
#endif 
#ifdef _rjOS_WIN
		SNetInfo m_stInfo;
#endif 
	};
}

