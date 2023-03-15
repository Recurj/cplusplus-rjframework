#pragma once
#include <recurj/rjframework/libs/db/odbc/statement.h>
#include <recurj/rjframework/libs/db/odbc/dbmonthmarker.h>
namespace rjDb {
	class RJDB_API CRJDBStatementMonth:public CRJDBStatement {
	public:
		virtual~CRJDBStatementMonth();
	protected:
		CRJDBStatementMonth(CRJDBConnection&,const CRJStringByte&,const CRJStringByte&,const CRJStringByte&);
		void SQLMonthCheckDTime(const uint8_t*);
		void SQLMonthCheckDTime(const CRJStringDTime&);
		void SQLMonthCheckLabel(const uint8_t*);
		void SQLMonthCheckLabel(const CRJStringByte&);
	protected:
		const CRJStringByte& m_clSql;
		CRJDBMonthMarker m_clMarker;
		CRJStringByteShort m_clPrefix;
		CRJStringByteExpanded m_clSQLMonth;
	};
	class RJDB_API CRJDBStatementMonthUpdate:public CRJDBStatementMonth {
	public:
		CRJDBStatementMonthUpdate(CRJDBConnection&,const CRJStringByte&,const CRJStringByte&);
	};
	class RJDB_API CRJDBStatementMonthInsert:public CRJDBStatementMonth {
	public:
		CRJDBStatementMonthInsert(CRJDBConnection&,const CRJStringByte&,const CRJStringByte&);
	};
	class RJDB_API CRJDBStatementMonthDelete:public CRJDBStatementMonth {
	public:
		CRJDBStatementMonthDelete(CRJDBConnection&,const CRJStringByte&,const CRJStringByte&);
	};
}

