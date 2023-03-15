#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/db/odbc/handleconnection.h>
using namespace rjDb;
std::mutex CRJDBHandleConnection::m_clDBConnect;
SQLSMALLINT CRJDBHandleConnection::m_lDBTrxnResult[2]= {
	SQL_ROLLBACK, SQL_COMMIT
};
CRJDBHandleConnection::CRJDBHandleConnection() {
	m_bDBConnection=false;
	m_hDB=nullptr;
}
CRJDBHandleConnection::~CRJDBHandleConnection() {
	Release();
}
SQLRETURN CRJDBHandleConnection::Create(SQLHENV h) {
	try {
		if (h) return ::SQLAllocHandle(SQL_HANDLE_DBC, h,& m_hDB);
	}
	catch(...) {}
	return SQL_ERROR;
}
SQLRETURN CRJDBHandleConnection::Release() {
	SQLRETURN rc=SQL_SUCCESS;
	try {
		if (m_hDB) {
			rc=::SQLFreeHandle(SQL_HANDLE_DBC, m_hDB);
			switch (rc) {
			case SQL_SUCCESS:
			case SQL_SUCCESS_WITH_INFO:m_hDB=nullptr;
				break;
			}
		}
	}
	catch(...) {}
	return rc;
}
SQLRETURN CRJDBHandleConnection::Connect(const CRJStringByte& clConnect, CRJStringByteVar& clError) {
	std::lock_guard<std::mutex>lg(m_clDBConnect);
	return npConnect(clConnect, clError);
}
SQLRETURN CRJDBHandleConnection::Disconnect() {
	std::lock_guard<std::mutex>lg(m_clDBConnect);
	return npDisconnect();
}
SQLRETURN CRJDBHandleConnection::npConnect(const CRJStringByte& clConnect, CRJStringByteVar& clError) {
	SQLRETURN rc=SQL_ERROR;
	try {
		if (IsDBReady()) {
			SQLSMALLINT v=0;
			rc=::SQLDriverConnect(m_hDB,nullptr,(SQLTCHAR*) clConnect.GetHeadForRead(),(SQLSMALLINT) clConnect.GetLength(),nullptr, 0,& v, SQL_DRIVER_NOPROMPT);
			switch (rc) {
			case SQL_SUCCESS:
			case SQL_SUCCESS_WITH_INFO:m_bDBConnection=true;
				break;
			case SQL_ERROR:break;
			case SQL_NO_DATA:
			case SQL_INVALID_HANDLE:
			case SQL_STILL_EXECUTING:
#ifdef _DEBUG
				CRJDataTools::DebugBreak();
#endif 
				break;
			default:
#ifdef _DEBUG
				CRJDataTools::DebugBreak();
#endif 
				break;
			}
		}
		else {
			CRJStringByte str("DB Handle is not ready");
			clError.Set(str);
#ifdef _DEBUG
			CRJDataTools::DebugBreak();
#endif 
		}
	}
	catch(...) {}
	return rc;
}
SQLRETURN CRJDBHandleConnection::npDisconnect() {
	SQLRETURN rc=SQL_ERROR;
	try {
		if (IsDBReady()) {
			Result(false);
			rc=::SQLDisconnect(m_hDB);
			switch (rc) {
			case SQL_SUCCESS:
			case SQL_SUCCESS_WITH_INFO:m_bDBConnection=false;
				break;
			}
		}
	}
	catch(...) {}
	return rc;
}
SQLRETURN CRJDBHandleConnection::Result(bool d) {
	SQLRETURN rc=SQL_ERROR;
	try {
		if (IsDBReady()) rc=::SQLEndTran(SQL_HANDLE_DBC, m_hDB, CRJDBHandleConnection::m_lDBTrxnResult[d& 1]);
		else rc=SQL_SUCCESS_WITH_INFO;
	}
	catch(...) {}
	return rc;
}

