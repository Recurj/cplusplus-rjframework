#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections//maps/handbook.h>
using namespace rjBase;
CRJAllocatorRuntime CMapHandbook::CElem::m_clHeap(Allocator::Region::Map, AllocatorMap::HeapHandbook,sizeof(CMapHandbook::CElem));
CMapHandbook::CMapHandbook() {}
CMapHandbook::~CMapHandbook() {
	Clear();
}
CMapHandbook::CElem::CElem(int64_t key,const CRJStringByte& value):
	CRJRBTreeLeafInt64(key),
m_clValue(value) {}
CMapHandbook::CElem::~CElem() {}
const CRJStringByte& CMapHandbook::CElem::GetValue() {
	return m_clValue;
}
void* CMapHandbook::CElem::operator new(size_t s) {
	return m_clHeap.Alloc(s);
}
void CMapHandbook::CElem::operator delete(void* p) {
	m_clHeap.Free((uint8_t*)p);
}
bool CMapHandbook::Remove(int64_t key) {
	return Delete(key);
}
bool CMapHandbook::Exist(int64_t key) {
	CRJStringByteFlex v;
	return Lookup(key, v);
}
bool CMapHandbook::Map(int64_t key,const CRJStringByte& value) {
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
bool CMapHandbook::Lookup(int64_t key, CRJStringByteFlex& clValue) {
	auto p=rjBase::BTree::CTreeInt64::Lookup(key);
	if (p!=nullptr) {
		clValue=((CElem*) p)->GetValue();
		return true;
	}
	return false;
}
CMapHandbook::CElem* CMapHandbook::Node(int64_t key) {
	return (CMapHandbook::CElem*) rjBase::BTree::CTreeInt64::Lookup(key);
}

