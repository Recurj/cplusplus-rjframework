#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/datatools/bytestorage.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/net/netsocket.h>
#include <recurj/rjframework/modules/modnet.h>
namespace rjNet {
	const size_t cNetDeferredBlockSize=4* 1024;
	const size_t cNetDeferredThreshold=32* 1024;
	class RJNET_API CRJNetSend final {
	public:
		CRJNetSend(CRJSocket&);
		~CRJNetSend();
		void Reset();
		bool SendData(const CRJStringByte&);
		bool SendData(const uint8_t*, size_t);
		bool SendReady();
		bool SendResult(CRJStringByteFlex&, int64_t);
		inline void SetMetrics(const SRJSocketProcessMetrics& st) {	m_stMetrics=st;	}
		inline const SRJSocketProcessMetrics& GetMetrics() const {	return m_stMetrics;}
		inline void SendError(int64_t lErr) {m_clSock.SocketOnError(lErr, CRJNet::eSocketActions::SEND);}
		inline bool HasData() const {return m_pLast!=nullptr;}
		inline CRJSocket& GetSock() {return m_clSock;}
		inline size_t GetVolume() const {return m_sVolume;}
		inline void SetThreshold(size_t t) {m_sThreshold=t;}
		inline size_t GetTotalSent() const {return m_sTotalSent;}
		inline bool SetError(int64_t err) {	m_lError=err;return false;}
	protected:
		bool send(CRJStringByteFlex&);
		bool push(CRJStringByteFlex&);
		bool pop();
		void freeFirst();
		class RJNET_API CRJNetSendElem:public CRJByteStorage {
		public:
			void*operator new(size_t);
			void operator delete(void*);
			CRJNetSendElem();
			virtual~CRJNetSendElem();
			inline void SetNext(CRJNetSendElem* p) {
				m_pNext=p;
			}
			inline CRJNetSendElem* GetNext() const {
				return m_pNext;
			}
			static CRJAllocatorRuntime m_clHeap;
			uint8_t m_bBlock[cNetDeferredBlockSize];
			CRJNetSendElem* m_pNext;
		};
	protected:
		CRJSocket& m_clSock;
		CRJNetSendElem* m_pFirst=nullptr,* m_pLast=nullptr;
		SRJSocketProcessMetrics m_stMetrics;
		size_t m_sThreshold=cNetDeferredThreshold, m_sVolume=0;
		size_t m_sTotalSent=0;
		int64_t m_lError=0;
	};
}

