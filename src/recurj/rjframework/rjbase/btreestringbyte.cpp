#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections/btrees/treestringbyte.h>
using namespace rjBase::BTree;
CTreeStringByte::CTreeStringByte() {}
CTreeStringByte::~CTreeStringByte() {
	Clear();
}
void CTreeStringByte::Clear() {
	m_sCache.Reset();
	CRJRBTree::Clear();
}
CRJRBTreeLeafStringByte* CTreeStringByte::Lookup(const CRJStringByte& key) const {
	if (m_sCache.Has(key)) return m_sCache.m_pLastNode;
	CRJRBTreeCompareStringByteRef compare(key);
	auto p = (CRJRBTreeLeafStringByte*)find(compare);
	if (p != nullptr) {
		m_sCache.Set(*p);
		return p;
	}
	return nullptr;
}
bool CTreeStringByte::Insert(CRJRBTreeLeafStringByte& leaf) {
	if (insert(leaf)) {
		m_sCache.Set(leaf);
		return true;
	}
	return false;
}
bool CTreeStringByte::onInsertFound(CRJRBTreeLeaf&, CRJRBTreeLeaf& found)
{
	m_sCache.Set((CRJRBTreeLeafStringByte&)found);
	return false;
}
bool CTreeStringByte::Delete(const CRJStringByte& key) {
	CRJRBTreeCompareStringByteRef compare(key);
	if (drop(compare)) {
		m_sCache.Reset();
		return true;
	}
	return false;
}
