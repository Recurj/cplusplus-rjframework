#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/db/odbc/cursor.h>
#include <recurj/rjframework/libs/base/strings/dictionary.h>
using namespace rjBase;
namespace rjDb {
	class RJDB_API CRJDBScannerWhere:public CRJStringByteExpanded {
	public:
		using CRJStringByteExpanded::CRJStringByteExpanded;
		CRJStringByteVar& WhereStart(const CRJStringByte&);
		CRJStringByteVar& WhereEnd(CRJStringByteVar&);
		CRJStringByteVar& WhereNumber(const CRJStringByte&, int64_t,const CRJStringByte& func=CRJStringDictionary::s_functionEQ);
		CRJStringByteVar& WhereNumber(const char*, int64_t,const CRJStringByte& func=CRJStringDictionary::s_functionEQ);
		CRJStringByteVar& WhereIn(const CRJStringByte&, uint8_t*, size_t);
		CRJStringByteVar& WhereIn(const char*, uint8_t*, size_t);
		CRJStringByteVar& WhereIn(const CRJStringByte&,const CRJStringByte&);
		CRJStringByteVar& WhereIn(const char*,const CRJStringByte&);
		CRJStringByteVar& WhereString(const CRJStringByte&, uint8_t*, size_t,const CRJStringByte& func=CRJStringDictionary::s_functionEQ);
		CRJStringByteVar& WhereString(const char*, uint8_t*, size_t,const CRJStringByte& func=CRJStringDictionary::s_functionEQ);
		CRJStringByteVar& WhereString(const CRJStringByte&,const CRJStringByte&,const CRJStringByte& func=CRJStringDictionary::s_functionEQ);
		CRJStringByteVar& WhereString(const char*,const CRJStringByte&,const CRJStringByte& func=CRJStringDictionary::s_functionEQ);
		CRJStringByteVar& WhereStringWithExact(bool,const char*, uint8_t*, size_t);
		CRJStringByteVar& WhereLike(const CRJStringByte&, uint8_t*, size_t);
		CRJStringByteVar& WhereLike(const char*, uint8_t*, size_t);
		CRJStringByteVar& WhereLike(const CRJStringByte&,const CRJStringByte&);
		CRJStringByteVar& WhereLike(const char*,const CRJStringByte&);
		CRJStringByteVar& WhereGroup(const char*, int64_t id, int64_t);
		CRJStringByteVar& WhereCompare(size_t,const char*, int64_t);
	};
	class RJDB_API CRJDBScanner:public CRJDBCursor {
	public:
		enum 
		class eUBScannerResults {
			RC_DBSCANNER_BREAKED=-4, RC_DBSCANNER_RUNNING=-3, RC_DBSCANNER_STOPPED=-2, RC_DBSCANNER_UNKNOWN=-1,
			RC_DBSCANNER_OK=0, RC_DBSCANNER_NO_DLL=1, RC_DBSCANNER_NO_MEMORY=2, RC_DBSCANNER_BAD_DLL=3,
			RC_DBSCANNER_NORUN_DLL=4, RC_DBSCANNER_NODB=5, RC_DBSCANNER_BADPARAM=6, RC_DBSCANNER_DBERROR=7,
			RC_DBSCANNER_NOREPORTTRUSTEE=8, RC_DBSCANNER_NOPARAMTRUSTEE=9, RC_DBSCANNER_NOREPORT=10,
			RC_DBSCANNER_NO_OUTPUT=11
		};
		CRJDBScanner(CRJDBConnection&,const CRJStringByte&);
		virtual~CRJDBScanner();
		virtual bool DBScannerScan();
		inline eUBScannerResults GetDBScannerResult() const {
			return m_lDBScannerResult;
		}
		inline bool IsDBScannerBreaked() const {
			return m_bDBScannerBreaked;
		}
		inline bool IsWhereOK() const {
			return m_clDBScannerWhere.OK();
		}
		bool OnDBScannerCheckAfterRead(int64_t, int64_t, int64_t, int64_t);
	protected:
		virtual bool OnDBScannerSelect();
		virtual bool OnDBScannerWhere();
		virtual bool OnDBScannerGroup();
		virtual bool OnDBScannerOrder();
		virtual bool OnDBScannerStart();
		virtual bool OnDBScannerSQLReady();
		virtual bool OnDBScannerScan();
		virtual void OnDBScannerScanned();
		bool DBScannerAddWhereDTime(const CRJStringByte&,const CRJStringByte&,const CRJStringByte&);
		bool DBScannerAddWhere(uint8_t*, size_t,bool bAnd=true);
		bool DBScannerAddWhere(const CRJStringByte&,bool bAnd=true);
		bool DBScannerAddWhere(const char*, int64_t,char op='=',bool bAnd=true);
		bool DBScannerAddWhereBrackets(const char*, int64_t,char op='=',bool bAnd=true);
		bool DBScannerAddWhereBrackets(const char*, uint8_t*, size_t,char op='=',bool bAnd=true);
		bool DBScannerAddWhereBracketsExact(bool,const char*, uint8_t*, size_t,char op='=',bool bAnd=true);
		bool DBScannerAddWhereBrackets(const char*, uint8_t*, size_t,const char*,bool bAnd=true);
		bool DBScannerAddWhereBracketsLike(const char*, uint8_t*, size_t,bool bAnd=true);
		bool DBScannerAddWhereBracketsLikeRight(const char*, uint8_t*, size_t,bool bAnd=true);
		bool DBScannerAddWhereBracketsLikeRight(const char*,const CRJStringByte&,bool bAnd=true);
		bool DBScannerAddWhereBracketsIn(const char*, uint8_t*, size_t,bool bAnd=true);
		bool DBScannerAddWhereBracketsIn(const char*,const CRJStringByte&,bool bAnd=true);
		bool OnDBScannerReset();
		void DBScannerQueryProcess(bool);
		inline bool HasWhere() const {
			return (m_clDBScannerWhere.GetLength()>0);
		}
		inline bool HasOrder() const {
			return (m_clDBScannerOrder.GetLength()>0);
		}
		inline bool HasGroup() const {
			return (m_clDBScannerGroup.GetLength()>0);
		}
		inline bool DBScannerSetBreaked() {
			m_bDBScannerBreaked=true;
			return false;
		}
	protected:
		const CRJStringByte& c_clSelect;
		CRJStringByteExpanded m_clTemp, m_clDBScannerSql, m_clDBScannerWhere, m_clDBScannerOrder,
		m_clDBScannerGroup;
		size_t m_sCount;
		bool m_bDBScannerBreaked;
		eUBScannerResults m_lDBScannerResult;
	};
}

