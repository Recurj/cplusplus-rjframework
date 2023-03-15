#pragma once
#include <recurj/rjframework/libs/base/datatools/bytestorage.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/modules/modwww.h>
namespace rjWWW {
	const size_t SIZE_HTTP_SERVER_INPUT_MODE=4;
	class RJWWW_API CHTTP:public CRJByteStorage {
	public:
		static const int64_t s_lMethodsCount=8;
		using funcRead=bool(CHTTP::*)();
		enum InputMode:size_t {
			INPUT_FIRST=0, INPUT_HEADER=1, INPUT_DATA=2, INPUT_ERROR=3
		};
		enum StreamError:int64_t {
			BAD_METHOD=-1, BAD_BINARY_LENGTH=-2, BAD_HEADER_PAIRS=-3, BAD_READ_MODE=-4, BAD_HEADER=-5,
			BAD_CONTENT_LENGTH=-6, BAD_VERSION_IN_STATUS_LINE=-7, BAD_COMMENT_IN_STATUS_LINE=-8,
			BAD_METHOD_IN_STATUS_LINE=-9, BAD_URL_IN_STATUS_LINE=-10, BAD_PARAMS=-11, BAD_PLUGIN=-12
		};
		enum Status:size_t {
			SUCCESS_OK=200, INFO_SWITCH_PROTOCOL=101, CLIENT_BAD_REQUEST=400, URL_NOT_FOUND=404,
			SERVER_INTERNAL=500, SERVER_NOT_IMPLEMENTED=501, SERVER_BAD_GATEWAY=502, SERVER_UNAVAILABLE=503,
			SERVER_TIMEOUT=504, SERVER_BAD_VERSION=505
		};
		enum Method:size_t {
			UNKNOWN=0, GET=1, HEAD=2, POST=3, PUT=4, TRACE=5, OPTIONS=6, DEL=7
		};
		struct CDataWeighted {
			void Set(const uint8_t* p, size_t l,double w) {
				pData=p;
				len=l;
				weight=w;
			}
			const uint8_t* pData;
			size_t len;
			double weight;
		};
		class CDataTableWeighted {
		public:
			CDataTableWeighted(CDataWeighted*, size_t);
			size_t Parse(const CRJStringByte&);
			CDataWeighted* m_pTable;
			size_t m_lFound, m_lVolume;
		protected:
			double Part(CRJStringByteFlex&);
			void Push(const uint8_t*, size_t,double);
			void Shift(size_t);
		};
		class CHandler {
		public:
			enum eResult:size_t {
				Error=1, NotLoaded=2,
			};
			using funcDo=bool(*)(CHTTP&);
			using funcInit=bool(*)(const rjBase::CRJStringByte&,const uint8_t*);
		};
		class RJWWW_API CMessage:public CRJStringByteExpanded {
		public:
			class AProcessor {
			public:
				virtual bool operator()(const CRJStringByte&,const CRJStringByte&)=0;
			};
			CMessage();
			virtual~CMessage();
			void Restart();
			bool SetHandlerResult(CHandler::eResult, Status=Status::SUCCESS_OK);
			bool Attributes(AProcessor&);
			inline const CRJStringByte& GetBinaryData() const {
				return m_clBinaryData;
			}
		public:
			CRJStringByteVarBuffer<8>m_clVersionRecv;
			CRJStringByteExpanded m_clBinaryData;
			CHandler::eResult m_lHandlerResult;
			Status m_lHandlerStatus;
			int64_t m_lVersionMajor, m_lVersionMinor;
		};
		class RJWWW_API CLang:public CRJStringByteVarBuffer<16> {
		public:
			using CRJStringByteVarBuffer::CRJStringByteVarBuffer;
			bool Known(const CRJStringByte&);
			void Define(const CRJStringByte&);
		};
		CHTTP();
		virtual~CHTTP();
		bool OnHTTPRecv();
	protected:
		virtual void HTTPDebug(const CRJStringByte&,const CRJStringByte&)=0;
		virtual void HTTPDebug(const CRJStringByte&,const uint8_t*, size_t)=0;
		virtual bool OnHTTPError(int64_t, size_t)=0;
		virtual bool OnHTTPRecvFirstLine(CRJStringByteFlex&)=0;
		virtual bool OnHTTPRecvTotal()=0;
		virtual bool OnHTTPRecvDone();
		virtual bool OnHTTPRecvFailure();
		virtual void OnHTTPReset();
		virtual bool OnHTTPRecvBinaryData(size_t);
		virtual bool OnHTTPHeaderAttribute(const CRJStringByte&,const CRJStringByte&);
		virtual const CRJStringByte& GetHTTPStatusCode(int64_t) {
			return s_clEmpty;
		}
		bool OnHTTPRecvFirst();
		bool OnHTTPRecvHeader();
		bool OnHTTPRecvHeaderString(CRJStringByteFlex&);
		bool OnHTTPRecvHeaderStringLastAttribute();
		bool OnHTTPRecvHeaderTotal();
		bool OnHTTPRecvBinary();
		bool OnHTTPRecvBinaryTotal();
		bool OnHTTPRecvError();
		bool OnHTTPRecvContentLength(const CRJStringByte&);
		static bool OnHTTPRecvContentDesired(const CRJStringByte&,const CRJStringByte&);
		bool OnHTTPAnalyzeVersion();
		inline CMessage& GetHTTPMessage() {
			return m_clHTTPMessage;
		}
	public:
		static CRJStringByte s_clEmpty;
		static CRJStringByte s_clHeaderContentLength;
		static CRJStringByte s_clHeaderAcceptEncoding;
		static CRJStringByte s_clHeaderAcceptLanguage;
		static CRJStringByte s_clHeaderAcceptCharset;
		static CRJStringByte s_clHeaderConnection;
		static CRJStringByte s_clHeaderLastModified;
		static CRJStringByte s_clHeaderUpgrade;
		static CRJStringByte s_clHeaderWebsocketKey;
		static CRJStringByte s_clHeaderWebsocketAccept;
		static CRJStringByte s_clHeaderCookie;
		static CRJStringByte s_clHeaderSetCookie;
		static CRJStringByte s_clHeaderSetCookieExpires;
		static CRJStringByte s_clHeaderContentEncodingGzip;
		static CRJStringByte s_clContentLanguage;
		static CRJStringByte s_clContentType;
		static CRJStringByte s_clConnectionClose;
		static CRJStringByte s_clConnectionKeepAlive;
		static CRJStringByte s_clConnectionUpgrade;
		static CRJStringByte s_clUpgradeWebsocket;
		static CRJStringByte s_clContentMimeJSON;
		static CRJStringByte s_clContentMimeHTML;
		static CRJStringByte s_clContentFileJS;
		static CRJStringByte s_clContentFileCSS;
		static CRJStringByte s_clContentFileJSON;
	protected:
		static CRJStringByte s_clMyProtocol;
		static funcRead s_TableOfFuncRead[SIZE_HTTP_SERVER_INPUT_MODE];
		CMessage m_clHTTPMessage;
		InputMode m_lHTTPInputMode;
		size_t m_lHTTPLength;
		CRJStringByteVarBuffer<128>m_clHTTPLastAttrName;
		CRJStringByteExpanded m_clHTTPLastAttrValue;
		bool m_bHTTPStoreAttributes;
	};
}

