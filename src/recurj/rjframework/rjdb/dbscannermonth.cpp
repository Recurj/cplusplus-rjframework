#include "stdafx.h"
#include <recurj/rjframework/libs/db/odbc/scannermonth.h>
using namespace rjDb;
CRJDBScannerMonth::CRJDBScannerMonth(CRJDBConnection& clDB,const CRJStringByte& tab,const CRJStringByte& sql):
	CRJDBScanner(clDB, sql),
c_clTable(tab) {
	m_bDBScannerDirection=true;
	m_lDBScannerMiscYear=m_sDBScannerMiscMonth=0;
}
CRJDBScannerMonth::CRJDBScannerMonth(CRJDBConnection& clDB,const CRJStringByte& tab,const CRJStringByte& sql,const CRJStringDTime& clFrom,const CRJStringDTime& clTo):
	CRJDBScanner(clDB, sql),
c_clTable(tab),
m_clDBScannerFrom(clFrom),
m_clDBScannerTo(clTo) {
	m_bDBScannerDirection=true;
	m_lDBScannerMiscYear=m_sDBScannerMiscMonth=0;
}
CRJDBScannerMonth::CRJDBScannerMonth(CRJDBConnection& clDB,const CRJStringByte& tab,const CRJStringByte& sql,const uint8_t* pFrom,const uint8_t* pTo):
	CRJDBScanner(clDB, sql),
c_clTable(tab) {
	m_clDBScannerFrom.SetLabel((char*) pFrom);
	m_clDBScannerTo.SetLabel((char*) pTo);
	m_bDBScannerDirection=true;
	m_lDBScannerMiscYear=m_sDBScannerMiscMonth=0;
}
CRJDBScannerMonth::~CRJDBScannerMonth() {}
int64_t CRJDBScannerMonth::GetMaxMonthCount() {
	return 12;
}
bool CRJDBScannerMonth::OnDBScannerSelect() {
	return CRJDBScanner::OnDBScannerSelect()&&
		m_clDBScannerSql.Cat(CRJDBStatement::s_SQLFrom).Cat(c_clTable).Cat(m_clDBMonth).OK();
}
void CRJDBScannerMonth::SetDBScannerDirection(bool b) {
	m_bDBScannerDirection=b;
}
bool CRJDBScannerMonth::DBScannerMonthStart() {
	try {
		return (m_clDBScannerFrom.ParseDateTime(m_clFrom))&&
			(m_clDBScannerTo.ParseDateTime(m_clTo))&&
			((m_clTo.m_lYear>m_clFrom.m_lYear)||
			((m_clTo.m_lYear==m_clFrom.m_lYear)&&
			(m_clTo.m_sMonth>=m_clFrom.m_sMonth)));
	}
	catch(...) {}
	m_lDBScannerResult=eUBScannerResults::RC_DBSCANNER_BADPARAM;
	return false;
}
bool CRJDBScannerMonth::DBScannerScan() {
	bool rc=false;
	try {
		if (DBScannerMonthStart()) {
			int64_t v=GetMaxMonthCount();
			if (m_bDBScannerDirection) {
				m_lDBScannerMiscYear=m_clFrom.m_lYear;
				m_sDBScannerMiscMonth=m_clFrom.m_sMonth;
			}
			else {
				m_lDBScannerMiscYear=m_clTo.m_lYear;
				m_sDBScannerMiscMonth=m_clTo.m_sMonth;
			}
			while (v>0) {
				if ((m_bDBScannerBreaked)||
					(!OnDBScannerMonth()))break;
				CRJDBScanner::DBScannerScan();
				if ((m_bDBScannerBreaked)||
					(!OnDBScannerGetNextMonth()))break;
				v--;
			}
			rc=true;
		}
	}
	catch(...) {}
	OnDBScannerReset();
	return rc;
}
bool CRJDBScannerMonth::OnDBScannerMonth() {
	m_clDBMonth.SetInt64PaddingLeft(m_lDBScannerMiscYear, 4).CatInt64Fmt(m_sDBScannerMiscMonth, 2);
	return true;
}
bool CRJDBScannerMonth::OnDBScannerGetNextMonth() {
	if (m_bDBScannerDirection) {
		SRJDateTime::NextMonth(m_lDBScannerMiscYear, m_sDBScannerMiscMonth);
		if ((m_lDBScannerMiscYear>m_clTo.m_lYear)||
			((m_lDBScannerMiscYear==m_clTo.m_lYear)&&
			(m_sDBScannerMiscMonth>m_clTo.m_sMonth))) return false;
	}
	else {
		SRJDateTime::PrevMonth(m_lDBScannerMiscYear, m_sDBScannerMiscMonth);
		if ((m_lDBScannerMiscYear<m_clFrom.m_lYear)||
			((m_lDBScannerMiscYear==m_clFrom.m_lYear)&&
			(m_sDBScannerMiscMonth<m_clFrom.m_sMonth))) return false;
	}
	return true;
}
bool CRJDBScannerMonth::OnDBScannerDTime(const CRJStringByte& clField) {
	bool rc=false;
	try {
		CRJStringByteDefault str;
		if ((m_sDBScannerMiscMonth==m_clFrom.m_sMonth)&&
			(m_lDBScannerMiscYear==m_clFrom.m_lYear)) {
			str.SetLabel("(", 1).Cat(clField).CatLabel(">='", 3).Cat(m_clDBScannerFrom).CatLabel("')", 2);
			if (!DBScannerAddWhere(str))throw-1;
		}
		if ((m_sDBScannerMiscMonth==m_clTo.m_sMonth)&&
			(m_lDBScannerMiscYear==m_clTo.m_lYear)) {
			str.SetLabel("(", 1).Cat(clField).CatLabel("<'", 2).Cat(m_clDBScannerTo).CatLabel("')", 2);
			if (!DBScannerAddWhere(str))throw-1;
		}
		rc=true;
	}
	catch(...) {}
	return rc;
}

