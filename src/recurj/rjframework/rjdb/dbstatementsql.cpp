#include "stdafx.h"
#include <recurj/rjframework/libs/db/odbc/statementsql.h>
using namespace rjDb;
CRJDBSQL::CRJDBSQL(CRJDBConnection& clDB):
	CRJDBStatement(clDB, CRJDBStatement::m_clSQLNotDefined) {}
CRJDBSQL::~CRJDBSQL() {}
bool CRJDBSQL::SQLStatementExecute() {
	DBStatementRelease();
	DBStatementSetSQL(GetSQLBuffer());
	return DBStatementDo();
}
bool CRJDBSQL::SQLStatementExecute(const CRJStringByte& clSql) {
	GetSQLBuffer().Set(clSql);
	return SQLStatementExecute();
}
bool CRJDBSQL::SQLStatementExecute(const CRJStringByte& clSql,const CRJStringByte& clTail) {
	GetSQLBuffer().Set(clSql).Cat(clTail);
	return SQLStatementExecute();
}
bool CRJDBSQL::SQLStatementExecute(const CRJStringByte& clSql,const CRJStringByte& clTab,const CRJStringByte& clTail) {
	GetSQLBuffer().Set(clSql).Cat(clTab).Cat(clTail);
	return SQLStatementExecute();
}

