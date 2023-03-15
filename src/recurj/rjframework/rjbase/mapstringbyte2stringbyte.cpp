#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections//maps/stringbyte2stringbyte.h>
using namespace rjBase;
CRJAllocatorRuntime CMapStringByte2StringByte::CElem::m_clHeap(Allocator::Region::Map, AllocatorMap::HeapStringByte2StringByte,sizeof(CMapStringByte2StringByte::CElem));
CMapStringByte2StringByte::CMapStringByte2StringByte() {}
CMapStringByte2StringByte::~CMapStringByte2StringByte() {
	Clear();
}
const CRJStringByte& CMapStringByte2StringByte::CElem::Key() const
{
	return m_clKey;
}
CMapStringByte2StringByte::CElem::CElem(const CRJStringByte& key,const CRJStringByte& value):
	CRJRBTreeLeafStringByte(key),
m_clValue(value) {}
CMapStringByte2StringByte::CElem::~CElem() {}
const CRJStringByte& CMapStringByte2StringByte::CElem::GetValue() {
	return m_clValue;
}
void* CMapStringByte2StringByte::CElem::operator new(size_t s) {
	return m_clHeap.Alloc(s);
}
void CMapStringByte2StringByte::CElem::operator delete(void* p) {
	m_clHeap.Free((uint8_t*)p);
}
bool CMapStringByte2StringByte::Remove(const CRJStringByte& key) {
	return Delete(key);
}
bool CMapStringByte2StringByte::Exist(const CRJStringByte& key) {
	CRJStringByteFlex v;
	return Lookup(key, v);
}
bool CMapStringByte2StringByte::Map(const CRJStringByte& key,const CRJStringByte& value) {
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
bool CMapStringByte2StringByte::Lookup(const CRJStringByte& key, CRJStringByteFlex& clValue) {
	auto p=rjBase::BTree::CTreeStringByte::Lookup(key);
	if (p!=nullptr) {
		clValue=((CElem*) p)->GetValue();
		return true;
	}
	return false;
}
CMapStringByte2StringByte::CElem* CMapStringByte2StringByte::Node(const CRJStringByte& key) {
	return (CMapStringByte2StringByte::CElem*) rjBase::BTree::CTreeStringByte::Lookup(key);
}

