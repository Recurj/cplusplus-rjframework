#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections/btrees/treeint64.h>
using namespace rjBase::BTree;
CTreeInt64::CTreeInt64() {}
CTreeInt64::~CTreeInt64() {
	Clear();
}
void CTreeInt64::Clear() {
	m_sCache.Reset();
	CRJRBTree::Clear();
}
CRJRBTreeLeafInt64* CTreeInt64::Lookup(int64_t key) const {
	if (m_sCache.Has(key)) return m_sCache.m_pLastNode;
	CRJRBTreeLeafInt64 compare(key);
	auto p = (CRJRBTreeLeafInt64*)find(compare);
	if (p!=nullptr) {
		m_sCache.Set(*p);
		return p;
	}
	return nullptr;
}
bool CTreeInt64::Insert(CRJRBTreeLeafInt64& leaf) {
	if (insert(leaf)) {
		m_sCache.Set(leaf);
		return true;
	}
	return false;
}

bool CTreeInt64::Delete(int64_t key) {
	CRJRBTreeLeafInt64 compare(key);
	if (drop(compare)) {
		m_sCache.Reset();
		return true;
	}
	return false;
}
 bool CTreeInt64::onInsertFound(CRJRBTreeLeaf&, CRJRBTreeLeaf& found)
{
	m_sCache.Set((CRJRBTreeLeafInt64&)found);
	return false;
}