#pragma once
#include <recurj/rjframework/libs/base/datatools/atomicnumber.h>
#include <recurj/rjframework/libs/db/db.h>
namespace rjDb {
	class CRJDBHandleStatement {
	public:
		CRJDBHandleStatement();
		virtual~CRJDBHandleStatement();
		operator SQLHDBC() const {
			return m_hSQLStatementExec;
		}
		inline bool IsStatementReady() const {
			return m_hSQLStatementExec;
		}
		SQLRETURN Create(SQLHDBC);
		SQLRETURN Release();
	protected:
		SQLHSTMT m_hSQLStatementExec;
	};
}

