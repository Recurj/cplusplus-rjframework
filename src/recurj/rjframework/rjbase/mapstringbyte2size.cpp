#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections//maps/stringbyte2size.h>
using namespace rjBase;
CRJAllocatorRuntime CMapStringByte2Size::CElem::m_clHeap(0, 0,sizeof(CMapStringByte2Size::CElem));
CMapStringByte2Size::CMapStringByte2Size() {}
CMapStringByte2Size::~CMapStringByte2Size() {
	Clear();
}
CMapStringByte2Size::CElem::CElem(const CRJStringByte& key, size_t value):
	CRJRBTreeLeafStringByte(key),
m_clValue(value) {}
CMapStringByte2Size::CElem::~CElem() {}
size_t CMapStringByte2Size::CElem::GetValue() {
	return m_clValue;
}
const CRJStringByte& CMapStringByte2Size::CElem::Key() const
{
	return m_clKey;
}

void CMapStringByte2Size::CElem::SetValue(size_t v) {
	m_clValue=v;
}
void* CMapStringByte2Size::CElem::operator new(size_t s) {
	return m_clHeap.Alloc(s);
}
void CMapStringByte2Size::CElem::operator delete(void* p) {
	m_clHeap.Free((uint8_t*)p);
}
bool CMapStringByte2Size::Remove(const CRJStringByte& key) {
	return Delete(key);
}
bool CMapStringByte2Size::Exist(const CRJStringByte& key) {
	return rjBase::BTree::CTreeStringByte::Lookup(key) != nullptr;
}
bool CMapStringByte2Size::Map(const CRJStringByte& key, size_t value) {
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
bool CMapStringByte2Size::Lookup(const CRJStringByte& key, size_t& clValue) {
	auto p=rjBase::BTree::CTreeStringByte::Lookup(key);
	if (p!=nullptr) {
		clValue=((CElem*) p)->GetValue();
		return true;
	}
	return false;
}

