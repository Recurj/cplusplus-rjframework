#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections/btrees/treesize.h>
using namespace rjBase::BTree;
CTreeSize::CTreeSize() {}
CTreeSize::~CTreeSize() {
	Clear();
}
void CTreeSize::Clear() {
	m_sCache.Reset();
	CRJRBTree::Clear();
}
CRJRBTreeLeafSize* CTreeSize::Lookup(size_t key) const {
	if (m_sCache.Has(key)) return m_sCache.m_pLastNode;
	CRJRBTreeLeafSize compare(key);
	auto p = (CRJRBTreeLeafSize*)find(compare);
	if (p!=nullptr) {
		m_sCache.Set(*p);
		return p;
	}
	return nullptr;
}
bool CTreeSize::Insert(CRJRBTreeLeafSize& leaf) {
	if (insert(leaf)) {
		m_sCache.Set(leaf);
		return true;
	}
	return false;
}
bool CTreeSize::onInsertFound(CRJRBTreeLeaf&, CRJRBTreeLeaf& found)
{
	m_sCache.Set((CRJRBTreeLeafSize&)found);
	return false;
}
bool CTreeSize::Delete(size_t key) {
	CRJRBTreeLeafSize compare(key);
	if (drop(compare)) {
		m_sCache.Reset();
		return true;
	}
	return false;
}
