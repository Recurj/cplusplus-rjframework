#pragma once
#include <local/rj/modules/modstorage.h>
#include <local/rj/libs/base/application/application.h>
#include <local/rj/libs/storage/rjdbstorage.h>
namespace uwlStorage {
	class UWLSTORAGE_API CRJStorage:public IAppStorage {
		CRJStorage();
		virtual~CRJStorage();
		bool Open(const CRJFileName&);
		bool AddCatalog(int64_t,const CRJStringByte&, int64_t&);
		bool AddVariable(int64_t,const CRJStringByte&,const CRJStringByte&);
		virtual bool GetIdSection(int64_t,const CRJStringByte&, int64_t&);
		virtual bool GetIdVariable(int64_t,const CRJStringByte&, int64_t&);
		virtual bool GetString(int64_t, int64_t, CRJStringByteVar&);
		virtual bool GetInt(int64_t, int64_t, int64_t&);
		virtual bool GetDouble(int64_t, int64_t,double&);
		virtual bool GetBool(int64_t, int64_t,bool&);
		virtual bool PutString(int64_t, int64_t,const CRJStringByte&);
		virtual bool PutInt(int64_t, int64_t, int64_t);
		virtual bool PutDouble(int64_t, int64_t,double);
		virtual bool PutBool(int64_t, int64_t,bool);
	protected:
		CDBStorage m_clDB;
	};
}

