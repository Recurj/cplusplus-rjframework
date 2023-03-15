#include "stdafx.h"
#include <recurj/rjframework/libs/db/odbc/statement.h>
#include <recurj/rjframework/libs/base/datatools/bytepacker.h>
#include <recurj/rjframework/libs/db/odbc/connection.h>
using namespace rjDb;
CRJStringByte CRJDBStatement::m_clDBConstraintError("23000");
CRJStringByte CRJDBStatement::m_clDBTRXNRollbacked("40001");
CRJStringByte CRJDBStatement::m_clSQLNotDefined("SQL Not defined");
CRJStringByte CRJDBStatement::m_clStatementCompleteDelete("DBStatementCompleteByRelease");
CRJStringByte CRJDBStatement::m_clStatementCompleteClose("DBStatementCompleteByClose");
const CRJStringByte CRJDBStatement::s_SQLSelect("select ");
const CRJStringByte CRJDBStatement::s_SQLFrom(" from ");
const CRJStringByte CRJDBStatement::s_SQLWhere(" where ");
const CRJStringByte CRJDBStatement::s_SQLInsert("insert into ");
const CRJStringByte CRJDBStatement::s_SQLInsertValues(" values ");
const CRJStringByte CRJDBStatement::s_SQLUpdate("update ");
const CRJStringByte CRJDBStatement::s_SQLUpdateSet(" set ");
const CRJStringByte CRJDBStatement::s_SQLDelete("delete from ");
const CRJStringByte CRJDBStatement::s_SQLOrderBy(" order by ");
const CRJStringByte CRJDBStatement::s_SQLGroupBy(" group by ");
const CRJStringByte CRJDBStatement::s_SQLWhereStart(" (");
const CRJStringByte CRJDBStatement::s_SQLWhereLikeEq(" like '%");
const CRJStringByte CRJDBStatement::s_SQLWhereInEq(" in (");
const CRJStringByte CRJDBStatement::s_SQLWhereIntEnd(") ");
const CRJStringByte CRJDBStatement::s_SQLWhereStrEnd("') ");
const CRJStringByte CRJDBStatement::s_SQLWhereLikeEnd("%') ");
const CRJStringByte CRJDBStatement::s_SQLWhereInEnd(")) ");
const CRJStringByte CRJDBStatement::s_SQLAnd(" and ");
const CRJStringByte CRJDBStatement::s_SQLOr(" or ");
const CRJStringByte CRJDBStatement::s_SQLAsc(" asc");
const CRJStringByte CRJDBStatement::s_SQLDesc(" desc");
CRJDBStatement::funcExec CRJDBStatement::m_stFuncTableStatementBaseExecuted[2]= {
	& CRJDBStatement::DBStatementOnExecutedDebugOff,& CRJDBStatement::DBStatementOnExecutedDebugOn
};
CRJDBStatement::CRJDBStatement(CRJDBConnection& clDB,const CRJStringByte& clSQL):
m_clDB(clDB),
m_clSQLString(clSQL) {
	m_lSQLStatementState=0;
}
CRJDBStatement::CRJDBStatement(CRJDBConnection& clDB):
m_clDB(clDB) {
	m_lSQLStatementState=0;
}
CRJDBStatement::~CRJDBStatement() {
	DBStatementClose();
}
bool CRJDBStatement::DBStatementIsResultOK(SQLRETURN v) {
	return m_clDB.IsDBResultOK(v,*this);
}
void CRJDBStatement::DBStatementClose() {
	DBStatementRelease();
	DBStatementDrop();
}
void CRJDBStatement::DBStatementDrop() {
	m_clDBHandleStatement.Release();
}
void CRJDBStatement::DBStatementRelease() {
	try {
		if (m_clDBHandleStatement.IsStatementReady()) {
			::SQLFreeStmt(m_clDBHandleStatement, SQL_CLOSE| SQL_UNBIND| SQL_RESET_PARAMS);
		}
	}
	catch(...) {}
	m_lSQLStatementState&=(~(DB_STATEMENT_IN_CURSOR| DB_STATEMENT_BUFFERS| DB_STATEMENT_PARAMETERS| DB_STATEMENT_PREPARED));
}
void CRJDBStatement::DBStatementOpen() {
	if (!m_clDBHandleStatement.IsStatementReady()) DBStatementAlloc();
}
bool CRJDBStatement::DBStatementIsDebugable(const CRJStringByte& clState) {
	return clState!=CRJDBStatement::m_clDBTRXNRollbacked;
}

bool CRJDBStatement::DBStatementUnBind() {
	try {
		if ((m_clDBHandleStatement.IsStatementReady())&&
			(DBStatementIsBound())) {
			::SQLFreeStmt(m_clDBHandleStatement, SQL_UNBIND);
		}
	}
	catch(...) {}
	m_lSQLStatementState&=(~DB_STATEMENT_BUFFERS);
	return true;
}
bool CRJDBStatement::DBStatementResetParameters() {
	try {
		if ((m_clDBHandleStatement.IsStatementReady())&&
			(DBStatementHasParameters())) {
			::SQLFreeStmt(m_clDBHandleStatement, SQL_RESET_PARAMS);
		}
	}
	catch(...) {}
	m_lSQLStatementState&=(~DB_STATEMENT_PARAMETERS);
	return true;
}
void CRJDBStatement::DBStatementAlloc() {
	if (!DBStatementIsResultOK(m_clDBHandleStatement.Create(m_clDB.GetDBConnectionItSelfConnectionHandle()))) DBStatementDrop();
}
bool CRJDBStatement::DBStatementPrepare() {
	try {
		if ((DBStatementIsAllocated())&&
			(DBStatementIsResultOK(::SQLPrepareA(m_clDBHandleStatement,(SQLCHAR*) m_clSQLString.GetHeadForRead(),(SQLINTEGER) m_clSQLString.GetLength())))&&
			(DBStatementPrepareParameters())) {
			m_lSQLStatementState|=DB_STATEMENT_PREPARED;
			return true;
		}
		else DBStatementRelease();
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementExec() {
	try {
		SQLRETURN rc;
		m_clDB.OnDBExec(*this);
		rc=::SQLExecute(m_clDBHandleStatement);
		(this->* CRJDBStatement::m_stFuncTableStatementBaseExecuted[m_clDB.GetDBConnectionItSelfDebugExecuted()])();
		return (rc==SQL_NO_DATA)?
			DBStatementExecNoData():
			DBStatementIsResultOK(rc);
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementExecNoData() {
	return true;
}
bool CRJDBStatement::DBStatementCancel() {
	try {
		if (m_clDBHandleStatement.IsStatementReady()) {
			return DBStatementIsResultOK(::SQLCancel(m_clDBHandleStatement));
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementDo() {
	return (DBStatementIsPrepared())?
		DBStatementExec():
		false;
}
bool CRJDBStatement::DBStatementIsPrepared() {
	return (m_lSQLStatementState& DB_STATEMENT_PREPARED)||
		DBStatementPrepare();
}
bool CRJDBStatement::DBStatementSetTimeout(int64_t lSec) {
	try {
		if (m_clDBHandleStatement.IsStatementReady()) {
			SQLULEN v=(lSec>0)?
				(SQLULEN) lSec:0;
#pragma warning(suppress: 6387)
			return DBStatementIsResultOK(::SQLSetStmtAttr(m_clDBHandleStatement, SQL_ATTR_QUERY_TIMEOUT,(SQLPOINTER) v, 0));
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementParameter(size_t lParam, CRJDBSQLSize& arg) {
	try {
		if (!m_clDB.HasSupport64BitsFields()) {
			arg.m_lIndicator=sizeof(uint32_t);
			return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 10, 0,& arg.m_sValue,sizeof(size_t),& arg.m_lIndicator));
		}
		else {
			arg.m_lIndicator=sizeof(size_t);
			return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 10, 0,& arg.m_sValue,sizeof(size_t),& arg.m_lIndicator));
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementParameter(size_t lParam, CRJDBSQLInt64& arg) {
	try {
		if (!m_clDB.HasSupport64BitsFields()) {
			arg.m_lIndicator=sizeof(int32_t);
			return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 10, 0,& arg.m_lValue,sizeof(int32_t),& arg.m_lIndicator));
		}
		else {
			arg.m_lIndicator=sizeof(int64_t);
			return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 10, 0,& arg.m_lValue,sizeof(int64_t),& arg.m_lIndicator));
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementParameter(size_t lParam, CRJDBSQLInt32& arg) {
	try {
		arg.m_lIndicator=sizeof(int32_t);
		return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 10, 0,& arg.m_lValue,sizeof(int32_t),& arg.m_lIndicator));
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementParameter(size_t lParam, CRJDBSQLStringByte& arg) {
	try {
		CRJStringByteVar& clValue=arg.GetString();
		SQLLEN lLen=static_cast<SQLLEN>(clValue.GetSize());
		arg.m_lIndicator=static_cast<SQLLEN>(clValue.GetLength());
		return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,(SQLUINTEGER) lLen, 0,(SQLPOINTER) clValue.GetHeadForRead(), lLen,& arg.m_lIndicator));
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementSetParameterBytes(size_t lParam, uint8_t* pBuf, size_t lLen, SQLLEN& lExecLen) {
	try {
		lExecLen=SQL_NTS;
		return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,(SQLUINTEGER) lLen, 0,(SQLPOINTER) pBuf, lLen,& lExecLen));
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementSetParameterBytesVar(size_t lParam, uint8_t* pBuf, size_t lLen, SQLLEN& lExecLen) {
	try {
		lExecLen=SQL_NTS;
		return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,(SQLUINTEGER) lLen, 0,(SQLPOINTER) pBuf, lLen,& lExecLen));
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementSetParameterString(size_t lParam, CRJStringByteVar& str, SQLLEN& lExecLen) {
	try {
		SQLLEN lLen=static_cast<SQLLEN>(str.GetSize());
		lExecLen=static_cast<SQLLEN>(str.GetLength());
		return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,(SQLUINTEGER) lLen, 0,(SQLPOINTER) str.GetHeadForRead(), lLen,& lExecLen));
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementSetParameterString(size_t lParam, CRJStringUnicodeVar& str, SQLLEN& lExecLen) {
	try {
		SQLULEN lLen=static_cast<SQLULEN>(str.GetSize());
		mdbStatementSetUnicodeParamLength(str, lExecLen);
		return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR,(SQLUINTEGER) lLen, 0,(SQLPOINTER) str.GetHeadForRead(),(SQLLEN)(lLen<<1),& lExecLen));
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementSetParameterStringVar(size_t lParam, CRJStringByteVar& str, SQLLEN& lExecLen) {
	try {
		SQLULEN lLen=static_cast<SQLULEN>(str.GetSize());
		lExecLen=static_cast<SQLLEN>(str.GetLength());
		return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, lLen, 0,(SQLPOINTER) str.GetHeadForRead(),(SQLLEN) lLen,& lExecLen));
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementSetParameterStringVar(size_t lParam, CRJStringUnicodeVar& str, SQLLEN& lExecLen) {
	try {
		SQLULEN lLen=static_cast<SQLULEN>(str.GetSize());
		mdbStatementSetUnicodeParamLength(str, lExecLen);
		return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, lLen, 0,(SQLPOINTER) str.GetHeadForRead(),(SQLLEN)(lLen<<1),& lExecLen));
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementSetParameterInt64(size_t lParam, int64_t& val, SQLLEN& lExecLen) {
	try {
		if (!m_clDB.HasSupport64BitsFields()) {
			lExecLen=sizeof(int32_t);
			return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 10, 0,& val,sizeof(int32_t),& lExecLen));
		}
		else {
			lExecLen=sizeof(int64_t);
			return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 10, 0,& val,sizeof(int64_t),& lExecLen));
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementSetParameterInt32(size_t lParam, int32_t& val, SQLLEN& lExecLen) {
	try {
		lExecLen=sizeof(int32_t);
		return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_BIGINT, 10, 0,& val,sizeof(int32_t),& lExecLen));
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementSetParameterSize(size_t lParam, size_t& val, SQLLEN& lExecLen) {
	try {
		if (!m_clDB.HasSupport64BitsFields()) {
			lExecLen=sizeof(uint32_t);
			return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 10, 0,& val,sizeof(size_t),& lExecLen));
		}
		else {
			lExecLen=sizeof(size_t);
			return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 10, 0,& val,sizeof(size_t),& lExecLen));
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementSetParameterTime(size_t lParam, time_t& val, SQLLEN& lExecLen) {
	try {
		if (!m_clDB.HasSupport64BitsFields()) {
			lExecLen=sizeof(int32_t);
			return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 10, 0,& val,sizeof(int32_t),& lExecLen));
		}
		else {
			lExecLen=sizeof(int64_t);
			return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 10, 0,& val,sizeof(time_t),& lExecLen));
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementSetParameterDouble(size_t lParam,double& val, SQLLEN& lExecLen) {
	try {
		lExecLen=sizeof(double);
		return DBStatementIsResultOK(::SQLBindParameter(m_clDBHandleStatement,(SQLUSMALLINT) lParam, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 15, 0,& val,sizeof(double),& lExecLen));
	}
	catch(...) {}
	return false;
}
void CRJDBStatement::DBStatementComplete(int64_t v) {
	switch (v) {
	case SQL_CB_DELETE:DBStatementCompleteByRelease();
		break;
	case SQL_CB_CLOSE:DBStatementCompleteByClose();
		break;
	}
}
void CRJDBStatement::DBStatementCompleteByRelease() {
	auto call=[this](CRJLogger& clLogger)->void {
		clLogger.Text(CRJDBStatement::m_clStatementCompleteDelete, m_clSQLString);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartDB, cLoggerLevelError, cb);
	DBStatementRelease();
}
void CRJDBStatement::DBStatementCompleteByClose() {
	if (m_clDBHandleStatement.IsStatementReady()) {
		auto call=[this](CRJLogger& clLogger)->void {
			clLogger.Text(CRJDBStatement::m_clStatementCompleteClose, m_clSQLString);
		};
		CRJLoggerFunctor<decltype(call)>cb(call);
		CRJLoggerThread::Check(cLoggerPartDB, cLoggerLevelError, cb);
	}
}
bool CRJDBStatement::DBStatementPrepareParameters() {
	return true;
}
void CRJDBStatement::OnDBError(bool bCritical) {
	try {
		m_clDB.OnDBErrorOnStatement(*this, bCritical);
		if (!m_clDB.IsDBConnect()) {
			m_clDB.DBReconnect();
		}
	}
	catch(...) {}
}
void CRJDBStatement::DBStatementSetSQL(const CRJStringByte& clSql) {
	m_clSQLString.Set(clSql);
}
bool CRJDBStatement::DBStatementResult(bool bResult) {
	return m_clDB.DBResult(bResult);
}
bool CRJDBStatement::DBStatementGetColumnInfo(size_t lColumn, SRJDBColumnInfo& stInfo) {
	try {
		if (m_clDBHandleStatement.IsStatementReady()) {
			return DBStatementIsResultOK(::SQLDescribeColA(m_clDBHandleStatement,(SQLUSMALLINT) lColumn, stInfo.szColumnName,sizeof(stInfo.szColumnName),& stInfo.lNameLength,& stInfo.lDataType,& stInfo.lColumnSize,& stInfo.lDecimalDigits,& stInfo.lNullable));
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBStatement::DBStatementGetColumnCount(size_t& lColumnCount) {
	try {
		if (m_clDBHandleStatement.IsStatementReady()) {
			SQLSMALLINT cols=0;
			if (DBStatementIsResultOK(::SQLNumResultCols(m_clDBHandleStatement,& cols))) {
				lColumnCount=(size_t) cols;
				return true;
			}
		}
	}
	catch(...) {}
	return false;
}
int64_t CRJDBStatement::DBStatementGetRowAffected() {
	try {
		if (m_clDBHandleStatement.IsStatementReady()) {
			SQLLEN l=0;
			if (DBStatementIsResultOK(::SQLRowCount(m_clDBHandleStatement,& l))) {
				return l;
			}
		}
	}
	catch(...) {}
	return -1;
}
bool CRJDBStatement::DBStatementIsAllocated() {
	if (!m_clDBHandleStatement.IsStatementReady()) {
		if (!m_clDB.IsDBConnect()) m_clDB.DBReconnect();
		else DBStatementAlloc();
		return m_clDBHandleStatement.IsStatementReady();
	}
	return false;
}
size_t CRJDBStatement::DBStatementBinaryInput(CRJStringByteVar& clStr,const uint8_t* pData, size_t lLen) {
	try {
		size_t l=lLen* 2, lDstLen=clStr.GetSize();
		if (l>lDstLen) {
			l=lDstLen&(~1);
			lLen=l>>1;
		}
		if (lLen>0) {
			if (pData) CRJBytePacker::UnpackMachine((uint8_t*) clStr.GetHeadForUpdate(), pData, lLen);
			else _MEMSET_BYTE_POINTER(clStr.GetHeadForUpdate(), 0x30, l);
		}
		clStr.SetLength(l);
		return l;
	}
	catch(...) {}
	clStr.SetEmpty();
	return 0;
}
size_t CRJDBStatement::DBStatementBinaryInput(CRJDBSQLStringByte& s,const uint8_t* pData, size_t lLen) {
	size_t sLen=DBStatementBinaryInput(s.GetString(), pData, lLen);
	s.SetLength(sLen);
	return sLen;
}
void CRJDBStatement::DBStatementOnExecutedDebugOff() {}
void CRJDBStatement::DBStatementOnExecutedDebugOn() {
	auto call=[this](CRJLogger& clLogger)->void {
		DBStatementDebugExecuted(clLogger);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartDB, cLoggerLevelInfo, cb);
}
void CRJDBStatement::DBStatementDebugExecuted(CRJLogger&) {}
int64_t CRJDBStatement::DBStatementIsBreaked() const {
	return m_clDB.IsDBQueryWasBreaked();
}

