#pragma once
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/db/db.h>
#include <recurj/rjframework/libs/db/odbc/statement.h>
namespace rjDb {
	class RJDB_API CRJDBSQL:public CRJDBStatement {
	public:
		virtual~CRJDBSQL();
		virtual CRJStringByteVar& GetSQLBuffer()=0;
		bool SQLStatementExecute();
		bool SQLStatementExecute(const CRJStringByte&);
		bool SQLStatementExecute(const CRJStringByte&,const CRJStringByte&);
		bool SQLStatementExecute(const CRJStringByte&,const CRJStringByte&,const CRJStringByte&);
	protected:
		CRJDBSQL(CRJDBConnection&);
	};
	class RJDB_API CDBStatementSQLExist:public CRJDBSQL {
	public:
		CDBStatementSQLExist(CRJStringByteVar& cl, CRJDBConnection& clDB):
			CRJDBSQL(clDB),
		m_clBuffer(cl) {}
		virtual~CDBStatementSQLExist() {}
		virtual CRJStringByteVar& GetSQLBuffer() {
			return m_clBuffer;
		}
	protected:
		CRJStringByteVar& m_clBuffer;
	};
}
template<
class T>
class CRJDBSQLTemplate:public rjDb::CRJDBSQL {
public:
	CRJDBSQLTemplate(rjDb::CRJDBConnection& clDB):
		rjDb::CRJDBSQL(clDB) {}
	CRJDBSQLTemplate(rjDb::CRJDBConnection& clDB,const char* p):
		rjDb::CRJDBSQL(clDB) {
		m_clBuffer.SetLabel(p);
	}
	CRJDBSQLTemplate(rjDb::CRJDBConnection& clDB,const CRJStringByte& cl):
		rjDb::CRJDBSQL(clDB) {
		m_clBuffer.Set(cl);
	}
	virtual~CRJDBSQLTemplate() {}
	virtual CRJStringByteVar& GetSQLBuffer() {
		return m_clBuffer;
	}
	void SQLUsePermanent() {
		DBStatementSetSQL(m_clBuffer);
	}
protected:
	T m_clBuffer;
};
namespace rjDb {
	RJDB_TEMPLATE template 
	class CRJDBSQLTemplate<CRJStringByteDefault>;
	RJDB_TEMPLATE template 
	class CRJDBSQLTemplate<CRJStringByteLong>;
	RJDB_TEMPLATE template 
	class CRJDBSQLTemplate<CRJStringByteExpanded>;
	using CRJDBSQLDefault=CRJDBSQLTemplate<CRJStringByteDefault>;
	using CRJDBSQLLong=CRJDBSQLTemplate<CRJStringByteLong>;
	using CRJDBSQLExpanded=CRJDBSQLTemplate<CRJStringByteExpanded>;
}

