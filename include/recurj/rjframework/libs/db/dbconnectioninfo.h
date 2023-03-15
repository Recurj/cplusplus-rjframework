#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/strings/byte/value.h>
#include <recurj/rjframework/modules/moddb.h>
#include <shared_mutex>
using namespace rjBase;
namespace rjDb {
	struct RJDB_API SDBConnectionOptions {
		SDBConnectionOptions();
		SDBConnectionOptions(const SDBConnectionOptions&);
		void Validate();
		SDBConnectionOptions& ConfigReadonly();
		size_t sDBConnectPause, sDBConnectRetry, sDBTimeout, sDBPacketSize, sDBTrxnIsolation;
		bool bSupport64BitsFields, bAutoCommit, bReadOnly;
	};
	class IRJDBConnectionInfo {
	public:
		virtual bool DBConnectionInfoGet(const CRJStringByte&, CRJStringByteFlex&, CRJStringByteFlex&, SDBConnectionOptions&)=0;
		virtual bool DBConnectionInfoAdd(const CRJStringByte&,const CRJStringByte&,const CRJStringByte&, SDBConnectionOptions&)=0;
	};
	class RJDB_API CRJDBConnectInfo:public IRJDBConnectionInfo {
	public:
		CRJDBConnectInfo()noexcept;
		CRJDBConnectInfo(const CRJDBConnectInfo&);
		CRJDBConnectInfo(const CRJStringByte&,const CRJStringByte&,const SDBConnectionOptions&);
		bool DBConnectionInfoGet(const CRJStringByte&, CRJStringByteFlex&, CRJStringByteFlex&, SDBConnectionOptions&) override;
		bool DBConnectionInfoAdd(const CRJStringByte&,const CRJStringByte&,const CRJStringByte&, SDBConnectionOptions&) override;
		virtual~CRJDBConnectInfo();
		void operator=(const CRJDBConnectInfo&);
		CRJStringByteExpanded m_clDBConnect, m_clDBSetup;
		SDBConnectionOptions m_stOptions;
	};
}

