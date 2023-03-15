#include "stdafx.h"
#include <recurj/rjframework/libs/db/odbc/statement.h>
using namespace rjDb;
CRJDBHandleStatement::CRJDBHandleStatement() {
	m_hSQLStatementExec=nullptr;
}
CRJDBHandleStatement::~CRJDBHandleStatement() {
	Release();
}
SQLRETURN CRJDBHandleStatement::Create(SQLHDBC h) {
	try {
		if (h) return ::SQLAllocHandle(SQL_HANDLE_STMT, h,& m_hSQLStatementExec);
	}
	catch(...) {}
	return SQL_ERROR;
}
SQLRETURN CRJDBHandleStatement::Release() {
	SQLRETURN rc=SQL_ERROR;
	try {
		if (m_hSQLStatementExec) {
			rc=::SQLFreeHandle(SQL_HANDLE_STMT, m_hSQLStatementExec);
			switch (rc) {
			case SQL_SUCCESS:
			case SQL_SUCCESS_WITH_INFO:m_hSQLStatementExec=nullptr;
				break;
			}
		}
	}
	catch(...) {}
	return rc;
}

