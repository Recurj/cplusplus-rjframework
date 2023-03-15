#pragma once
#include <recurj/rjframework/libs/base/collections/btrees/treestringbyte.h>
#include <recurj/rjframework/libs/db/dbconnectioninfo.h>
namespace rjDb {
	class CRJDBCatalogMap:public BTree::CTreeStringByte {
	public:
		CRJDBCatalogMap();
		virtual~CRJDBCatalogMap();
		class CElem:public BTree::CRJRBTreeLeafStringByte {
			friend class CRJDBCatalogMap;
		public:
			CElem(const CRJStringByte&,const CRJDBConnectInfo&);
			virtual~CElem();
			CRJDBConnectInfo& GetValue();
			void*operator new(size_t);
			void operator delete(void*);
			static CRJAllocatorRuntime m_clHeap;
		protected:
			CRJDBConnectInfo m_clValue;
		};
		bool Remove(const CRJStringByte&);
		bool Exist(const CRJStringByte&);
		bool Map(const CRJStringByte&,const CRJDBConnectInfo&);
		CRJDBConnectInfo* Lookup(const CRJStringByte&);
	};
	class RJDB_API CRJDBCatalog:public IRJDBConnectionInfo {
	public:
		CRJDBCatalog();
		virtual~CRJDBCatalog();
		bool DBConnectionInfoGet(const CRJStringByte&, CRJStringByteFlex&, CRJStringByteFlex&, SDBConnectionOptions&) override;
		bool DBConnectionInfoAdd(const CRJStringByte&,const CRJStringByte&,const CRJStringByte&, SDBConnectionOptions&) override;
	protected:
		CRJDBCatalogMap m_clTable;
	};
	class RJDB_API CRJDBConnectInfoShared:public CRJDBConnectInfo {
	public:
		bool DBConnectionInfoGet(const CRJStringByte&, CRJStringByteFlex&, CRJStringByteFlex&, SDBConnectionOptions&) override;
		bool DBConnectionInfoAdd(const CRJStringByte&,const CRJStringByte&,const CRJStringByte&, SDBConnectionOptions&) override;
	protected:
		mutable std::shared_mutex mutex;
	};
}

