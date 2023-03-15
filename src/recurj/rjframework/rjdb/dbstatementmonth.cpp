#include "stdafx.h"
#include <recurj/rjframework/libs/db/odbc/statementmonth.h>
using namespace rjDb;
CRJDBStatementMonth::CRJDBStatementMonth(CRJDBConnection& clDB,const CRJStringByte& statement,const CRJStringByte& tab,const CRJStringByte& sql):
	CRJDBStatement(clDB),
m_clSql(sql) {
	m_clPrefix.Set(statement).Cat(tab);
}
CRJDBStatementMonth::~CRJDBStatementMonth() {}
void CRJDBStatementMonth::SQLMonthCheckDTime(const uint8_t* pDTime) {
	if (m_clMarker.CheckDTime(pDTime)) {
		DBStatementRelease();
		DBStatementSetSQL(m_clSQLMonth.Set(m_clPrefix).Cat(m_clMarker).Cat(m_clSql));
	}
}
void CRJDBStatementMonth::SQLMonthCheckDTime(const CRJStringDTime& clDTime) {
	if (m_clMarker.CheckDTime(clDTime.GetHeadForRead())) {
		DBStatementRelease();
		DBStatementSetSQL(m_clSQLMonth.Set(m_clPrefix).Cat(m_clMarker).Cat(m_clSql));
	}
}
void CRJDBStatementMonth::SQLMonthCheckLabel(const uint8_t* pLabel) {
	if (m_clMarker.CheckLabel(pLabel)) {
		DBStatementRelease();
		DBStatementSetSQL(m_clSQLMonth.Set(m_clPrefix).Cat(m_clMarker).Cat(m_clSql));
	}
}
void CRJDBStatementMonth::SQLMonthCheckLabel(const CRJStringByte& label) {
	if (m_clMarker.CheckLabel(label.GetHeadForRead())) {
		DBStatementRelease();
		DBStatementSetSQL(m_clSQLMonth.Set(m_clPrefix).Cat(m_clMarker).CatSingle(' ').Cat(m_clSql));
	}
}
CRJDBStatementMonthUpdate::CRJDBStatementMonthUpdate(CRJDBConnection& clDB,const CRJStringByte& tab,const CRJStringByte& sql):
	CRJDBStatementMonth(clDB, CRJDBStatement::s_SQLUpdate, tab, sql) {}
CRJDBStatementMonthInsert::CRJDBStatementMonthInsert(CRJDBConnection& clDB,const CRJStringByte& tab,const CRJStringByte& sql):
	CRJDBStatementMonth(clDB, CRJDBStatement::s_SQLInsert, tab, sql) {}
CRJDBStatementMonthDelete::CRJDBStatementMonthDelete(CRJDBConnection& clDB,const CRJStringByte& tab,const CRJStringByte& sql):
	CRJDBStatementMonth(clDB, CRJDBStatement::s_SQLDelete, tab, sql) {}

