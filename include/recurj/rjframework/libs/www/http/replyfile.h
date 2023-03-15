#pragma once
#include <recurj/rjframework/libs/base/file/filereader.h>
#include <recurj/rjframework/libs/www/http/reply.h>
namespace rjWWW {
	class RJWWW_API CHTTPReplyFile:public CHTTPReply {
	public:
		CHTTPReplyFile();
		virtual~CHTTPReplyFile();
		virtual size_t GetBinaryDataLength() {
			return m_sFileLength;
		};
		virtual bool GetBinaryData(CRJByteWriter&, size_t);
		bool Ready(CRJFileName&,bool);
	protected:
		CRJFileReader m_clFile;
		size_t m_sFileLength;
	};
}

