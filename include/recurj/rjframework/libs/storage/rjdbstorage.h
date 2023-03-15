#pragma once
#include <local/rj/modules/modstorage.h>
#include <local/rj/libs/base/application/application.h>
#include <local/rj/thirdparty/sqlite/sqlitedb.h>
#include <local/rj/thirdparty/sqlite/sqlitedbcursor.h>
using namespace rjSQLite;
namespace uwlStorage {
	constexpr int64_t NameCatalog=0;
	constexpr int64_t NameVariable=1;
	class CDBStorage:public CRJSQLiteDB {
	public:
		CDBStorage();
		virtual~CDBStorage();
		bool AddCatalog(int64_t,const CRJStringByte&, int64_t&);
		bool AddVariable(int64_t,const CRJStringByte&,const CRJStringByte&);
		bool Rename(int64_t, int64_t,const CRJStringByte&,const CRJStringByte&);
		bool FindName(int64_t, int64_t,const CRJStringByte&, int64_t&);
		bool FindValue(int64_t, int64_t, CRJStringByteVar&);
		bool UpdateValue(int64_t, int64_t,const CRJStringByte&);
	protected:
		virtual bool DBPrepare();
		virtual bool DBCreate();
		bool AddName(int64_t, int64_t, int64_t,const CRJStringByte&);
		bool AddValue(int64_t, int64_t,const CRJStringByte&);
	protected:
		static CRJStringByte s_TableCatalogs;
		static CRJStringByte s_TableVars;
		static CRJStringByte s_TableNames;
		static CRJStringByte s_TableValues;
		static CRJStringByte s_sqlFindName;
		static CRJStringByte s_sqlUpdateName;
		static CRJStringByte s_sqlFindValue;
		static CRJStringByte s_sqlUpdateValue;
		static CRJStringByte s_sqlInsertCID;
		static CRJStringByte s_sqlInsertVID;
		static CRJStringByte s_sqlInsertName;
		static CRJStringByte s_sqlInsertValue;
		CRJSQLiteDBStatementCursor m_sqlFindName;
		CRJSQLiteDBStatementCursor m_sqlFindValue;
	};
}

