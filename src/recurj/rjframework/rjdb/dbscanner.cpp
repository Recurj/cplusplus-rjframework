#include "stdafx.h"
#include <recurj/rjframework/libs/db/odbc/scanner.h>
#include <recurj/rjframework/libs/db/odbc/connection.h>
#include <recurj/rjframework/libs/base/strings/dictionary.h>
using namespace rjBase;
using namespace rjDb;
CRJDBScanner::CRJDBScanner(CRJDBConnection& clDB,const CRJStringByte& sql):
	CRJDBCursor(clDB, CRJDBStatement::m_clSQLNotDefined),
c_clSelect(sql) {
	m_lDBScannerResult=eUBScannerResults::RC_DBSCANNER_OK;
	m_bDBScannerBreaked=false;
}
CRJDBScanner::~CRJDBScanner() {}
bool CRJDBScanner::DBScannerScan() {
	bool rc=false;
	try {
		if ((OnDBScannerSelect())&&
			(OnDBScannerWhere())&&
			(OnDBScannerGroup())&&
			(OnDBScannerOrder())) {
			if (OnDBScannerStart()) {
				m_sCount=0;
				while (OnDBScannerScan()) {
					OnDBScannerScanned();
					if (m_bDBScannerBreaked)break;
				};
			}
			rc=!m_bDBScannerBreaked;
		}
	}
	catch(...) {}
	OnDBScannerReset();
	return rc;
}
bool CRJDBScanner::OnDBScannerReset() {
	DBScannerQueryProcess(false);
	DBStatementClose();
	return true;
}
bool CRJDBScanner::OnDBScannerSelect() {
	OnDBScannerReset();
	return m_clDBScannerSql.SetEmpty().Set(CRJDBStatement::s_SQLSelect).Cat(c_clSelect).OK();
}
bool CRJDBScanner::OnDBScannerWhere() {
	return m_clDBScannerWhere.SetEmpty().OK();
}
bool CRJDBScanner::OnDBScannerGroup() {
	return m_clDBScannerGroup.SetEmpty().OK();
}
bool CRJDBScanner::OnDBScannerOrder() {
	return m_clDBScannerOrder.SetEmpty().OK();
}
bool CRJDBScanner::OnDBScannerStart() {
	try {
		if (m_clDBScannerWhere.OK()&&
			m_clDBScannerGroup.OK()&&
			m_clDBScannerOrder.OK()) {
			if (m_clDBScannerWhere.GetLength()>0) m_clDBScannerSql.Cat(CRJDBStatement::s_SQLWhere).Cat(m_clDBScannerWhere);
			if (m_clDBScannerGroup.GetLength()>0) m_clDBScannerSql.Cat(CRJDBStatement::s_SQLGroupBy).Cat(m_clDBScannerGroup);
			if (m_clDBScannerOrder.GetLength()>0) m_clDBScannerSql.Cat(CRJDBStatement::s_SQLOrderBy).Cat(m_clDBScannerOrder);
			if (m_clDBScannerSql.OK()) {
				if (OnDBScannerSQLReady()) {
					if (DBCursorScan()) {
						DBScannerQueryProcess(true);
						return true;
					}
					else m_lDBScannerResult=eUBScannerResults::RC_DBSCANNER_DBERROR;
				}
				else m_lDBScannerResult=eUBScannerResults::RC_DBSCANNER_DBERROR;
			}
			else m_lDBScannerResult=eUBScannerResults::RC_DBSCANNER_NO_MEMORY;
		}
		else m_lDBScannerResult=eUBScannerResults::RC_DBSCANNER_NO_MEMORY;
	}
	catch(...) {}
	return false;
}
bool CRJDBScanner::OnDBScannerSQLReady() {
	DBStatementSetSQL(m_clDBScannerSql);
	return true;
}
bool CRJDBScanner::OnDBScannerScan() {
	if (DBStatementGetDB().DBNeedBreak(0)) return DBScannerSetBreaked();
	if (DBCursorNext()) {
		m_sCount++;
		return true;
	}
	return false;
}
bool CRJDBScanner::DBScannerAddWhereDTime(const CRJStringByte& clField,const CRJStringByte& from,const CRJStringByte& to) {
	CRJStringByteDefault str;
	str.SetLabel("(", 1).Cat(clField).CatLabel(">='", 3).Cat(from).CatLabel("')", 2);
	if (!DBScannerAddWhere(str)) return false;
	str.SetLabel("(", 1).Cat(clField).CatLabel("<'", 2).Cat(to).CatLabel("')", 2);
	if (!DBScannerAddWhere(str)) return false;
	return true;
}
bool CRJDBScanner::DBScannerAddWhere(uint8_t* val, size_t sVol,bool bAnd) {
	if ((!m_clDBScannerWhere.GetLength())||
		(m_clDBScannerWhere.OK()&&
		m_clDBScannerWhere.Cat((bAnd)?
		CRJDBStatement::s_SQLAnd:CRJDBStatement::s_SQLOr).OK())) {
		if (m_clDBScannerWhere.CatDataAsLabel(val, sVol).OK()) return true;
	}
	m_lDBScannerResult=eUBScannerResults::RC_DBSCANNER_NO_MEMORY;
	return false;
}
bool CRJDBScanner::DBScannerAddWhere(const CRJStringByte& clAdd,bool bAnd) {
	if ((!m_clDBScannerWhere.GetLength())||
		(m_clDBScannerWhere.OK()&&
		m_clDBScannerWhere.Cat((bAnd)?
		CRJDBStatement::s_SQLAnd:CRJDBStatement::s_SQLOr).OK())) {
		if (m_clDBScannerWhere.Cat(clAdd).OK()) return true;
	}
	m_lDBScannerResult=eUBScannerResults::RC_DBSCANNER_NO_MEMORY;
	return false;
}
bool CRJDBScanner::DBScannerAddWhere(const char* name, int64_t val,char op,bool bAnd) {
	return DBScannerAddWhere(m_clTemp.SetSingle(' ').CatLabel(name).CatSingle(op).CatInt64(val).CatSingle(' '), bAnd);
}
bool CRJDBScanner::DBScannerAddWhereBrackets(const char* name, int64_t val,char op,bool bAnd) {
	return DBScannerAddWhere(m_clTemp.SetLabel(" (", 2).CatLabel(name).CatSingle(op).CatInt64(val).CatLabel(") ", 2), bAnd);
}
bool CRJDBScanner::DBScannerAddWhereBracketsExact(bool bExact,const char* name, uint8_t* val, size_t sVol,char op,bool bAnd) {
	return (bExact)?
		DBScannerAddWhereBrackets(name, val, sVol, op, bAnd):
		DBScannerAddWhereBracketsLike(name, val, sVol, bAnd);
}
bool CRJDBScanner::DBScannerAddWhereBrackets(const char* name, uint8_t* val, size_t sVol,char op,bool bAnd) {
	return DBScannerAddWhere(m_clTemp.SetLabel(" (", 2).CatLabel(name).CatSingle(op).CatSingle('\'').CatDataAsLabel(val, sVol).CatLabel("') ", 3), bAnd);
}
bool CRJDBScanner::DBScannerAddWhereBrackets(const char* name, uint8_t* val, size_t sVol,const char* cmp,bool bAnd) {
	return DBScannerAddWhere(m_clTemp.SetLabel(" (", 2).CatLabel(name).CatLabel(cmp).CatSingle('\'').CatDataAsLabel(val, sVol).CatLabel("') ", 3), bAnd);
}
bool CRJDBScanner::DBScannerAddWhereBracketsLike(const char* name, uint8_t* val, size_t sVol,bool bAnd) {
	return DBScannerAddWhere(m_clTemp.SetLabel(" (", 2).CatLabel(name).CatLabel(" like '%", 8).CatDataAsLabel(val, sVol).CatLabel("%') ", 4), bAnd);
}
bool CRJDBScanner::DBScannerAddWhereBracketsLikeRight(const char* name, uint8_t* val, size_t sVol,bool bAnd) {
	return DBScannerAddWhere(m_clTemp.SetLabel(" (", 2).CatLabel(name).CatLabel(" like '", 7).CatDataAsLabel(val, sVol).CatLabel("%') ", 4), bAnd);
}
bool CRJDBScanner::DBScannerAddWhereBracketsLikeRight(const char* name,const CRJStringByte& s,bool bAnd) {
	return DBScannerAddWhere(m_clTemp.SetLabel(" (", 2).CatLabel(name).CatLabel(" like '", 7).Cat(s).CatLabel("%') ", 4), bAnd);
}
bool CRJDBScanner::DBScannerAddWhereBracketsIn(const char* name, uint8_t* val, size_t sVol,bool bAnd) {
	return DBScannerAddWhere(m_clTemp.SetLabel(" (", 2).CatLabel(name).CatLabel(" in (", 5).CatDataAsLabel(val, sVol).CatLabel(")) ", 3), bAnd);
}
bool CRJDBScanner::DBScannerAddWhereBracketsIn(const char* name,const CRJStringByte& s,bool bAnd) {
	return DBScannerAddWhere(m_clTemp.SetLabel(" (", 2).CatLabel(name).CatLabel(" in (", 5).Cat(s).CatLabel(")) ", 3), bAnd);
}
void CRJDBScanner::OnDBScannerScanned() {
	m_bDBScannerBreaked=DBStatementIsBreaked()>0;
}
bool CRJDBScanner::OnDBScannerCheckAfterRead(int64_t lCount, int64_t lBorder, int64_t lCountDelay, int64_t lDelay) {
	try {
		if (lCount>0) {
			if (lCount==lBorder) return DBScannerSetBreaked();
			else if ((lDelay>0)&&
				((lCountDelay<2)||
				(!(lCount% lCountDelay)))) {
				if (DBStatementGetDB().DBNeedBreak(lDelay)) return DBScannerSetBreaked();
			}
		}
	}
	catch(...) {}
	return true;
}
void CRJDBScanner::DBScannerQueryProcess(bool bProcess) {
	DBStatementGetDB().DBQueryProcess(bProcess);
}
CRJStringByteVar& CRJDBScannerWhere::WhereStart(const CRJStringByte& f) {
	return Set(CRJDBStatement::s_SQLWhereStart).Cat(f);
}
CRJStringByteVar& CRJDBScannerWhere::WhereEnd(CRJStringByteVar& s) {
	return s.Cat(CRJDBStatement::s_SQLWhereIntEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereNumber(const CRJStringByte& f, int64_t v,const CRJStringByte& func) {
	return Set(CRJDBStatement::s_SQLWhereStart).Cat(f).Cat(func).CatInt64(v).Cat(CRJDBStatement::s_SQLWhereIntEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereNumber(const char* f, int64_t v,const CRJStringByte& func) {
	return Set(CRJDBStatement::s_SQLWhereStart).CatLabel(f).Cat(func).CatInt64(v).Cat(CRJDBStatement::s_SQLWhereIntEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereIn(const CRJStringByte& f, uint8_t* val, size_t sVol) {
	return Set(CRJDBStatement::s_SQLWhereStart).Cat(f).Cat(CRJDBStatement::s_SQLWhereInEq).CatDataAsLabel(val, sVol).Cat(CRJDBStatement::s_SQLWhereInEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereIn(const char* f, uint8_t* val, size_t sVol) {
	return Set(CRJDBStatement::s_SQLWhereStart).CatLabel(f).Cat(CRJDBStatement::s_SQLWhereInEq).CatDataAsLabel(val, sVol).Cat(CRJDBStatement::s_SQLWhereInEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereIn(const CRJStringByte& f,const CRJStringByte& v) {
	return Set(CRJDBStatement::s_SQLWhereStart).Cat(f).Cat(CRJDBStatement::s_SQLWhereInEq).Cat(v).Cat(CRJDBStatement::s_SQLWhereInEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereIn(const char* f,const CRJStringByte& v) {
	return Set(CRJDBStatement::s_SQLWhereStart).CatLabel(f).Cat(CRJDBStatement::s_SQLWhereInEq).Cat(v).Cat(CRJDBStatement::s_SQLWhereInEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereString(const CRJStringByte& f, uint8_t* val, size_t sVol,const CRJStringByte& func) {
	return Set(CRJDBStatement::s_SQLWhereStart).Cat(f).Cat(func).CatSingle('\'').CatDataAsLabel(val, sVol).Cat(CRJDBStatement::s_SQLWhereStrEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereString(const char* f, uint8_t* val, size_t sVol,const CRJStringByte& func) {
	return Set(CRJDBStatement::s_SQLWhereStart).CatLabel(f).Cat(func).CatSingle('\'').CatDataAsLabel(val, sVol).Cat(CRJDBStatement::s_SQLWhereStrEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereString(const CRJStringByte& f,const CRJStringByte& v,const CRJStringByte& func) {
	return Set(CRJDBStatement::s_SQLWhereStart).Cat(f).Cat(func).CatSingle('\'').Cat(v).Cat(CRJDBStatement::s_SQLWhereStrEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereString(const char* f,const CRJStringByte& v,const CRJStringByte& func) {
	return Set(CRJDBStatement::s_SQLWhereStart).CatLabel(f).Cat(func).CatSingle('\'').Cat(v).Cat(CRJDBStatement::s_SQLWhereStrEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereLike(const CRJStringByte& f, uint8_t* val, size_t sVol) {
	return Set(CRJDBStatement::s_SQLWhereStart).Cat(f).Cat(CRJDBStatement::s_SQLWhereLikeEq).CatDataAsLabel(val, sVol).Cat(CRJDBStatement::s_SQLWhereLikeEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereLike(const char* f, uint8_t* val, size_t sVol) {
	return Set(CRJDBStatement::s_SQLWhereStart).CatLabel(f).Cat(CRJDBStatement::s_SQLWhereLikeEq).CatDataAsLabel(val, sVol).Cat(CRJDBStatement::s_SQLWhereLikeEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereLike(const CRJStringByte& f,const CRJStringByte& v) {
	return Set(CRJDBStatement::s_SQLWhereStart).Cat(f).Cat(CRJDBStatement::s_SQLWhereLikeEq).Cat(v).Cat(CRJDBStatement::s_SQLWhereLikeEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereLike(const char* f,const CRJStringByte& v) {
	return Set(CRJDBStatement::s_SQLWhereStart).CatLabel(f).Cat(CRJDBStatement::s_SQLWhereLikeEq).Cat(v).Cat(CRJDBStatement::s_SQLWhereLikeEnd);
}
CRJStringByteVar& CRJDBScannerWhere::WhereStringWithExact(bool bExact,const char* pField, uint8_t* pValue, size_t lSize) {
	return (bExact)?
		WhereString(pField, pValue, lSize):
		WhereLike(pField, pValue, lSize);
}
CRJStringByteVar& CRJDBScannerWhere::WhereGroup(const char* pField, int64_t id, int64_t lTag) {
	return WhereEnd(WhereStart(pField).CatLabel(" in (select robj2 from obj2link where robj1=").CatInt64(id).CatLabel(" and ltag2=").CatInt64(lTag));
}
CRJStringByteVar& CRJDBScannerWhere::WhereCompare(size_t lFlag,const char* pField, int64_t val) {
	switch (lFlag) {
	case CRJCompareNE: return WhereNumber(pField, val, CRJStringDictionary::s_functionNE1);
	case CRJCompareGT: return WhereNumber(pField, val, CRJStringDictionary::s_functionGT);
	case CRJCompareLT: return WhereNumber(pField, val, CRJStringDictionary::s_functionLT);
	case CRJCompareGE: return WhereNumber(pField, val, CRJStringDictionary::s_functionGE);
	case CRJCompareLE: return WhereNumber(pField, val, CRJStringDictionary::s_functionLE);
	}
	return WhereNumber(pField, val);
}

