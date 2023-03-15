#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections/btrees/index.h>
using namespace rjBase::BTree;
rjBase::CRJAllocatorRuntime CRJTreeIndex::CRJTreeIndex::CNodeCatalogLeaves::m_clHeap(Allocator::Region::BTree, AllocatorBTree::CatalogLeavesIndex, sizeof(CNodeCatalogLeaves));
rjBase::CRJAllocatorRuntime CRJTreeIndex::CRJTreeIndex::CNodeCatalog::m_clHeap(Allocator::Region::BTree, AllocatorBTree::CatalogIndex, sizeof(CNodeCatalog));
CRJTreeIndex::CNodeEmpty CRJTreeIndex::s_Empty;
CRJTreeIndex::CNodeEmpty::CNodeEmpty()
{}
CRJTreeIndex::CNodeEmpty::~CNodeEmpty()
{}
Something CRJTreeIndex::CNodeEmpty::Find(size_t) const
{return nullptr;}
CRJTreeIndex::AddResult  CRJTreeIndex::CNodeEmpty::Add(CRJTreeIndex& tree, Something leaf) {
	CNodeCatalogLeaves* p = new CNodeCatalogLeaves(leaf);
	if (p != nullptr) {
		tree.setRoot(*p);
		return AddResult::OK;
	}
	return AddResult::Error;
}
CRJTreeIndex::CNodeCatalogLeaves::CNodeCatalogLeaves(Something leaf) :
	m_sCount(1) {
	m_pNodes[0] = leaf;
}
CRJTreeIndex::CNodeCatalogLeaves::~CNodeCatalogLeaves() {
	cleanup();
}
void* CRJTreeIndex::CNodeCatalogLeaves::operator new(size_t s) {
	return m_clHeap.Alloc(s);
}
void CRJTreeIndex::CNodeCatalogLeaves::operator delete(void* p) {
	m_clHeap.Free((uint8_t*)p);
}
void CRJTreeIndex::CNodeCatalogLeaves::cleanup() {
	m_sCount = 0;
}
Something CRJTreeIndex::CNodeCatalogLeaves::Find(size_t s) const {
	return (m_sCount <= s) ? nullptr : m_pNodes[s];
}
CRJTreeIndex::AddResult CRJTreeIndex::CNodeCatalogLeaves::Add(CRJTreeIndex& tree, Something leaf) {
	if (m_sCount >= cRJTreeIndexCatalog) {
		return tree.isItRoot(this) ? createRoot(tree, leaf) : AddResult::NeedNode;
	}
	return placeAvailable(leaf);
}

CRJTreeIndex::AddResult CRJTreeIndex::CNodeCatalogLeaves::placeAvailable(Something leaf) {
	m_pNodes[m_sCount] = leaf;
	m_sCount++;
	return AddResult::OK;
}
CRJTreeIndex::AddResult  CRJTreeIndex::CNodeCatalogLeaves::createRoot(CRJTreeIndex& tree, Something leaf) {
	CNode* q = tree.createBranch(leaf,1);
	if (q != nullptr) {
		CNodeCatalog* parent = new CNodeCatalog(this, q, 1);
		if (parent) {
			tree.setRoot(*parent);
			return AddResult::OK;
		}
	}
	return AddResult::Error;
}
CRJTreeIndex::CNodeCatalog::CNodeCatalog(CNode* f, CNode* s, size_t level) :
	m_sCount(2), m_sLevel(level) {
	m_pNodes[0] = f;
	m_pNodes[1] = s;
}
CRJTreeIndex::CNodeCatalog::CNodeCatalog(CNode& node, size_t level) :
	m_sCount(1), m_sLevel(level) {
	m_pNodes[0] = &node;
}
CRJTreeIndex::CNodeCatalog::~CNodeCatalog() {
	cleanup();
}
void* CRJTreeIndex::CNodeCatalog::operator new(size_t s) {
	return m_clHeap.Alloc(s);
}
void CRJTreeIndex::CNodeCatalog::operator delete(void* p) {
	m_clHeap.Free((uint8_t*)p);
}
void CRJTreeIndex::CNodeCatalog::cleanup() {
	for (size_t i = 0; i < m_sCount; i++) delete m_pNodes[i];
	m_sCount = 0;
}
Something CRJTreeIndex::CNodeCatalog::Find(size_t v) const {
	size_t s = (m_sLevel << 2) + m_sLevel;
	size_t child = v >> s;
	return m_pNodes[child]->Find(v - (child << s));
}
CRJTreeIndex::AddResult CRJTreeIndex::CNodeCatalog::Add(CRJTreeIndex& tree, Something leaf) {
	CRJTreeIndex::AddResult r = m_pNodes[m_sCount - 1]->Add(tree, leaf);
	if (r == AddResult::NeedNode) {
		if (m_sCount >= cRJTreeIndexCatalog) {
			if (tree.isItRoot(this)) return createRoot(tree, leaf);
			return AddResult::NeedNode;
		}
		CNode* q = tree.createBranch(leaf,m_sLevel);
		if (q == nullptr) return AddResult::Error;
		m_pNodes[m_sCount] = q;
		m_sCount++;
		return AddResult::OK;
	}
	return r;
}
CRJTreeIndex::AddResult  CRJTreeIndex::CNodeCatalog::createRoot(CRJTreeIndex& tree, Something leaf) {
	CNode* q = tree.createBranch(leaf,m_sLevel + 1);
	if (q != nullptr) {
		CNodeCatalog* parent = new CNodeCatalog(this, q, m_sLevel + 1);
		if (parent) {
			tree.setRoot(*parent);
			return AddResult::OK;
		}
	}
	return AddResult::Error;
}
CRJTreeIndex::CRJTreeIndex()
{
	setRoot(s_Empty);
}
CRJTreeIndex::~CRJTreeIndex()
{
	Clear();
}
void CRJTreeIndex::Clear()
{
	if (!isItRoot(&s_Empty)) {
		delete m_pRoot;
		setRoot(s_Empty);
	}
}
CRJTreeIndex::CNode* CRJTreeIndex::createBranch(Something leaf,size_t level) {
	CNode* q = new CNodeCatalogLeaves(leaf);
	if (q != nullptr) {
		size_t s = 1;
		CNode* p = nullptr;
		while (s < level) {
			p = q;
			q = new CNodeCatalog(*p, s);
			if (!q) {
				delete p;
				return nullptr;
			}
			s++;
		}
		return q;
	}
	return nullptr;
}

void CRJTreeIndex::setRoot(CNode& node)
{
	m_pRoot = &node;
}
bool CRJTreeIndex::isItRoot(CNode* p) const
{
	return m_pRoot == p;
}
bool CRJTreeIndex::Add(Something leaf)
{
	return m_pRoot->Add(*this, leaf) == AddResult::OK;
}
CRJTreeIndex::CNode::CNode()
{}
CRJTreeIndex::CNode::~CNode()
{}
