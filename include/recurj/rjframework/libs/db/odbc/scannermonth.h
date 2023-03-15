#pragma once
#include <recurj/rjframework/libs/base/strings/dtime.h>
#include <recurj/rjframework/libs/db/odbc/scanner.h>
namespace rjDb {
	class RJDB_API CRJDBScannerMonth:public CRJDBScanner {
	public:
		CRJDBScannerMonth(CRJDBConnection&,const CRJStringByte&,const CRJStringByte&);
		CRJDBScannerMonth(CRJDBConnection&,const CRJStringByte&,const CRJStringByte&,const CRJStringDTime&,const CRJStringDTime&);
		CRJDBScannerMonth(CRJDBConnection&,const CRJStringByte&,const CRJStringByte&,const uint8_t*,const uint8_t*);
		virtual~CRJDBScannerMonth();
		bool DBScannerScan() override;
		void SetDBScannerDirection(bool);
	protected:
		bool OnDBScannerSelect() override;
		virtual bool OnDBScannerMonth();
		virtual bool OnDBScannerGetNextMonth();
		virtual bool OnDBScannerDTime(const CRJStringByte&);
		virtual int64_t GetMaxMonthCount();
		bool DBScannerMonthStart();
	protected:
		const CRJStringByte& c_clTable;
		CRJStringDTime m_clDBScannerFrom, m_clDBScannerTo, m_clDBMonth;
		bool m_bDBScannerDirection;
		int64_t m_lDBScannerMiscYear;
		size_t m_sDBScannerMiscMonth;
		SRJDateTime m_clFrom, m_clTo;
	};
}

