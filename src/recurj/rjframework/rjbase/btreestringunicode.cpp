#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections/btrees/treestringunicode.h>
using namespace rjBase::BTree;
CTreeStringUnicode::CTreeStringUnicode() {}
CTreeStringUnicode::~CTreeStringUnicode() {
	Clear();
}
void CTreeStringUnicode::Clear() {
	m_sCache.Reset();
	CRJRBTree::Clear();
}
CRJRBTreeLeafStringUnicode* CTreeStringUnicode::Lookup(const CRJStringUnicode& key) const {
	if (m_sCache.Has(key)) return m_sCache.m_pLastNode;
	CRJRBTreeCompareStringUnicodeRef compare(key);
	auto p = (CRJRBTreeLeafStringUnicode*)find(compare);
	if (p != nullptr) {
		m_sCache.Set(*p);
		return p;
	}
	return nullptr;
}
bool CTreeStringUnicode::Insert(CRJRBTreeLeafStringUnicode& leaf) {
	if (insert(leaf)) {
		m_sCache.Set(leaf);
		return true;
	}
	return false;
}
bool CTreeStringUnicode::onInsertFound(CRJRBTreeLeaf&, CRJRBTreeLeaf& found)
{
	m_sCache.Set((CRJRBTreeLeafStringUnicode&)found);
	return false;
}
bool CTreeStringUnicode::Delete(const CRJStringUnicode& key) {
	CRJRBTreeCompareStringUnicodeRef compare(key);
	if (drop(compare)) {
		m_sCache.Reset();
		return true;
	}
	return false;
}
