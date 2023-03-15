#pragma once
#include <recurj/rjframework/libs/base/collections/lists/defs.h>
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/datatools/bytereader.h>
#include <recurj/rjframework/libs/net/netsocketeventpool.h>
#include <recurj/rjframework/libs/net/netsocket.h>
#include <recurj/rjframework/libs/net/messages/netmessagesession.h>
#include <recurj/rjframework/libs/net/netsocketevent.h>
#include <recurj/rjframework/libs/net/dataapi/netsessions.h>
namespace rjNet {
	class RJNET_API CRJNetSocketEventSessionPooled:public CRJSocketTCPPool {
	public:
		CRJNetSocketEventSessionPooled(IRJSocketHolder&);
		virtual~CRJNetSocketEventSessionPooled();
		void PoolRelease() override;
		void OnNetEventRead() override;
		void OnNetEventSend() override;
		bool PoolStart() override;
		bool Transmit(CRJSocket&,const CRJStringByte&);
		SUNetSessionMetrics m_stMetric;
	protected:
		void RecvData();
		void OnRecv(const uint8_t*, size_t);
		void OnConfirm(CRJByteReader&);
		void OnDone(CRJByteReader&);
		void OnStatistic(CRJByteReader&);
		class CWaitElem:public CRJListItem<CWaitElem> {
		public:
			static CWaitElem* Get(CRJSocket& clSock, uint64_t id,const CRJStringByte& s) {
				return new CWaitElem(clSock, id, s);
			}
			CRJSocket m_clSock;
			uint64_t m_lId;
			void operator delete(void*);
		protected:
			void*operator new(size_t);
			CWaitElem(CRJSocket& clSock, uint64_t id,const CRJStringByte& s):m_clSock(clSock.SocketHolder()){
				m_clSock.SocketTake(clSock);
				m_lId=id;
				m_clParam.Set(s);
			}
			static CRJAllocatorRuntime m_clHeap;
			CRJStringByteDefault m_clParam;
		};
	protected:
		CRJListBase<CWaitElem>m_clWaitList;
		CRJNetSend m_clNetSend;
		CRJByteStorageAssigned<cUNetSocketInPoolSessionsBufSize>m_clStorage;
		uint64_t m_pid, m_lSeq;
	};
}

