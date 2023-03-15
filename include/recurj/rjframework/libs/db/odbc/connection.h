#pragma once
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/strings/unicode/var.h>
#include <recurj/rjframework/libs/base/application/application.h>
#include <recurj/rjframework/libs/db/odbc/handleconnection.h>
#include <recurj/rjframework/libs/db/odbc/cursor.h>
#include <recurj/rjframework/libs/db/dbinterfaces.h>
#include <recurj/rjframework/libs/db/dbconnectioninfo.h>
namespace rjDb {
	class CRJDBSQL;
	class rjBase::CRJStringUnicode;
	const size_t cDBStringOdbcStateLen=32;
	const size_t cDBStringOdbcErrorLen=1024+cDBStringOdbcStateLen;
	using CDBStringOdbcState=CRJStringByteVarBuffer<cDBStringOdbcStateLen>;
	using CDBStringOdbcError=CRJStringByteVarBuffer<cDBStringOdbcErrorLen>;
	constexpr size_t DB_State_WasUpdatedStatus=1;
	constexpr size_t DB_State_AutoCommit=2;
	constexpr size_t DB_State_ReadOnly=4;
	constexpr size_t DB_State_ShowWarning=8;
	constexpr size_t DB_Behavior_Rollback=0;
	constexpr size_t DB_Behavior_Commit=1;
	enum ErrorDB:int64_t {
		Init=2001, Connect, Prepare, Find
	};
	class IRJDBConnector {
	public:
		virtual bool DBConnect(const CRJStringByte&,const CRJStringByte&, SDBConnectionOptions&)=0;
	};
	class RJDB_API CRJDBConnection:public IRJDBError,public IRJDBConnector {
		using funcStatusUpdated=void(CRJDBConnection::*)();
	public:
		CRJDBConnection(IRJDBConnectionInfo&);
		CRJDBConnection(IRJDBConnectionInfo&,const CRJStringByte&);
		virtual~CRJDBConnection();
		static bool DBAppOpen();
		static void DBAppClose();
		static bool DPAppWasOpened();
		static void DBError(int64_t, int32_t lLevel=cLoggerLevelCritical);
		static void DBError(int64_t,const CRJStringByte&, int32_t lLevel=cLoggerLevelCritical);
		virtual void OnDBShowError(bool);
		virtual void OnDBShowErrorOnStatement(CRJDBStatement&,bool);
		virtual CRJDBConnection& DBReconnect();
		virtual void OnDBError(bool);
		virtual bool DBNeedBreak(int64_t);
		virtual bool DBResult(bool d=true);
		virtual void DBQueryProcess(bool);
		virtual void DBQueryBreak();
		virtual bool IsDBConnectionAlive();
		virtual int32_t GetDBConnectionItSelfDebugStatusLevel();
		virtual void SetDBTraceOn(const char*);
		virtual void SetDBTraceOff();
		bool DBConnect(const CRJStringByte&,const CRJStringByte&, SDBConnectionOptions&) override;
		bool DBConnect();
		void DBDisconnect();
		bool OnDBErrorOnStatement(CRJDBStatement&,bool);
		void OnDBExec(CRJDBStatement&);
		bool HasDBError(CRJDBStatement&);
		bool HasDBErrorConstraint(CRJDBStatement&);
		CRJStringByte& DBFormatError();
		void DBPushCursor(CRJDBCursor&);
		void DBPopCursor(CRJDBCursor&);
		inline CRJDBConnection& GetDBItSelf() {
			return *this;
		}
		inline bool GetDBUpdateStatus() const {
			return m_sDBState& DB_State_WasUpdatedStatus;
		}
		void DBNoError();
		bool DBResultAfterExec(bool);
		inline int64_t IsDBQueryWasBreaked() const {
			return m_clBreakFlag.anGet();
		}
		inline void SetDBShowWarning(bool b) {
			DBUpdateState(b, DB_State_ShowWarning);
		}
		inline bool IsDBConnect() const {
			return m_clDBHandle.GetDBConnection();
		}
		bool IsDBResultOK(SQLRETURN, IRJDBError&);
		static int32_t GetDBConnectionItSelfDebugLevel(bool);
		inline const CRJStringByte& GetDBErrorState() const {
			return m_clDBState;
		}
		inline const CRJStringByte& GetDBErrorNative() const {
			return m_clDBErrorNative;
		}
		inline const CRJStringByte& GetDBError() const {
			return m_clDBErrorFormated;
		}
		inline int64_t GetDBConnectionItSelfErrorNativeCode() const {
			return m_lDBErrorNativeCode;
		}
		inline size_t GetDBConnectionItSelfDebugFetched() const {
			return m_sDBDebugFetched;
		}
		inline size_t GetDBConnectionItSelfDebugExecuted() const {
			return m_sDBDebugExecuted;
		}
		inline bool HasSupport64BitsFields() const {
			return m_bSupport64BitsFields;
		}
		inline IRJDBConnectionInfo& GetDBConnectionInfo() {
			return m_clDBConnectionInfo;
		}
		void SetDBAutoCommit(bool);
		void SetDBReadOnly(bool);
		SQLHDBC GetDBConnectionItSelfConnectionHandle();
		bool SQLExec(const CRJStringByte&);
	protected:
		void DBSetDebug();
		virtual void OnDBConnected(const CRJStringByte&,const SDBConnectionOptions&);
		virtual void OnDBShowConnectStatus();
		virtual void OnDBInitSeassion(const CRJStringByte&);
		virtual void OnDBInitApp();
		virtual void OnDBStatementsResult(int64_t);
		virtual void DBInitSessionRunSql(CRJDBSQL&);
		virtual void OnDBConnStatusWasChanged();
		void OnDBConnectStatus();
		void DBInitSessionFromFile(CRJFileName&);
		void DBResetCursors();
		bool OnDBErrorInfoEnv();
		void OnConnStatusNoChanged();
		void OnConnStatusWasChanged();
		inline bool DBHasOpenedCursors() const {
			return m_clDBCursors.Count()>0;
		}
		inline bool DBShowWarning() const {
			return m_sDBState& DB_State_ShowWarning;
		}
		inline void DBUpdateState(bool b, size_t flag) {
			if (b) m_sDBState|=flag;
			else m_sDBState&=(~flag);
		}
	private:
		void OnDBErrorUnknown();
		bool OnDBErrorInfo(SQLHSTMT, SQLSMALLINT);
		bool DBConnectStep(const CRJStringByte&,const CRJStringByte&,const SDBConnectionOptions&);
		bool DBDriverConnect(const CRJStringByte&);
		void DBDriverDisconnect();
		bool DBAlloc(const SDBConnectionOptions&);
		void DBFree();
		SQLRETURN DBAllocHandle();
		bool SetOdbcAutoCommit();
		bool SetOdbcReadOnly();
	private:
		void DBInit();
	protected:
		static SQLHENV m_hDBApplEnv;
		static const char c_szDBStateHY00[8];
		static const char c_szDBState4200[8];
		static const CRJStringByte m_clPhraseOdbcError;
		static const CRJStringByte m_clPhraseOdbcNative;
		static std::mutex m_clDBAccessEnv;
		static funcStatusUpdated m_clFuncOnOnStatusComplete[2];
		IRJDBConnectionInfo& m_clDBConnectionInfo;
		CRJStringByteAllocated m_clDBReference;
		CRJLatch m_clBreakFlag;
		CRJDBHandleConnection m_clDBHandle;
		CRJDBListCursors m_clDBCursors;
		CDBStringOdbcState m_clDBState;
		CDBStringOdbcError m_clDBErrorNative;
		CDBStringOdbcError m_clDBErrorFormated;
		size_t m_sDBState;
		bool m_bSupport64BitsFields;
		int64_t m_lDBReconnectFlag;
		int64_t m_lDBErrorNativeCode;
		size_t m_sDBDebugFetched;
		size_t m_sDBDebugExecuted;
		int64_t m_lDBBehavior[2];
	};
	class RJDB_API CRJDBCommitByResult final {
	public:
		CRJDBCommitByResult(CRJDBConnection& clDB):
			m_clDB(clDB) {
			m_bResult=false;
		}
		CRJDBCommitByResult(CRJDBConnection& clDB,bool b):
			m_clDB(clDB),
		m_bResult(b) {}~CRJDBCommitByResult() {
			m_clDB.DBResult(m_bResult);
		}
		bool SetResult(bool b) {
			m_bResult=b;
			return b;
		}
		CRJDBConnection& m_clDB;
		bool m_bResult;
	};
	class RJDB_API CRJDBCommitAuto final {
	public:
		CRJDBCommitAuto(CRJDBConnection& clDB):
			m_clDB(clDB) {}~CRJDBCommitAuto() {
			m_clDB.DBResult(true);
		}
		CRJDBConnection& m_clDB;
	};
}

