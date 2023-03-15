#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections/btrees/rbtree.h>
using namespace rjBase::BTree;
CRJAllocatorRuntime CRJRBTree::CNode::m_clHeap(Allocator::Region::BTree, AllocatorBTree::RBTree, sizeof(CRJRBTree::CNode));
void* CRJRBTree::CNode::operator new(size_t s) {
	return m_clHeap.Alloc(s);
}
void  CRJRBTree::CNode::operator delete(void* p) {
	m_clHeap.Free((uint8_t*)p);
}
CRJRBTree::CNode::CNode(CRJRBTreeLeaf& leaf, CNode* parent, bool bRed) :
	m_clLeaf(leaf), m_bRed(bRed), m_pParent(parent)
{
	m_pChilds[NodeLeft] = nullptr;
	m_pChilds[NodeRight] = nullptr;
}
CRJRBTree::CNode::~CNode()
{
	CRJRBTreeLeaf::FreeLeaf(m_clLeaf);
	clear(m_pChilds[NodeLeft]);
	clear(m_pChilds[NodeRight]);
}
void CRJRBTree::CNode::clear(CNode* p) {
	if (p != nullptr) delete p;
}

size_t CRJRBTree::CNode::childMasks()
{
	size_t c = 0;
	if (m_pChilds[NodeLeft] != nullptr) c |= MaskLeft;
	if (m_pChilds[NodeRight] != nullptr) c |= MaskRight;
	return c;
}
CRJRBTree::CNode* CRJRBTree::CNode::sidemost(size_t dir,size_t place)
{
	CNode* p = m_pChilds[place];
	CNode* q = p->m_pChilds[dir];
	while (q != nullptr) {
		p = q;
		q = p->m_pChilds[dir];
	}
	return p;
}

bool CRJRBTree::CNode::childIsBlack(size_t dir)
{
	CNode* p = m_pChilds[dir];
	return p == nullptr || !p->m_bRed;
}
