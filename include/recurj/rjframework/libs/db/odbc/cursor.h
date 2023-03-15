#pragma once
#include <recurj/rjframework/libs/base/collections/lists/defs.h>
#include <recurj/rjframework/libs/db/db.h>
#include <recurj/rjframework/libs/db/odbc/statement.h>
#define mdbCursorSetUnicodeParamSize(s, ind) ind = (SQLLEN)(s.GetSize() << 1)
#define mdbCursorGetUnicodeParamSize(s) (SQLLEN)(s.GetSize() << 1)
#define mdbCursorFormatResultLabel(ind,buf) CRJDBCursor::DBCursorFormatResultBytes(m_lIndicators[ind],buf,sizeof(buf))
#define mdbCursorFormatResultString(ind,clStr) CRJDBCursor::DBCursorFormatResultString(m_lIndicators[ind],clStr)
#define mdbCursorFormatResultStringDTime(ind,clStr) CRJDBCursor::SQLStatementFormatDTimeResult(m_lIndicators[ind],clStr)
namespace rjDb {
	class RJDB_API CRJDBCursor :public CRJListItem<CRJDBCursor>, public CRJDBStatement {
		friend class CRJDBCursorAutoCommit;
		using funcFetched = void(CRJDBCursor::*)();
	public:
		CRJDBCursor(CRJDBConnection&, const CRJStringByte&);
		virtual~CRJDBCursor();
		bool DBCursorScan();
		bool DBCursorNext();
		bool DBCursorFind();
		void DBCursorClose();
		bool DBCursorCloseAfterExec(bool);
		bool DBCursorGet(size_t, CRJStringByteVar&);
		bool DBCursorGet(size_t, CRJStringUnicodeVar&);
		bool DBCursorGet(size_t, void*, size_t, CRJStringByteVar&);
		bool DBCursorGet(size_t, size_t&, bool* b = nullptr);
		bool DBCursorGet(size_t, int32_t&, bool* b = nullptr);
		bool DBCursorGet(size_t, int64_t&, bool* b = nullptr);
		bool DBCursorGetDouble(size_t, double&, bool* b = nullptr);
		bool DBCursorResult(size_t, CRJDBSQLSize&);
		bool DBCursorResult(size_t, CRJDBSQLInt64&);
		bool DBCursorResult(size_t, CRJDBSQLInt32&);
		bool DBCursorResult(size_t, CRJDBSQLStringByte&);
		bool DBCursorSetResultBytes(size_t, uint8_t*, size_t, SQLLEN&);
		bool DBCursorSetResultString(size_t, CRJStringByteVar&, SQLLEN&);
		bool DBCursorSetResultString(size_t, CRJStringUnicodeVar&, SQLLEN&);
		bool DBCursorSetResultInt32(size_t, int32_t&, SQLLEN&);
		bool DBCursorSetResultInt64(size_t, int64_t&, SQLLEN&);
		bool DBCursorSetResultSize(size_t, size_t&, SQLLEN&);
		bool DBCursorSetResultDouble(size_t, double&, SQLLEN&);
		bool DBCursorSetResultTime(size_t, time_t&, SQLLEN&);
		static size_t DBCursorFormatResult(CRJDBSQLStringByte&);
		static size_t DBCursorFormatResultBinary(CRJDBSQLStringByte&);
		static size_t DBCursorFormatResultBytes(SQLLEN&, uint8_t*, size_t);
		static size_t DBCursorFormatResultString(SQLLEN, CRJStringByteVar&);
		static size_t DBCursorFormatResultString(SQLLEN, CRJStringUnicodeVar&);
		static void DBCursorFormatResultInt64(SQLLEN, int64_t&, int64_t);
		static void DBCursorFormatResultSize(SQLLEN, size_t&, size_t);
		static void DBCursorFormatResultDouble(SQLLEN, double&, double);
	protected:
		CRJDBCursor(CRJDBConnection&);
		virtual void DBCursorDebugFetched(CRJLogger&);
		virtual void DBCursorFormatResults();
		void DBStatementRelease() override;
		void DBCursorOnFetchedDebugOff();
		void DBCursorOnFetchedDebugOn();
		void pushDb();
		void popDb();

	protected:
		static funcFetched m_stFuncTableStatementCursorFetched[2];
	};
	class RJDB_API CRJDBQuery final {
	public:
		CRJDBQuery(CRJDBCursor& cl) :
			m_clCursor(cl) {}
		~CRJDBQuery() {
			m_clCursor.DBCursorClose();
		}
		CRJDBCursor& m_clCursor;
	};
	//class RJDB_API CRJDBCursorAutoCommit {
	//public:
	//	CRJDBCursorAutoCommit(CRJDBCursor& cl) :
	//		m_clCursor(cl) {}
	//	~CRJDBCursorAutoCommit();
	//	CRJDBCursor& m_clCursor;
	//};
	using CRJDBListCursors = CRJList<CRJDBCursor>;
}

