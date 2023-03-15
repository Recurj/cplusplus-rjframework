#pragma once
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/base/tasks/taskqueue.h>
#include <recurj/rjframework/modules/modnet.h>
#ifdef _rjOS_FAMILY_UNIX
#define EX_SOCKET_OK 0
#define EX_SOCKET_INVALID -1
constexpr int64_t EX_SOCKET_BAD_OPTIONS = ENOPROTOOPT;
constexpr int64_t EX_SOCKET_ERROR_NOT_EXIST = ENOTSOCK;
constexpr int64_t EX_SOCKET_ERROR_NOT_CONNECT = ENOTCONN;
constexpr int64_t EX_SOCKET_ERROR_BLOCK = EWOULDBLOCK;
constexpr int64_t EX_SOCKET_ERROR_SEND = EINTR;
using HSOCKET=int;
#endif 
#ifdef _rjOS_WIN
constexpr SOCKET EX_SOCKET_INVALID = INVALID_SOCKET;
constexpr int64_t EX_SOCKET_BAD_OPTIONS = WSAENOPROTOOPT;
constexpr int64_t EX_SOCKET_ERROR_NOT_EXIST = WSAENOTSOCK;
constexpr int64_t EX_SOCKET_ERROR_NOT_CONNECT = WSAENOTCONN;
constexpr int64_t EX_SOCKET_ERROR_BLOCK = WSAEWOULDBLOCK;
constexpr int64_t EX_SOCKET_ERROR_SEND = WSAEINTR;
using HSOCKET=SOCKET;
#endif 
constexpr int64_t EX_SOCKET_ERROR_EXCEPTION = -1;
constexpr int64_t EX_SOCKET_ERROR_TOO_MANY_DEFFERED = -2;
namespace rjNet {
	constexpr size_t cRJNetTaskQueueSize = 3;
	struct RJNET_API SRJSocketProcessMetrics {
		SRJSocketProcessMetrics(int64_t r, uint32_t dw):
			lRetry(r),
		dwWait(dw) {}
		SRJSocketProcessMetrics() {
			lRetry=SRJSocketProcessMetrics::s_stSendMetrics.lRetry;
			dwWait=SRJSocketProcessMetrics::s_stSendMetrics.dwWait;
		}
		const static SRJSocketProcessMetrics s_stSendMetrics;
		int64_t lRetry;
		uint32_t dwWait;
	};
	class CRJSocket;
	class RJNET_API CRJNet {
	public:
		static bool Open();
		static void Close();
		static int64_t GetNetLastError();
		const static CRJStringByte s_localHost;
		enum eSocketActions :size_t {
			NONE = 0, ALLOC = 1, LISTEN = 2, CONNECT = 3, CLOSE = 4, ENUM = 5, ACCEPT = 6, READ = 7, SEND = 8, RESET_EVENT = 9,
			SET_EVENT = 10, SET_OPTION_USE_ADDR = 11, SET_OPTION_KEEPALIVE = 12, SET_OPTION_NON_BLOCK_MODE = 13,
			BIND_LOCAL_ADDRESS = 14, GET_HOST_ADDRESS = 15, BIND_REMOTE_ADDRESS = 16, BIND_LISTEN = 17,
			GET_REMOTE_INFO = 18, GET_LOCAL_INFO = 19, FIND_REMOTE_HOST = 20, SET_OPTION_NO_LINGER = 21,
			SET_OPTION_TOTAL_LINGER = 22, SET_OPTION_NO_DELAY = 23, SET_OPTION_SET_SEND_BUFFER = 24,
			SET_OPTION_SET_RECV_BUFFER = 25, WAIT_READ = 26, WAIT_SEND = 27, SET_OPTION_TIMEOUT = 28,
			READMSG = 29, SENDMSG = 30, ALLOCIPC = 31, RELEASE = 32, UINX_ACCEPT = 33, UINX_CONNECT = 34,
			UINX_LISTEN = 35, IPC_ALLOC = 36, IPC_DUPLICATE = 37, FIND_LOCAL_HOST = 38,
		};
	};
	class IRJSocketHolder {
	public:
		virtual void OnSocketError(int64_t, size_t) = 0;
		virtual void OnSocketWarning(int64_t, size_t) = 0;
	};
	class IRJSocketManager  {
	public:
		virtual void OnNetEventConnect() = 0;
		virtual void OnNetEventAccept() = 0;
		virtual void OnNetEventRead() = 0;
		virtual void OnNetEventClose() = 0;
		virtual void OnNetEventSend() = 0;
		virtual void OnNetEventError(int64_t, CRJNet::eSocketActions) = 0;
	};
	using CRJNetListenerData=RJListenerMapData<cRJOSPollMax>;
	using CRJNetTaskQueue=RJNET_API CRJTaskQueueVariant<cRJNetTaskQueueSize>;
	class CRJNetTask :public CRJNetTaskQueue, public IRJSocketManager, public IRJSocketHolder {
	public:
		CRJNetTask() {}
		virtual ~CRJNetTask() {}
		inline IRJSocketHolder& SocketHolder() { return *this; }
	};
}

