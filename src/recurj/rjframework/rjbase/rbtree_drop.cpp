#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections/btrees/rbtree.h>
using namespace rjBase::BTree;
/*
	Main point is
	The single child node has properties:
	a) Child node is RED (otherwise violation Rule 2)
	b) Child node is single:
		if its child black - violation Rule 2
		if its chile red - violation Rule 1
	b) Parent node is BLACK (otherwise violation Rule 1)
*/
CRJRBTree::dropfunc CRJRBTree::s_dropTable[4] =
{
	&CRJRBTree::dropTerminal,&CRJRBTree::dropSingleChild,
	&CRJRBTree::dropSingleChild,&CRJRBTree::dropChilds
};
bool CRJRBTree::drop(CRJRBTreeLeaf&leaf) {
	int64_t c;
	size_t d = 0;
	CNode* p = m_pRoot;
	while (p != nullptr) {
		c = leaf.Compare(p->m_clLeaf);
		if (c == 0) {
			size_t m = p->childMasks();
			return (this->*CRJRBTree::s_dropTable[m])(p, d, m >> 1);
		}
		d = (c + 1) >> 1;
		p = p->m_pChilds[d];
	}
	return true;
}
bool CRJRBTree::freeDeleted(CNode* p)
{
	p->m_pChilds[NodeLeft] = nullptr;
	p->m_pChilds[NodeRight] = nullptr;
	delete p;
	m_sCount--;
#ifdef _DEBUG
	if (!validate()) return false;
#endif
	return true;
}
/*
*  elem - removing terminal node
*  pathParent = place inside parent
*/
bool CRJRBTree::dropTerminal(CNode* elem, size_t pathParent, size_t) {
	CNode* q = elem->m_pParent;
	if (q == nullptr) {
		m_pRoot = nullptr;
		return freeDeleted(elem);
	}
	else q->m_pChilds[pathParent] = nullptr;
	if (!elem->m_bRed) dropRebalance(q, pathParent);
	return freeDeleted(elem);
}
/*
*  elem - removing terminal node
*  place = place inside parent
*  Note - See main point
*/
bool CRJRBTree::dropSingleChild(CNode* elem, size_t pathParent, size_t pathChild)
{
	CNode* x = swap(elem, elem->m_pChilds[pathChild], pathParent);
	x->m_bRed = elem->m_bRed;
	return freeDeleted(elem);
}
bool CRJRBTree::dropChilds(CNode* elem, size_t pathParent, size_t)
{
	size_t pathSibling = (pathParent + 1) & 1;
	CNode* x = elem->sidemost(pathParent, pathSibling);
	CNode* q = x->m_pChilds[pathSibling];
	swapChild(elem, x, pathParent);
	if (q == nullptr) { // x is terminal
		bool red = x->m_bRed;
		x->m_bRed = elem->m_bRed;
		q = x->m_pParent;
		if (q == elem) q = x; // new parent 
		else {
			x->m_pParent->m_pChilds[pathParent] = nullptr; // remove x from parent
			swapChild(elem, x, pathSibling);
			pathSibling = pathParent; // new path
		}
		swap(elem, x, pathParent);
		if (!red) dropRebalance(q, pathSibling);
	}
	else {// See main point - single child again
		if (x->m_pParent != elem) {
			q->m_pParent = x->m_pParent;
			x->m_pParent->m_pChilds[pathParent] = q;
			swapChild(elem, x, pathSibling);
		}
		x->m_bRed = elem->m_bRed;
		q->m_bRed = false;
		swap(elem, x, pathParent);
	}
	return freeDeleted(elem);
}
/*
*  parent - parent of removed terminal node
*  pathElement = place of termainl node inside parent
*/
void CRJRBTree::dropRebalance(CNode* parent, size_t pathElement) {
	CNode* b;
	size_t pathSibling;
loop:
	pathSibling = (pathElement + 1) & 1;
	b = parent->m_pChilds[pathSibling];
	if (b->m_bRed) {
		b->m_bRed = false;
		parent->m_bRed = true;
		rotate(parent, pathSibling, pathElement); // b is not red now
		b = parent->m_pChilds[pathSibling];
	}
	if (b->childIsBlack(pathSibling)) {
		if (b->childIsBlack(pathElement)) {
			b->m_bRed = true;
			if (parent->m_bRed) {
				parent->m_bRed = false;
				return;
			}
			goto up;
		}
		else {
			b->m_pChilds[pathElement]->m_bRed = false;
			b->m_bRed = true;
			b = rotate(b, pathElement, pathSibling);
		}
	}
	b->m_bRed = parent->m_bRed;
	parent->m_bRed = false;
	b->m_pChilds[pathSibling]->m_bRed = false;
	rotate(parent, pathSibling, pathElement);
	return;
up:
	b = parent;
	parent = parent->m_pParent;
	if (parent == nullptr) return;
	pathElement = parent->getDir(b);
	goto loop;
}
