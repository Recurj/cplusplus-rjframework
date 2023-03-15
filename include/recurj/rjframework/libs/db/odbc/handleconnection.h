#pragma once
#include <mutex>
#include <recurj/rjframework/libs/base/datatools/atomicnumber.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/db/db.h>
namespace rjDb {
	class CRJDBHandleConnection {
	public:
		CRJDBHandleConnection();
		virtual~CRJDBHandleConnection();
		operator SQLHDBC() const {
			return m_hDB;
		}
		inline bool IsDBReady() const {
			return m_hDB;
		}
		inline bool GetDBConnection() const {
			return m_bDBConnection;
		}
		SQLRETURN Create(SQLHENV);
		SQLRETURN Connect(const CRJStringByte&, CRJStringByteVar&);
		SQLRETURN Disconnect();
		SQLRETURN Release();
		SQLRETURN Result(bool);
	private:
		SQLRETURN npConnect(const CRJStringByte&, CRJStringByteVar&);
		SQLRETURN npDisconnect();
	protected:
		static std::mutex m_clDBConnect;
		static SQLSMALLINT m_lDBTrxnResult[2];
		SQLHDBC m_hDB;
		bool m_bDBConnection;
	};
}

