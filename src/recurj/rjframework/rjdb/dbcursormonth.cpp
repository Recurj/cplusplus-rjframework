#include "stdafx.h"
#include <stdlib.h>
#include <recurj/rjframework/libs/base/strings/dtime.h>
#include <recurj/rjframework/libs/db/odbc/cursormonth.h>
using namespace rjDb;
CRJDBCursorMonth::CRJDBCursorMonth(CRJDBConnection& clDB,const CRJStringByte& tab,const CRJStringByte& select,const CRJStringByte& cwhere):
	CRJDBCursor(clDB),
c_clSelect(select),
c_clWhere(cwhere) {
	m_clTable.Set(tab);
}
CRJDBCursorMonth::~CRJDBCursorMonth() {}
void CRJDBCursorMonth::SQLMonthCheckDTime(const uint8_t* pDTime) {
	if (m_clMarker.CheckDTime(pDTime)) {
		DBStatementRelease();
		DBStatementSetSQL(BuildSql());
	}
}
void CRJDBCursorMonth::SQLMonthCheckDTime(const CRJStringDTime& clDTime) {
	if (m_clMarker.CheckDTime(clDTime.GetHeadForRead())) {
		DBStatementRelease();
		DBStatementSetSQL(BuildSql());
	}
}
void CRJDBCursorMonth::SQLMonthCheckLabel(const uint8_t* pLabel) {
	if (m_clMarker.CheckLabel(pLabel)) {
		DBStatementRelease();
		DBStatementSetSQL(BuildSql());
	}
}
void CRJDBCursorMonth::SQLMonthCheckLabel(const CRJStringByte& clLabel) {
	if (m_clMarker.CheckLabel(clLabel.GetHeadForRead())) {
		DBStatementRelease();
		DBStatementSetSQL(BuildSql());
	}
}
void CRJDBCursorMonth::SQLMonthCurrent() {
	CRJDTimeTools::SetYYYYMM(CRJDTimeTools::GetDTime(), m_clMarker.GetHeadForUpdate(), m_clMarker.GetSize());
	DBStatementRelease();
	DBStatementSetSQL(BuildSql());
}
CRJStringByteVar& CRJDBCursorMonth::BuildSql() {
	m_clSQLMonth.Set(CRJDBStatement::s_SQLSelect).Cat(c_clSelect).Cat(CRJDBStatement::s_SQLFrom).Cat(m_clTable).Cat(m_clMarker);
	if (c_clWhere.IsNonEmpty()) m_clSQLMonth.Cat(CRJDBStatement::s_SQLWhere).Cat(c_clWhere);
	return m_clSQLMonth;
}

