#include "stdafx.h"
#include <recurj/rjframework/libs/db/odbc/cursor.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/datatools/bytepacker.h>
#include <recurj/rjframework/datalength.h>
#include <recurj/rjframework/libs/db/odbc/connection.h>
using namespace rjDb;
CRJDBCursor::funcFetched CRJDBCursor::m_stFuncTableStatementCursorFetched[2]= {
	& CRJDBCursor::DBCursorOnFetchedDebugOff,& CRJDBCursor::DBCursorOnFetchedDebugOn
};
CRJDBCursor::CRJDBCursor(CRJDBConnection& clDB,const CRJStringByte& clSQL):
	CRJDBStatement(clDB, clSQL) {}
CRJDBCursor::CRJDBCursor(CRJDBConnection& clDB):
	CRJDBStatement(clDB) {}
CRJDBCursor::~CRJDBCursor() {
	DBCursorClose();
}
bool CRJDBCursor::DBCursorScan() {
	DBCursorClose();
	if (DBStatementDo()) {
		pushDb();
		return true;
	}
	DBCursorClose();
	return false;
}
bool CRJDBCursor::DBCursorFind() {
	bool rc=false;
	DBCursorClose();
	if (DBStatementDo()) {
		pushDb();
		rc=DBCursorNext();
	}
	DBCursorClose();
	return rc;
}
bool CRJDBCursor::DBCursorNext() {
	try {
		if (!DBStatementIsBreaked()) {
			SQLRETURN v=::SQLFetch(m_clDBHandleStatement);
			if (v!=SQL_NO_DATA&&
				DBStatementIsResultOK(v)) {
				DBCursorFormatResults();
				(this->* CRJDBCursor::m_stFuncTableStatementCursorFetched[m_clDB.GetDBConnectionItSelfDebugFetched()])();
				return true;
			}
		}
	}
	catch(...) {}
	return false;
}
void CRJDBCursor::DBCursorOnFetchedDebugOff() {}
void CRJDBCursor::DBCursorOnFetchedDebugOn() {
	auto call=[this](CRJLogger& clLogger)->void {
		DBCursorDebugFetched(clLogger);
	};
	CRJLoggerFunctor<decltype(call)>cb(call);
	CRJLoggerThread::Check(cLoggerPartDB, cLoggerLevelTrace, cb);
}
void CRJDBCursor::DBCursorDebugFetched(CRJLogger&) {}
void CRJDBCursor::DBStatementRelease() {

}
void CRJDBCursor::pushDb()
{
	m_lSQLStatementState |= DB_STATEMENT_IN_CURSOR;
	m_clDB.DBPushCursor(*this);
}
void CRJDBCursor::popDb() {
	m_clDB.DBPopCursor(*this);
	m_lSQLStatementState&=(~DB_STATEMENT_IN_CURSOR);
}
void CRJDBCursor::DBCursorClose() {
	if (DBStatementIsCursor()) {
		popDb();
	}
	if (m_clDBHandleStatement.IsStatementReady())
		::SQLFreeStmt(m_clDBHandleStatement, SQL_CLOSE);
}
bool CRJDBCursor::DBCursorCloseAfterExec(bool rc) {
	DBCursorClose();
	return rc;
}
bool CRJDBCursor::DBCursorGet(size_t ind, CRJStringByteVar& clStr) {
	try {
		SQLLEN v;
		SQLLEN s=static_cast<SQLLEN>(clStr.GetSize());
		if (DBStatementIsResultOK(::SQLGetData(m_clDBHandleStatement,(SQLUSMALLINT) ind, SQL_C_CHAR, clStr.GetHeadForUpdate(), s,& v))) {
			if (v<=0) clStr.SetEmpty();
			else {
				clStr.SetLength(static_cast<size_t>((v>s)?
					s:v)).TrimRight(' ');
				return true;
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorGet(size_t ind, CRJStringUnicodeVar& clStr) {
	try {
		SQLLEN v;
		SQLLEN s=mdbCursorSetUnicodeParamSize(clStr, v);
		if (DBStatementIsResultOK(::SQLGetData(m_clDBHandleStatement,(SQLUSMALLINT) ind, SQL_C_WCHAR, clStr.GetHeadForUpdate(), s,& v))) {
			if (v<=0) clStr.SetEmpty();
			else {
				clStr.SetLength(static_cast<size_t>((v>s)?
					s:v)).TrimRight(' ');
				return true;
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorGet(size_t ind,void* buf, size_t l, CRJStringByteVar& clStr) {
	try {
		size_t lTempLen=clStr.GetSize();
		if (lTempLen>16) {
			SQLLEN v;
			size_t s=lTempLen-4;
			size_t i=(l>s)?	s:l;
			char* pData=(char*) clStr.GetHeadForUpdate();
			if (DBStatementIsResultOK(::SQLGetData(m_clDBHandleStatement,(SQLUSMALLINT) ind, SQL_C_CHAR, pData,static_cast<SQLLEN>(i* 2+1),& v))) {
				if ((v==SQL_NO_TOTAL)||	(v==SQL_NULL_DATA)) v=0;
				else CRJBytePacker::PackMachine((uint8_t*) buf,(uint8_t*) pData, i);
				clStr.SetLength(static_cast<size_t>(v)/ 2);
				return true;
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorGet(size_t ind, size_t& l,bool* b) {
	try {
		if (m_clDBHandleStatement.IsStatementReady()) {
			SQLLEN v;
			l=0;
			if (DBStatementIsResultOK(::SQLGetData(m_clDBHandleStatement,(SQLUSMALLINT) ind, m_clDB.HasSupport64BitsFields()?
				SQL_C_SLONG:SQL_C_SBIGINT,& l,sizeof(l),& v))) {
				if (b)* b=!(((v==SQL_NO_TOTAL)||
					(v==SQL_NULL_DATA)));
				return true;
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorGet(size_t ind, int64_t& l,bool* b) {
	try {
		if (m_clDBHandleStatement.IsStatementReady()) {
			SQLLEN v;
			l=0;
			if (DBStatementIsResultOK(::SQLGetData(m_clDBHandleStatement,(SQLUSMALLINT) ind, m_clDB.HasSupport64BitsFields()?
				SQL_C_SLONG:SQL_C_SBIGINT,& l,sizeof(l),& v))) {
				if (b)* b=!(((v==SQL_NO_TOTAL)||
					(v==SQL_NULL_DATA)));
				return true;
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorGet(size_t ind, int32_t& l,bool* b) {
	try {
		if (m_clDBHandleStatement.IsStatementReady()) {
			SQLLEN v;
			l=0;
			if (DBStatementIsResultOK(::SQLGetData(m_clDBHandleStatement,(SQLUSMALLINT) ind, SQL_C_SBIGINT,& l,sizeof(l),& v))) {
				if (b)* b=!(((v==SQL_NO_TOTAL)||
					(v==SQL_NULL_DATA)));
				return true;
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorGetDouble(size_t ind,double& d,bool* b) {
	try {
		if (m_clDBHandleStatement.IsStatementReady()) {
			SQLLEN v;
			if (DBStatementIsResultOK(::SQLGetData(m_clDBHandleStatement,(SQLUSMALLINT) ind, SQL_C_DOUBLE,& d,sizeof(d),& v))) {
				if (b)* b=!(((v==SQL_NO_TOTAL)||
					(v==SQL_NULL_DATA)));
				return true;
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorResult(size_t ind, CRJDBSQLSize& arg) {
	try {
		arg.m_lIndicator=SQL_NULL_DATA;
		arg.m_sValue=0;
		if (DBStatementIsResultOK(::SQLBindCol(m_clDBHandleStatement,(SQLUSMALLINT) ind, 
			m_clDB.HasSupport64BitsFields()?SQL_C_ULONG:SQL_C_UBIGINT,
			& arg.m_sValue,sizeof(size_t),& arg.m_lIndicator))) {
			m_lSQLStatementState|=DB_STATEMENT_BUFFERS;
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorResult(size_t ind, CRJDBSQLInt64& arg) {
	try {
		arg.m_lIndicator=SQL_NULL_DATA;
		arg.m_lValue=0;
		if (DBStatementIsResultOK(::SQLBindCol(m_clDBHandleStatement,(SQLUSMALLINT) ind, 
			m_clDB.HasSupport64BitsFields()?SQL_C_SLONG:SQL_C_SBIGINT,
			& arg.m_lValue,sizeof(int64_t),& arg.m_lIndicator))) {
			m_lSQLStatementState|=DB_STATEMENT_BUFFERS;
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorResult(size_t ind, CRJDBSQLInt32& arg) {
	try {
		arg.m_lIndicator=SQL_NULL_DATA;
		arg.m_lValue=0;
		if (DBStatementIsResultOK(::SQLBindCol(m_clDBHandleStatement,(SQLUSMALLINT) ind, SQL_C_SBIGINT,& arg.m_lValue,sizeof(int32_t),& arg.m_lIndicator))) {
			m_lSQLStatementState|=DB_STATEMENT_BUFFERS;
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorResult(size_t ind, CRJDBSQLStringByte& arg) {
	try {
		CRJStringByteVar& clValue=arg.GetString();
		arg.m_lIndicator=SQL_NULL_DATA;
		if (DBStatementIsResultOK(::SQLBindCol(m_clDBHandleStatement,(SQLUSMALLINT) ind, SQL_C_CHAR, clValue.GetHeadForUpdate(),static_cast<SQLLEN>(clValue.GetSize()),& arg.m_lIndicator))) {
			m_lSQLStatementState|=DB_STATEMENT_BUFFERS;
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorSetResultBytes(size_t ind, uint8_t* p, size_t lLen, SQLLEN& lInd) {
	try {
		lInd=SQL_NULL_DATA;
		if (DBStatementIsResultOK(::SQLBindCol(m_clDBHandleStatement,(SQLUSMALLINT) ind, SQL_C_CHAR, p,static_cast<SQLLEN>(lLen),& lInd))) {
			m_lSQLStatementState|=DB_STATEMENT_BUFFERS;
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorSetResultString(size_t ind, CRJStringByteVar& clStr, SQLLEN& lInd) {
	try {
		lInd=SQL_NULL_DATA;
		if (DBStatementIsResultOK(::SQLBindCol(m_clDBHandleStatement,(SQLUSMALLINT) ind, SQL_C_CHAR, clStr.GetHeadForUpdate(),static_cast<SQLLEN>(clStr.GetSize()),& lInd))) {
			m_lSQLStatementState|=DB_STATEMENT_BUFFERS;
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorSetResultString(size_t ind, CRJStringUnicodeVar& clStr, SQLLEN& lInd) {
	try {
		lInd=SQL_NULL_DATA;
		if (DBStatementIsResultOK(::SQLBindCol(m_clDBHandleStatement,(SQLUSMALLINT) ind, SQL_C_WCHAR, clStr.GetHeadForUpdate(), mdbCursorGetUnicodeParamSize(clStr),& lInd))) {
			m_lSQLStatementState|=DB_STATEMENT_BUFFERS;
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorSetResultInt64(size_t ind, int64_t& val, SQLLEN& lInd) {
	try {
		lInd=SQL_NULL_DATA;
		val=0;
		if (DBStatementIsResultOK(::SQLBindCol(m_clDBHandleStatement,(SQLUSMALLINT) ind, m_clDB.HasSupport64BitsFields()?
			SQL_C_SLONG:SQL_C_SBIGINT,& val,sizeof(int64_t),& lInd))) {
			m_lSQLStatementState|=DB_STATEMENT_BUFFERS;
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorSetResultInt32(size_t ind, int32_t& val, SQLLEN& lInd) {
	try {
		lInd=SQL_NULL_DATA;
		if (DBStatementIsResultOK(::SQLBindCol(m_clDBHandleStatement,(SQLUSMALLINT) ind, SQL_C_SLONG,& val,sizeof(int32_t),& lInd))) {
			m_lSQLStatementState|=DB_STATEMENT_BUFFERS;
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorSetResultSize(size_t ind, size_t& val, SQLLEN& lInd) {
	try {
		lInd=SQL_NULL_DATA;
		val=0;
		if (DBStatementIsResultOK(::SQLBindCol(m_clDBHandleStatement,(SQLUSMALLINT) ind, m_clDB.HasSupport64BitsFields()?
			SQL_C_SLONG:SQL_C_SBIGINT,& val,sizeof(int64_t),& lInd))) {
			m_lSQLStatementState|=DB_STATEMENT_BUFFERS;
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorSetResultTime(size_t ind, time_t& val, SQLLEN& lInd) {
	try {
		lInd=SQL_NULL_DATA;
		if (DBStatementIsResultOK(::SQLBindCol(m_clDBHandleStatement,(SQLUSMALLINT) ind, m_clDB.HasSupport64BitsFields()?
			SQL_C_SLONG:SQL_C_SBIGINT,& val,sizeof(int64_t),& lInd))) {
			m_lSQLStatementState|=DB_STATEMENT_BUFFERS;
			return true;
		}
	}
	catch(...) {}
	return false;
}
bool CRJDBCursor::DBCursorSetResultDouble(size_t ind,double& val, SQLLEN& lInd) {
	try {
		lInd=SQL_NULL_DATA;
		if (DBStatementIsResultOK(::SQLBindCol(m_clDBHandleStatement,(SQLUSMALLINT) ind, SQL_C_DOUBLE,& val,sizeof(double),& lInd))) {
			m_lSQLStatementState|=DB_STATEMENT_BUFFERS;
			return true;
		}
	}
	catch(...) {}
	return false;
}
void CRJDBCursor::DBCursorFormatResults() {}
size_t CRJDBCursor::DBCursorFormatResultBytes(SQLLEN& ind, uint8_t* pBuf, size_t lLen) {
	CRJStringByteVarAssigned str((uint8_t*) pBuf, lLen);
	return CRJDBCursor::DBCursorFormatResultString(ind, str);
}
size_t CRJDBCursor::DBCursorFormatResult(CRJDBSQLStringByte& arg) {
	return CRJDBCursor::DBCursorFormatResultString(arg.m_lIndicator, arg.GetString());
}
size_t CRJDBCursor::DBCursorFormatResultString(SQLLEN ind, CRJStringByteVar& clStr) {
	if (ind<=0) clStr.SetEmpty();
	else {
		if (static_cast<size_t>(ind)>=clStr.GetSize()) clStr.SetLength(clStr.GetSize()-1);
		else clStr.SetLengthSafed(static_cast<size_t>(ind));
		return clStr.TrimRight(' ').GetLength();
	}
	return 0;
}
size_t CRJDBCursor::DBCursorFormatResultString(SQLLEN ind, CRJStringUnicodeVar& clStr) {
	if (ind<=0) clStr.SetEmpty();
	else {
		ind=ind>>1;
		if (static_cast<size_t>(ind)>=clStr.GetSize()) clStr.SetLength(clStr.GetSize()-1);
		else clStr.SetLengthSafed(static_cast<size_t>(ind));
		return clStr.TrimRight(' ').GetLength();
	}
	return 0;
}
void CRJDBCursor::DBCursorFormatResultInt64(SQLLEN ind, int64_t& val, int64_t valifnull) {
	switch (ind) {
	case SQL_NO_TOTAL:
	case SQL_NULL_DATA:val=valifnull;
		break;
	}
}
void CRJDBCursor::DBCursorFormatResultSize(SQLLEN ind, size_t& val, size_t valifnull) {
	switch (ind) {
	case SQL_NO_TOTAL:
	case SQL_NULL_DATA:val=valifnull;
		break;
	}
}
void CRJDBCursor::DBCursorFormatResultDouble(SQLLEN ind,double& val,double valifnull) {
	switch (ind) {
	case SQL_NO_TOTAL:
	case SQL_NULL_DATA:val=valifnull;
		break;
	}
}
size_t CRJDBCursor::DBCursorFormatResultBinary(CRJDBSQLStringByte& dbs) {
	try {
		SQLLEN ind=dbs.GetIndicator();
		if (ind>0) {
			CRJStringByteVar& s=dbs.GetString();
			size_t l=s.GetSize();
			if (l>0) {
				size_t sLen=(size_t) ind;	// Read Count
				uint8_t* pData=s.GetHeadForUpdate();
				sLen >>= 1;	// Storage Count
				if (sLen>l) sLen=l;
				CRJBytePacker::PackMachine(pData, pData, sLen);
				s.SetLengthSafed(sLen);
				return l;
			}
		}
	}
	catch(...) {}
	dbs.Empty();
	return 0;
}
//
//CRJDBCursorAutoCommit::~CRJDBCursorAutoCommit() {
//	m_clCursor.DBCursorClose();
//	m_clCursor.m_clDB.DBResult(true);
//}
