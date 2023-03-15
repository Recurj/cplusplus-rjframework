#include "stdafx.h"
#include <recurj/rjframework/libs/db/dbconnectioninfomap.h>
using namespace rjDb;
CRJAllocatorRuntime CRJDBCatalogMap::CElem::m_clHeap(Allocator::Region::DB, AllocatorDB::Info,sizeof(CRJDBCatalogMap::CElem));
CRJDBCatalogMap::CRJDBCatalogMap() {}
CRJDBCatalogMap::~CRJDBCatalogMap() {
	Clear();
}
CRJDBCatalogMap::CElem::CElem(const CRJStringByte& key,const CRJDBConnectInfo& value):
	CRJRBTreeLeafStringByte(key),
m_clValue(value) {}
CRJDBCatalogMap::CElem::~CElem() {}
CRJDBConnectInfo& CRJDBCatalogMap::CElem::GetValue() {
	return m_clValue;
}
void* CRJDBCatalogMap::CElem::operator new(size_t s) {
	return m_clHeap.Alloc(s);
}
void CRJDBCatalogMap::CElem::operator delete(void* p) {
	m_clHeap.Free((uint8_t*)p);
}
bool CRJDBCatalogMap::Remove(const CRJStringByte& key) {
	return Delete(key);
}
bool CRJDBCatalogMap::Exist(const CRJStringByte& key) {
	return BTree::CTreeStringByte::Lookup(key)!=nullptr;
}
bool CRJDBCatalogMap::Map(const CRJStringByte& key,const CRJDBConnectInfo& value) {
	try {
		CElem* pElem=new CElem(key, value);
		if (pElem!=nullptr) {
			if (Insert(* pElem)) return true;
			delete pElem;
			return false;
		}
	}
	catch(...) {}
	return false;
}
CRJDBConnectInfo* CRJDBCatalogMap::Lookup(const CRJStringByte& key) {
	auto p=BTree::CTreeStringByte::Lookup(key);
	if (p!=nullptr) return &((CElem*) p)->GetValue();
	return nullptr;
}

