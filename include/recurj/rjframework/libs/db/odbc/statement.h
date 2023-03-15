#pragma once
#include <recurj/rjframework/libs/base/strings/unicode/var.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/db/odbc/handlestatement.h>
#include <recurj/rjframework/modules/moddb.h>
#include <recurj/rjframework/libs/db/dbinterfaces.h>
#define mdbStatementSetUnicodeParamLength(s,ind) ind = (SQLLEN)(s.GetLength() << 1)
#define mdbStatementSetValueLength(ind,val) m_lIndicators[ind]=(SQLLEN)(val)
#define mdbStatementSetStringByteLength(ind,str) m_lIndicators[ind]=(SQLLEN)((str).GetLength())
#define mdbStatementSetStringUnicodeLength(ind,str) m_lIndicators[ind]=(SQLLEN)((str).GetLength()<< 1)
namespace rjDb {
	class CRJDBConnection;
	constexpr uint64_t DB_STATEMENT_PREPARED=1;
	constexpr uint64_t DB_STATEMENT_BUFFERS=2;
	constexpr uint64_t DB_STATEMENT_PARAMETERS=4;
	constexpr uint64_t DB_STATEMENT_IN_CURSOR=8;
	struct RJDB_API CRJDBSQLSize {
		operator size_t() const {return m_sValue;}
		inline void operator=(size_t v) {m_sValue=v;}
		inline void Reset() {m_sValue=0;}
		size_t m_sValue;
		SQLLEN m_lIndicator;
	};
	struct RJDB_API CRJDBSQLInt64 {
		operator int64_t() const {return m_lValue;}
		inline void operator=(int64_t v) {m_lValue=v;}
		inline void Reset() {m_lValue=0;}
		int64_t m_lValue;
		SQLLEN m_lIndicator;
	};
	struct RJDB_API CRJDBSQLDouble {
		operator double() const {return m_dValue;}
		inline void operator=(double v) {m_dValue=v;}
		inline void Reset() {m_dValue=0;}
		double m_dValue;
		SQLLEN m_lIndicator;
	};
	struct RJDB_API CRJDBSQLInt32 {
		operator int32_t() const {return m_lValue;}
		inline void operator=(int32_t v) {m_lValue=v;}
		inline void Reset() {m_lValue=0;}
		int32_t m_lValue;
		SQLLEN m_lIndicator;
	};
	class RJDB_API CRJDBSQLStringByte {
	public:
		virtual CRJStringByteVar& GetString() const=0;
		inline SQLLEN GetIndicator() const {return m_lIndicator;}
		operator const CRJStringByte&() const {	return GetString();	}
		inline void SetLabel(const char* p) {m_lIndicator=(SQLLEN) GetString().SetLabel(p).GetLength();}
		inline void SetLength(size_t s) {m_lIndicator=(SQLLEN) s;}
		inline void SetLength() {m_lIndicator=(SQLLEN) GetString().GetLength();}
		void Empty() {	GetString().SetEmpty();	m_lIndicator=0;}
		SQLLEN m_lIndicator;
	};
	class RJDB_API CRJDBSQLStringDTime:public CRJDBSQLStringByte {
	public:
		inline SQLLEN GetIndicator() const {return m_lIndicator;}
		CRJStringByteVar& GetString() const override {return (CRJStringByteVar&) m_clValue;};
		void SetCurrent() {m_clValue.SetDTApplCurrent();m_lIndicator=(SQLLEN) m_clValue.GetLength();}
		void Set(time_t t) {m_clValue.SetDTAppl(t);m_lIndicator=(SQLLEN) m_clValue.GetLength();}
		inline void operator=(const CRJStringByte& v) {m_clValue.Set(v);m_lIndicator=(SQLLEN) m_clValue.GetLength();}
		inline void operator=(const CRJStringDTime& v) {m_clValue=v;m_lIndicator=(SQLLEN) m_clValue.GetLength();}
		CRJStringDTime m_clValue;
	};
	class RJDB_API CRJDBSQLStringByteExpanded:public CRJDBSQLStringByte {
	public:
		inline SQLLEN GetIndicator() const {return m_lIndicator;}
		CRJStringByteVar& GetString() const override {return (CRJStringByteVar&) m_clValue;};
		inline void operator=(const CRJStringByte& v) {m_clValue.Set(v);m_lIndicator=(SQLLEN) m_clValue.GetLength();}
		CRJStringByteExpanded m_clValue;
	};
	template<size_t VOLUME>
	class CRJDBSQLStringByteBuffer:public CRJDBSQLStringByte {
	public:
		CRJStringByteVar& GetString() const override {
			return (CRJStringByteVar&) m_clValue;
		};
		inline void operator=(const CRJStringByte& v) {
			m_clValue.Set(v);
			m_lIndicator=(SQLLEN) m_clValue.GetLength();
		}
		void SetLabel(const char* p) {
			m_clValue.SetLabel(p);
			m_lIndicator=(SQLLEN) m_clValue.GetLength();
		}
		CRJStringByteVarBuffer<VOLUME>m_clValue;
	};
	template<typename T>
	class CRJDBSQLStringTyped:public CRJDBSQLStringByte {
	public:
		inline SQLLEN GetIndicator() const {
			return m_lIndicator;
		}
		CRJStringByteVar& GetString() const override {
			return (CRJStringByteVar&) m_clValue;
		};
		void Set(const CRJStringByte& v) {
			m_clValue.Set(v);
			m_lIndicator=(SQLLEN) m_clValue.GetLength();
		}
		const T& GetTypedValueConst() const {
			return m_clValue;
		}
		void operator=(const CRJStringByte& v) {
			m_clValue=v;
			m_lIndicator=(SQLLEN) m_clValue.GetLength();
		}
		T m_clValue;
	};
	class RJDB_API CRJDBStatement: public IRJDBError {
		friend class CRJDBScannerWhere;
	public:
		struct SRJDBColumnInfo {
			SQLCHAR szColumnName[256];
			SQLULEN lColumnSize;
			SQLSMALLINT lNameLength;
			SQLSMALLINT lDataType;
			SQLSMALLINT lDecimalDigits;
			SQLSMALLINT lNullable;
		};
		using funcExec=void(CRJDBStatement::*)();
		CRJDBStatement(CRJDBConnection&,const CRJStringByte&);
		virtual~CRJDBStatement();
		virtual void OnDBError(bool);
		virtual bool DBStatementIsDebugable(const CRJStringByte&);
		bool DBStatementDo();
		bool DBStatementExec();
		bool DBStatementIsPrepared();
		void DBStatementOpen();
		void DBStatementClose();
		bool DBStatementUnBind();
		bool DBStatementResetParameters();
		bool DBStatementParameter(size_t, CRJDBSQLSize&);
		bool DBStatementParameter(size_t, CRJDBSQLInt64&);
		bool DBStatementParameter(size_t, CRJDBSQLInt32&);
		bool DBStatementParameter(size_t, CRJDBSQLStringByte&);
		bool DBStatementSetParameterBytes(size_t, uint8_t*, size_t, SQLLEN&);
		bool DBStatementSetParameterString(size_t, CRJStringByteVar&, SQLLEN&);
		bool DBStatementSetParameterString(size_t, CRJStringUnicodeVar&, SQLLEN&);
		bool DBStatementSetParameterBytesVar(size_t, uint8_t*, size_t, SQLLEN&);
		bool DBStatementSetParameterStringVar(size_t, CRJStringByteVar&, SQLLEN&);
		bool DBStatementSetParameterStringVar(size_t, CRJStringUnicodeVar&, SQLLEN&);
		bool DBStatementSetParameterInt64(size_t, int64_t&, SQLLEN&);
		bool DBStatementSetParameterInt32(size_t, int32_t&, SQLLEN&);
		bool DBStatementSetParameterSize(size_t, size_t&, SQLLEN&);
		bool DBStatementSetParameterDouble(size_t,double&, SQLLEN&);
		bool DBStatementSetParameterTime(size_t, time_t&, SQLLEN&);
		void DBStatementSetSQL(const CRJStringByte&);
		bool DBStatementSetTimeout(int64_t);
		bool DBStatementResult(bool);
		int64_t DBStatementGetRowAffected();
		int64_t DBStatementIsBreaked() const;
		void DBStatementComplete(int64_t);
		bool DBStatementGetColumnInfo(size_t, SRJDBColumnInfo&);
		bool DBStatementGetColumnCount(size_t&);
		inline bool DBStatementIsPrepared() const {
			return (m_lSQLStatementState& DB_STATEMENT_PREPARED)!=0;
		}
		inline bool DBStatementIsBound() const {
			return (m_lSQLStatementState& DB_STATEMENT_BUFFERS)!=0;
		}
		inline bool DBStatementHasParameters() const {
			return (m_lSQLStatementState& DB_STATEMENT_PARAMETERS)!=0;
		}
		inline bool DBStatementIsCursor() const {
			return (m_lSQLStatementState& DB_STATEMENT_IN_CURSOR)!=0;
		}
		inline bool DBStatementCommit() {
			return DBStatementResult(true);
		}
		inline bool DBStatementRollback() {
			return DBStatementResult(false);
		}
		inline SQLHSTMT DBStatementGet() const {
			return m_clDBHandleStatement;
		}
		inline CRJDBConnection& DBStatementGetDB() {
			return m_clDB;
		}
		inline CRJDBStatement& DBStatementGetMe() {
			return *this;
		}
		inline CRJStringByte& DBStatementGetSQL() {
			return m_clSQLString;
		}
		inline bool DBStatementIsConstraintError(const CRJStringByte& str) const {
			return str==CRJDBStatement::m_clDBConstraintError;
		}
		static size_t DBStatementBinaryInput(CRJStringByteVar&,const uint8_t*, size_t);
		static size_t DBStatementBinaryInput(CRJDBSQLStringByte&,const uint8_t*, size_t);
	protected:
		CRJDBStatement(CRJDBConnection&);
		virtual bool DBStatementExecNoData();
		virtual bool DBStatementPrepareParameters();
		virtual void DBStatementDebugExecuted(CRJLogger&);
		virtual void DBStatementRelease();
		inline bool DBStatementIsResultOK(SQLRETURN);
		void DBStatementOnExecutedDebugOff();
		void DBStatementOnExecutedDebugOn();
		void DBStatementCompleteByClose();
		void DBStatementCompleteByRelease();
		bool DBStatementCancel();
		void DBStatementAlloc();
		void DBStatementDrop();
	private:
		bool DBStatementPrepare();
		bool DBStatementIsAllocated();
	protected:
		static CRJStringByte m_clDBConstraintError;
		static CRJStringByte m_clDBTRXNRollbacked;
		static CRJStringByte m_clSQLNotDefined;
		static CRJStringByte m_clStatementCompleteDelete;
		static CRJStringByte m_clStatementCompleteClose;
	public:
		static const CRJStringByte s_SQLSelect;
		static const CRJStringByte s_SQLFrom;
		static const CRJStringByte s_SQLWhere;
		static const CRJStringByte s_SQLInsert;
		static const CRJStringByte s_SQLInsertValues;
		static const CRJStringByte s_SQLUpdate;
		static const CRJStringByte s_SQLUpdateSet;
		static const CRJStringByte s_SQLDelete;
		static const CRJStringByte s_SQLOrderBy;
		static const CRJStringByte s_SQLGroupBy;
		static const CRJStringByte s_SQLWhereStart;
		static const CRJStringByte s_SQLWhereLikeEq;
		static const CRJStringByte s_SQLWhereInEq;
		static const CRJStringByte s_SQLWhereIntEnd;
		static const CRJStringByte s_SQLWhereStrEnd;
		static const CRJStringByte s_SQLWhereLikeEnd;
		static const CRJStringByte s_SQLWhereInEnd;
		static const CRJStringByte s_SQLAnd;
		static const CRJStringByte s_SQLOr;
		static const CRJStringByte s_SQLAsc;
		static const CRJStringByte s_SQLDesc;
	protected:
		static funcExec m_stFuncTableStatementBaseExecuted[2];
		CRJDBConnection& m_clDB;
		CRJDBHandleStatement m_clDBHandleStatement;
		CRJStringByteFlex m_clSQLString;
		uint64_t m_lSQLStatementState;
	};
}

