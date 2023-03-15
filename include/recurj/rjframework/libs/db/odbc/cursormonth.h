#pragma once
#include <recurj/rjframework/libs/db/odbc/cursor.h>
#include <recurj/rjframework/libs/db/odbc/dbmonthmarker.h>
namespace rjDb {
	class RJDB_API CRJDBCursorMonth:public CRJDBCursor {
	public:
		CRJDBCursorMonth(CRJDBConnection&,const CRJStringByte&,const CRJStringByte&,const CRJStringByte&);
		virtual~CRJDBCursorMonth();
	protected:
		void SQLMonthCheckDTime(const uint8_t*);
		void SQLMonthCheckDTime(const CRJStringDTime&);
		void SQLMonthCheckLabel(const uint8_t*);
		void SQLMonthCheckLabel(const CRJStringByte&);
		void SQLMonthCurrent();
		CRJStringByteVar& BuildSql();
	protected:
		const CRJStringByte& c_clSelect;
		const CRJStringByte& c_clWhere;
		CRJDBMonthMarker m_clMarker;
		CRJStringByteExpanded m_clSQLMonth;
		CRJStringByteShort m_clTable;
	};
}

