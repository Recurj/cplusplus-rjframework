#pragma once
#include <recurj/rjframework/libs/base/datatools/bytestorage.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/datatools/result.h>
#include <recurj/rjframework/libs/base/os/hevent.h>
#include <recurj/rjframework/libs/net/netbase.h>
#include <recurj/rjframework/modules/modnet.h>
namespace rjNet {
	constexpr size_t cSocketEnppointAddressSize=8;
	constexpr size_t cSocketSendDeferredMax=1024* 1024;
	class CRJNetSend;
	struct RJNET_API SRJSocketPartInfo {
		CRJStringByteVar& ToString(CRJStringByteVar&) const;
		CRJStringByteVar& AddrToString(CRJStringByteVar&) const;
		void Reset();
		uint8_t m_bDefined;
		uint8_t m_sCount;
		uint16_t m_sPort;
		uint16_t m_uAddress[cSocketEnppointAddressSize];
	};
	struct RJNET_API SRJSocketEndpoint {
		CRJStringByteVar& ToString(CRJStringByteVar&) const;
		void Reset();
		SRJSocketPartInfo m_stLocal;
		SRJSocketPartInfo m_stRemote;
	};
	class RJNET_API CRJSocket {
		friend class CRJSocketAccepted;
		friend class CRJSocketDuplicated;
		friend class CRJSocketReference;
		friend class CRJSocketTCP;
		friend class CRJSocketUDP;
		friend class CRJSocketIPC;
	public:
		CRJSocket(IRJSocketHolder &);
		CRJSocket(IRJSocketHolder& ,HSOCKET);
		virtual~CRJSocket();
		CRJSocket& SocketTake(CRJSocket&);
		virtual void SocketRelease();
		virtual size_t SocketGetSendDeferredMax() const;
		void SocketInfo(size_t,const CRJStringByte&);
		void SocketInfoData(size_t,const CRJStringByte&, size_t);
		bool SocketOnError(int64_t, CRJNet::eSocketActions);
		bool SocketWarning(int64_t, CRJNet::eSocketActions);
		int64_t SocketOnErrorCode(int64_t, CRJNet::eSocketActions);
		int64_t SocketGetError();
		inline HSOCKET SocketGet() const {return m_hHandle;}
		inline HSOCKET SocketDetach() {	HSOCKET h=m_hHandle;m_hHandle=EX_SOCKET_INVALID;return h;}
		inline bool SocketIsValid() const {	return (m_hHandle!=EX_SOCKET_INVALID);}
		void SocketAttach(HSOCKET);
		void SocketClose();
		bool SocketBind(size_t &,const CRJStringByte&);
		bool SocketConnect(size_t,const CRJStringByte&);
		bool SocketConnect(
		struct sockaddr_in&);
		bool SocketReadAvailable(CRJStringByteVar&,const SRJSocketProcessMetrics&);
		bool SocketReadNeeded(uint8_t*, size_t,const SRJSocketProcessMetrics&, size_t&);
		bool SocketReadBlock(uint8_t*, size_t&);
		bool SocketReadIsWaiting(size_t&);
		bool SocketSend(CRJStringByteFlex&, CRJNetSend&);
		bool SocketWaitRead(bool&);
		bool SocketWaitRead(int64_t,bool&,bool&);
		bool SocketWaitSend(int64_t,bool&,bool&);
		int64_t SocketPrepareMapRead(fd_set&, fd_set&);
		bool SocketExistInMap(fd_set&);
		int64_t SocketSetOptionReUseAddr(bool);
		int64_t SocketSetOptionLinger(bool, int64_t);
		int64_t SocketSetOptionSendBufferSize(size_t);
		int64_t SocketSetOptionRecvBufferSize(size_t);
		int64_t SocketSetOptionBlockMode(bool);
		bool SocketIsConnected() const;
		bool SocketGetConnectionInfo(CRJStringByteVar&);
		bool SocketGetConnectionInfo(SRJSocketEndpoint&);
		bool SocketGetConnectionRemoteInfo(SRJSocketPartInfo&);
		bool SocketGetConnectionLocalInfo(SRJSocketPartInfo&);
		inline IRJSocketHolder& SocketHolder() { return m_clHolder; }

		inline CRJSocket& SocketGetMe() {return *this;}
		static bool SocketIsValid(HSOCKET h) {return (h!=EX_SOCKET_INVALID);}
		static bool SocketGetHostName(CRJStringByteVar&);
		static bool SocketError(int64_t, CRJNet::eSocketActions);
	public:
		static bool SocketIsErrorFatal(int64_t);
	protected:
		virtual bool OnSocketSendNeedWait(const SRJSocketProcessMetrics&);
		virtual bool OnSocketReadNeedWait(const SRJSocketProcessMetrics&);
		bool SocketFindRemoteHostByName(const CRJStringByte&, in_addr&);
		void SocketSin2Info(
		struct sockaddr_in&, SRJSocketPartInfo&);
	private:
		CRJSocket(const CRJSocket&)=delete;
		CRJSocket&operator=(const CRJSocket&)=delete;
	public:
		static const CRJStringByte s_clConnected;
		static const CRJStringByte s_clDisconnected;
		static const CRJStringByte s_clRecv;
		static const CRJStringByte s_clSend;
		static const CRJStringByte s_clLocalHost;
#ifdef _rjOS_FAMILY_UNIX
		static const CRJStringByte s_clSockPath;
#endif 
	private:
		IRJSocketHolder& m_clHolder;
		HSOCKET m_hHandle;
	};
	class RJNET_API CRJSocketDuplicated:public CRJSocket {
	public:
		CRJSocketDuplicated(IRJSocketHolder& holder):CRJSocket(holder) {}
		CRJSocketDuplicated(const CRJSocket& cl) :CRJSocket(cl.m_clHolder) {
			SocketDuplicate(cl);
		}
		virtual~CRJSocketDuplicated() {
			m_hHandle=EX_SOCKET_INVALID;
		}
		void SocketRelease() override {
			m_hHandle=EX_SOCKET_INVALID;
		}
		void SocketDuplicate(const CRJSocket& cl) {
			m_hHandle=cl.m_hHandle;
		}
		void SocketDuplicate(HSOCKET h) {
			m_hHandle=h;
		}
		void SocketReUse() {
			m_hHandle=EX_SOCKET_INVALID;
		}
	};
	class RJNET_API CRJSocketReference {
	public:
		inline HSOCKET SocketHandleGet(const CRJSocket& clSock) const {
			return clSock.m_hHandle;
		}
	};
	struct RJNET_API SRJNetSocketEndPoint {
		SRJNetSocketEndPoint();
		void operator=(const SRJNetSocketEndPoint&);
		inline bool IsDefined() const;
		CRJStringByteAllocated m_clHost;
		size_t m_sPort;
	};
#ifdef _rjOS_WIN
	struct SNetInfo {
		uint64_t id;
		WSAPROTOCOL_INFOW wsa;
	};
#endif 
}

