#include "stdafx.h"
#include <recurj/rjframework/libs/db/odbc/connection.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/base/file/filereadertext.h>
#include <recurj/rjframework/libs/db/odbc/cursor.h>
#include <recurj/rjframework/libs/db/odbc/statementsql.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <recurj/rjframework/libs/base/logger/loggerthread.h>
#include <recurj/rjframework/libs/base/strings/byte/error.h>
#include <recurj/rjframework/errorcodes.h>
#pragma warning (disable : 4312)
using namespace rjDb;
const char CRJDBConnection::c_szDBStateHY00[8]="HY00";
const char CRJDBConnection::c_szDBState4200[8]="4200";
const CRJStringByte CRJDBConnection::m_clPhraseOdbcError("ODBC Error ");
const CRJStringByte CRJDBConnection::m_clPhraseOdbcNative("[Native ");
std::mutex CRJDBConnection::m_clDBAccessEnv;
CRJDBConnection::funcStatusUpdated CRJDBConnection::m_clFuncOnOnStatusComplete[2]= {
	& CRJDBConnection::OnConnStatusNoChanged,& CRJDBConnection::OnConnStatusWasChanged
};
SQLHENV CRJDBConnection::m_hDBApplEnv=nullptr;
bool CRJDBConnection::DPAppWasOpened() {
	return m_hDBApplEnv!=nullptr;
}
void CRJDBConnection::DBError(int64_t lErr, int32_t lLevel) {
	auto call=[lErr](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Message(err.Error(lErr));
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartDB, lLevel, cb);
}
void CRJDBConnection::DBError(int64_t lErr,const CRJStringByte& s, int32_t lLevel) {
	auto call=[& s, lErr](CRJLogger& clLogger)->void {
		CRJStringError err;
		clLogger.Text(err.Error(lErr), s);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartDB, lLevel, cb);
}
bool CRJDBConnection::DBAppOpen() {
	try {
		if (m_hDBApplEnv!=nullptr) return true;
		else {
			SQLHENV h=nullptr;
			SQLRETURN v=::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE,& h);
			if ((v==SQL_SUCCESS)||
				(v==SQL_SUCCESS_WITH_INFO)) {
				::SQLSetEnvAttr(h, SQL_ATTR_ODBC_VERSION,(SQLPOINTER*) SQL_OV_ODBC3, 0);
				m_hDBApplEnv=h;
				return true;
			}
		}
	}
	catch(...) {}
	return false;
}
void CRJDBConnection::DBAppClose() {
	try {
		if (m_hDBApplEnv) {
			switch (::SQLFreeHandle(SQL_HANDLE_ENV, m_hDBApplEnv)) {
			case SQL_SUCCESS:
			case SQL_SUCCESS_WITH_INFO:m_hDBApplEnv=nullptr;
				break;
			}
		}
	}
	catch(...) {}
}
bool CRJDBConnection::IsDBResultOK(SQLRETURN r, IRJDBError& dbObj) {
	if (r==SQL_SUCCESS) return true;
	else if (r==SQL_SUCCESS_WITH_INFO) {
		if (DBShowWarning()) dbObj.OnDBError(false);
		return true;
	}
	else dbObj.OnDBError(true);
	return false;
}
int32_t CRJDBConnection::GetDBConnectionItSelfDebugLevel(bool bCritical) {
	return (bCritical)?
		cLoggerLevelError:cLoggerLevelWarning;
}
CRJDBConnection::CRJDBConnection(IRJDBConnectionInfo& dbInfo):
	m_clDBConnectionInfo(dbInfo) {
	DBInit();
}
CRJDBConnection::CRJDBConnection(IRJDBConnectionInfo& dbInfo,const CRJStringByte& clDBName):
	m_clDBConnectionInfo(dbInfo),
m_clDBReference(clDBName) {
	DBInit();
}
CRJDBConnection::~CRJDBConnection() {
	DBDriverDisconnect();
}
void CRJDBConnection::DBInit() {
	m_bSupport64BitsFields=true;
	m_sDBState=0;
	m_lDBReconnectFlag=-1;
	m_lDBErrorNativeCode=0;
	m_sDBDebugFetched=0;
	m_sDBDebugExecuted=0;
	m_lDBBehavior[DB_Behavior_Commit]=SQL_CB_DELETE;
	m_lDBBehavior[DB_Behavior_Rollback]=SQL_CB_DELETE;
}
void CRJDBConnection::DBSetDebug() {
	size_t lDBDebug=CRJLoggerThread::GetLevel(cLoggerPartDB);
	m_sDBDebugExecuted=(lDBDebug>=cLoggerLevelInfo)?
		1:0;
	m_sDBDebugFetched=(lDBDebug>=cLoggerLevelTrace)?
		1:0;
}
void CRJDBConnection::OnDBConnectStatus() {
	m_sDBState|=DB_State_WasUpdatedStatus;
	OnDBShowConnectStatus();
}
void CRJDBConnection::OnDBShowConnectStatus() {
	auto call=[this](CRJLogger& clLogger)->void {
		clLogger.Message((!IsDBConnect())?
			"DB Disconnected":"DB Connected");
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartDB, GetDBConnectionItSelfDebugStatusLevel(), cb);
}
void CRJDBConnection::SetDBAutoCommit(bool b) {
	DBUpdateState(b, DB_State_AutoCommit);
	SetOdbcAutoCommit();
}
void CRJDBConnection::SetDBReadOnly(bool b) {
	DBUpdateState(b, DB_State_ReadOnly);
	SetOdbcReadOnly();
};
int32_t CRJDBConnection::GetDBConnectionItSelfDebugStatusLevel() {
	return cLoggerLevelError;
}
SQLHDBC CRJDBConnection::GetDBConnectionItSelfConnectionHandle() {
	return m_clDBHandle;
}
CRJDBConnection& CRJDBConnection::DBReconnect() {
	m_lDBReconnectFlag++;
	if (!m_lDBReconnectFlag) {
		if (DBConnect()) {
			(this->* CRJDBConnection::m_clFuncOnOnStatusComplete[m_sDBState& 1])();
		}
	}
	m_lDBReconnectFlag--;
	return *this;
}
bool CRJDBConnection::DBConnect() {
	CRJStringByteFlex clConnect, clSetup;
	SDBConnectionOptions stOpts;
	return (m_clDBConnectionInfo.DBConnectionInfoGet(m_clDBReference, clConnect, clSetup, stOpts))?
		DBConnect(clConnect, clSetup, stOpts):
		false;
}
bool CRJDBConnection::DBConnect(const CRJStringByte& clConnect,const CRJStringByte& clSetup, SDBConnectionOptions& stOpts) {
	stOpts.Validate();
	while (stOpts.sDBConnectRetry>0) {
		stOpts.sDBConnectRetry--;
		if (DBConnectStep(clConnect, clSetup, stOpts)) return true;
		if (DBNeedBreak(stOpts.sDBConnectPause))break;
	}
	return false;
}
bool CRJDBConnection::DBNeedBreak(int64_t) {
	return false;
}
bool CRJDBConnection::DBConnectStep(const CRJStringByte& clConnect,const CRJStringByte& clInit,const SDBConnectionOptions& stOpts) {
	DBDisconnect();
	if (DBAlloc(stOpts)) {
		if (!DBDriverConnect(clConnect)) DBDriverDisconnect();
		else {
			DBSetDebug();
			OnDBConnected(clInit, stOpts);
			OnDBConnectStatus();
			return true;
		}
	}
	else DBFree();
	return false;
}
void CRJDBConnection::DBDisconnect() {
	bool b=m_clDBHandle.GetDBConnection();
	DBDriverDisconnect();
	if (b) OnDBConnectStatus();
}
void CRJDBConnection::OnDBExec(CRJDBStatement& clSQL) {
	auto call=[this,& clSQL](CRJLogger& clLogger)->void {
		clLogger.Text("Running SQL", clSQL.DBStatementGetSQL());
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	m_clBreakFlag.latchClose();
	CRJLoggerThread::Check(cLoggerPartDB, cLoggerLevelInfo, cb);
}
void CRJDBConnection::DBQueryBreak() {
	m_clBreakFlag.latchOpen();
}
void CRJDBConnection::OnDBError(bool bCritical) {
	OnDBErrorUnknown();
	if (m_hDBApplEnv) {
		if (m_clDBHandle.IsDBReady()?
			OnDBErrorInfo(m_clDBHandle, SQL_HANDLE_DBC):
			OnDBErrorInfoEnv()) {
			OnDBShowError(bCritical);
			if (!IsDBConnectionAlive()) DBDisconnect();
		}
	}
}
bool CRJDBConnection::OnDBErrorOnStatement(CRJDBStatement& clSQL,bool bCritical) {
	OnDBErrorUnknown();
	if (m_hDBApplEnv) {
		if (OnDBErrorInfo(clSQL.DBStatementGet(), SQL_HANDLE_STMT)) {
			OnDBShowErrorOnStatement(clSQL, bCritical);
			if (!IsDBConnectionAlive()) {
				clSQL.DBStatementClose();
				DBDisconnect();
			}
			return true;
		}
		return false;
	}
	return false;
}
void CRJDBConnection::OnDBShowError(bool bCritical) {
	auto call=[this](CRJLogger& clLogger)->void {
		clLogger.Message(DBFormatError());
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartDB, CRJDBConnection::GetDBConnectionItSelfDebugLevel(bCritical), cb);
}
void CRJDBConnection::OnDBShowErrorOnStatement(CRJDBStatement& clSQL,bool bCritical) {
	const CRJStringByte& clState=GetDBErrorState();
	if (clSQL.DBStatementIsDebugable(clState)) {
		auto call=[this,& clSQL](CRJLogger& clLogger)->void {
			CRJLoggerFormat clFormater(clLogger, DBFormatError());
			clLogger.CatLabel(" on running SQL ").Cat(clSQL.DBStatementGetSQL());
		};
		CRJLoggerFunctor<decltype(call)>cb(call);
		CRJLoggerThread::Check(cLoggerPartDB, CRJDBConnection::GetDBConnectionItSelfDebugLevel(bCritical), cb);
	}
}
bool CRJDBConnection::HasDBError(CRJDBStatement& clSQL) {
	if (m_clDBState.GetLength()>0) return true;
	OnDBErrorUnknown();
	if (m_hDBApplEnv) return OnDBErrorInfo(clSQL.DBStatementGet(), SQL_HANDLE_STMT);
	return false;
}
bool CRJDBConnection::HasDBErrorConstraint(CRJDBStatement& clSQL) {
	if (HasDBError(clSQL)) return clSQL.DBStatementIsConstraintError(m_clDBState);
	return false;
}
void CRJDBConnection::OnDBErrorUnknown() {
	m_lDBErrorNativeCode=-1;
	m_clDBState.SetEmpty();
	m_clDBErrorNative.SetEmpty();
}
bool CRJDBConnection::OnDBErrorInfo(SQLHSTMT h, SQLSMALLINT t) {
	try {
		SQLSMALLINT l;
		SQLINTEGER lNativeError;
		SQLRETURN rc=::SQLGetDiagRecA(t, h, 1,(SQLCHAR*) m_clDBState.GetHeadForUpdate(),& lNativeError,(SQLCHAR*) m_clDBErrorNative.GetHeadForUpdate(),(SQLSMALLINT) m_clDBErrorNative.GetSize()-1,& l);
		if (rc!=SQL_NO_DATA) {
			m_lDBErrorNativeCode=lNativeError;
			m_clDBState.SetLengthSafed(5);
			m_clDBErrorNative.CalcCLength();
			return true;
		}
		else m_clDBErrorNative.SetSingle('$');
	}
	catch(...) {}
	return false;
}
bool CRJDBConnection::OnDBErrorInfoEnv() {
	std::lock_guard<std::mutex>lg(CRJDBConnection::m_clDBAccessEnv);
	return OnDBErrorInfo(m_hDBApplEnv, SQL_HANDLE_ENV);
}
bool CRJDBConnection::DBDriverConnect(const CRJStringByte& clConnect) {
	if (m_clDBHandle.GetDBConnection()) return true;
	CDBStringOdbcError clBufOdbcErrorFormated;
	SQLRETURN v=m_clDBHandle.Connect(clConnect, m_clDBErrorNative);
	if (IsDBResultOK(v,*this)) return true;
	else if (m_clDBState.GetLength()>0) {
		clBufOdbcErrorFormated.Set(m_clDBErrorFormated);
		if (!(DBFormatError()==clBufOdbcErrorFormated)) {
			auto call=[this](CRJLogger& clLogger)->void {
				clLogger.Message(m_clDBErrorFormated);
			};
			CRJLoggerFunctor<decltype(call)>cb(call);
			CRJLoggerThread::Check(cLoggerPartDB, CRJDBConnection::GetDBConnectionItSelfDebugLevel(true), cb);
		}
	}
	DBNoError();
	return false;
}
void CRJDBConnection::DBDriverDisconnect() {
	m_clDBHandle.Disconnect();
	DBFree();
}
bool CRJDBConnection::DBResult(bool d) {
	OnDBStatementsResult(m_lDBBehavior[d?
		1:0]);
	switch (m_clDBHandle.Result(d)) {
	case SQL_SUCCESS:
	case SQL_SUCCESS_WITH_INFO: return true;
	default:
		if (d) OnDBStatementsResult(m_lDBBehavior[DB_Behavior_Rollback]);
		break;
	}
	DBResetCursors();
	return false;
}
bool CRJDBConnection::DBResultAfterExec(bool rc) {
	DBResult(rc);
	return rc;
}
void CRJDBConnection::OnDBStatementsResult(int64_t v) {
	for (auto& elem:m_clDBCursors) elem.DBStatementComplete(v);
}
void CRJDBConnection::DBResetCursors() {
	m_clDBCursors.Reset();
}
void CRJDBConnection::SetDBTraceOn(const char* p) {
	try {
		if (m_clDBHandle.IsDBReady()) {
			::SQLSetConnectAttr(m_clDBHandle, SQL_ATTR_TRACEFILE,(SQLPOINTER) p, SQL_NTS);
			::SQLSetConnectAttr(m_clDBHandle, SQL_ATTR_TRACE,(SQLPOINTER) SQL_OPT_TRACE_ON, 0);
		}
	}
	catch(...) {}
}
void CRJDBConnection::SetDBTraceOff() {
	try {
		if (m_clDBHandle.IsDBReady()) {
			::SQLSetConnectAttr(m_clDBHandle, SQL_ATTR_TRACE, SQL_OPT_TRACE_OFF, 0);
		}
	}
	catch(...) {}
}
void CRJDBConnection::DBFree() {
	std::lock_guard<std::mutex>lg(CRJDBConnection::m_clDBAccessEnv);
	m_clDBHandle.Release();
}
SQLRETURN CRJDBConnection::DBAllocHandle() {
	std::lock_guard<std::mutex>lg(CRJDBConnection::m_clDBAccessEnv);
	return m_clDBHandle.Create(m_hDBApplEnv);
}
bool CRJDBConnection::DBAlloc(const SDBConnectionOptions& stOpts) {
	try {
		if (IsDBResultOK(DBAllocHandle(),*this)&&
			m_clDBHandle.IsDBReady()) {
			IsDBResultOK(SQLSetConnectAttr(m_clDBHandle, SQL_ATTR_CONNECTION_TIMEOUT,(SQLPOINTER) stOpts.sDBTimeout, 0),*this);
			SetOdbcAutoCommit();
			SetOdbcReadOnly();
			DBResult(true);
			if (stOpts.sDBPacketSize>0) IsDBResultOK(SQLSetConnectAttr(m_clDBHandle, SQL_ATTR_PACKET_SIZE,(SQLPOINTER) stOpts.sDBPacketSize, 0),*this);
			return true;
		}
	}
	catch(...) {}
	return false;
}
void CRJDBConnection::DBPushCursor(CRJDBCursor& clCursor) {
	m_clDBCursors.Bot(clCursor);
}
void CRJDBConnection::DBPopCursor(CRJDBCursor& clCursor) {
	m_clDBCursors.Remove(clCursor);
}
void CRJDBConnection::OnDBConnected(const CRJStringByte& clInit,const SDBConnectionOptions& stOpts) {
	try {
		if (m_clDBHandle.IsDBReady()) {
			SQLSMALLINT v=0;
			SQLUINTEGER level=0;
			char tmp[256]= {
				0x00
			};
			IsDBResultOK(SQLGetInfo(m_clDBHandle, SQL_DEFAULT_TXN_ISOLATION,& level,sizeof(level),& v),*this);
			if ((stOpts.sDBTrxnIsolation)&&
				(stOpts.sDBTrxnIsolation!=level)) {
				if (IsDBResultOK(SQLSetConnectAttr(m_clDBHandle, SQL_ATTR_TXN_ISOLATION,(SQLPOINTER) stOpts.sDBTrxnIsolation, 0),*this)) {
					auto call=[this,& stOpts](CRJLogger& clLogger)->void {
						clLogger.Size("Set isolation level", stOpts.sDBTrxnIsolation);
					};
					CRJLoggerFunctor<decltype(call)>cb(call);
					CRJLoggerThread::Check(cLoggerPartDB, cLoggerLevelError, cb);
				}
			}
			IsDBResultOK(SQLGetInfo(m_clDBHandle, SQL_NEED_LONG_DATA_LEN,(SQLPOINTER) tmp,sizeof(tmp)-2,& v),*this);
			IsDBResultOK(SQLGetInfo(m_clDBHandle, SQL_CURSOR_COMMIT_BEHAVIOR,& m_lDBBehavior[DB_Behavior_Commit],sizeof(m_lDBBehavior[DB_Behavior_Commit]),& v),*this);
			IsDBResultOK(SQLGetInfo(m_clDBHandle, SQL_CURSOR_ROLLBACK_BEHAVIOR,& m_lDBBehavior[DB_Behavior_Rollback],sizeof(m_lDBBehavior[DB_Behavior_Rollback]),& v),*this);
			SetDBReadOnly(stOpts.bReadOnly);
			if (!stOpts.bReadOnly) SetDBAutoCommit(stOpts.bAutoCommit);
			m_bSupport64BitsFields=stOpts.bSupport64BitsFields;
			OnDBInitSeassion(clInit);
			OnDBInitApp();
		}
	}
	catch(...) {}
}
void CRJDBConnection::OnDBInitApp() {}
void CRJDBConnection::OnDBInitSeassion(const CRJStringByte& cl) {
	if (cl.GetLength()>0) {
		CRJStringByteExpanded clInit;
		uint8_t* p=clInit.Set(cl).GetHeadForUpdate();
		if (p[0]=='@') {
			CRJStringUnicodeDefault s;
			CRJFileName fn(CRJEncoding::translateUtf8ToUnicode(clInit, s).TruncLeft(1));
			DBInitSessionFromFile(fn);
		}
		else {
			CDBStatementSQLExist sql(clInit,*this);
			DBInitSessionRunSql(sql);
		}
	}
}
void CRJDBConnection::DBQueryProcess(bool) {}
bool CRJDBConnection::IsDBConnectionAlive() {
	try {
		if (!m_clDBHandle.IsDBReady()) return false;
		else if (((m_clDBState[0]=='0')&&
			(m_clDBState[1]=='8'))||
			((*((int64_t*) m_clDBState.GetHeadForRead())==*((int64_t*) c_szDBStateHY00))&&
			(m_clDBState[4]=='0'))||
			((*((int64_t*) m_clDBState.GetHeadForRead())==*((int64_t*) c_szDBState4200))&&
			(m_clDBState[4]=='0'))) return false;
		else {
			SQLUINTEGER cd=SQL_CD_FALSE;
			SQLINTEGER len=sizeof(cd);
			switch (::SQLGetConnectAttr(m_clDBHandle, SQL_ATTR_CONNECTION_DEAD,& cd, SQL_IS_UINTEGER,& len)) {
			case SQL_SUCCESS:
			case SQL_SUCCESS_WITH_INFO: return (cd!=SQL_CD_TRUE);
				break;
			}
		}
	}
	catch(...) {}
	return true;
}
void CRJDBConnection::DBInitSessionRunSql(CRJDBSQL& sql) {
	DBResult(sql.SQLStatementExecute());
}
void CRJDBConnection::DBInitSessionFromFile(CRJFileName& clFileName) {
	CRJFileReaderText fh;
	if (fh.OpenEncoding(clFileName)) {
		CRJDBSQLTemplate<CRJStringByteLong>sql(*this);
		while (fh.ReadlnFormat(sql.GetSQLBuffer())) {
			DBInitSessionRunSql(sql);
		}
	}
}
void CRJDBConnection::DBNoError() {
	m_clDBState.SetEmpty();
	m_clDBErrorNative.SetEmpty();
	m_lDBErrorNativeCode=0;
}
CRJStringByte& CRJDBConnection::DBFormatError() {
	return m_clDBErrorFormated.Set(CRJDBConnection::m_clPhraseOdbcError).Cat(m_clDBState).Cat(CRJDBConnection::m_clPhraseOdbcNative).CatInt64(m_lDBErrorNativeCode).CatSingle(':').Cat(m_clDBErrorNative).CatSingle(']').Replace(0x0D, 0x20).Replace(0x0A, 0x20);
}
void CRJDBConnection::OnDBConnStatusWasChanged() {}
void CRJDBConnection::OnConnStatusNoChanged() {}
void CRJDBConnection::OnConnStatusWasChanged() {
	OnDBConnStatusWasChanged();
	m_sDBState&=(~DB_State_WasUpdatedStatus);
}
bool CRJDBConnection::SetOdbcAutoCommit() {
	SQLPOINTER v=(SQLPOINTER)(m_sDBState& DB_State_AutoCommit?
		SQL_AUTOCOMMIT_ON:SQL_AUTOCOMMIT_OFF);
	return IsDBResultOK(SQLSetConnectAttr(m_clDBHandle, SQL_ATTR_AUTOCOMMIT, v, 0),*this);
}
bool CRJDBConnection::SetOdbcReadOnly() {
	SQLPOINTER v=(SQLPOINTER)(m_sDBState& DB_State_ReadOnly?
		SQL_MODE_READ_ONLY:SQL_MODE_READ_WRITE);
	return IsDBResultOK(SQLSetConnectAttr(m_clDBHandle, SQL_ATTR_ACCESS_MODE, v, 0),*this);
}
bool CRJDBConnection::SQLExec(const CRJStringByte& s) {
	CRJDBStatement sql(*this, s);
	return sql.DBStatementDo();
}

