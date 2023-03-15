#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections/btrees/rbtree.h>
using namespace rjBase::BTree;
bool CRJRBTree::insert(CRJRBTreeLeaf& leaf) {
	int64_t c;
	CNode* p = m_pRoot;
	SNodeInsert context;
	while (p != nullptr) {
		c = leaf.Compare(p->m_clLeaf);
		if (c == 0) return onInsertFound(leaf, p->m_clLeaf);
		context.m_sGrandPlace = context.m_sParentPlace;
		context.m_pGrandParent = context.m_pParent;
		context.m_sParentPlace = (c + 1) >> 1;
		context.m_pParent = p;
		p = p->m_pChilds[context.m_sParentPlace];
	}
	p = new CNode(leaf, context.m_pParent, true);
	if (p != nullptr) {
		context.insertRebalance(*this, p);
		m_pRoot->m_bRed = false;
		m_sCount++;
#ifdef _DEBUG
		if (!validate()) return false;
#endif
		return true;
	}
	return false;
}
CRJRBTree::SNodeInsert::SNodeInsert():
	m_sParentPlace(NodeLeft), m_sGrandPlace(NodeLeft),m_pParent(nullptr)
{}
void CRJRBTree::SNodeInsert::insertRebalance(CRJRBTree&tree, CNode*pElem)
{
	if (m_pParent == nullptr) tree.m_pRoot = pElem;
	else {
		CNode* p;
		size_t gbdir;
		m_pParent->m_pChilds[m_sParentPlace] = pElem;
		while (m_pParent->m_bRed) { // and grandparent exist
			gbdir = (m_sGrandPlace + 1) & 1; // where is uncle
			p = m_pGrandParent->m_pChilds[gbdir]; // here is uncle
			if (p != nullptr  && p->m_bRed) {//  uncle is read, go to next level
				m_pGrandParent->m_bRed = true;
				m_pParent->m_bRed = false;
				p->m_bRed = false;
				pElem = m_pGrandParent;
				m_pParent = pElem->m_pParent;
				if (m_pParent == nullptr) return; // Elem is root and will be black
				m_pGrandParent = m_pParent->m_pParent;
				if (m_pGrandParent == nullptr) return; // New parent is root and it is black
				m_sParentPlace = m_pParent->getDir(pElem);
				m_sGrandPlace = m_pGrandParent->getDir(m_pParent);
			}
			else {
				if (m_sParentPlace != m_sGrandPlace) { // lite rotate, we know parent of rotated m_pParent
					p = pElem->m_pChilds[m_sGrandPlace];
					m_pParent->m_pChilds[m_sParentPlace] = p;
					if (p != nullptr) p->m_pParent = m_pParent;
					pElem->m_pChilds[m_sGrandPlace] = m_pParent;
					pElem->m_pParent = m_pGrandParent;
					m_pGrandParent->m_pChilds[m_sGrandPlace] = pElem;
					m_pParent->m_pParent = pElem;
					m_pParent=pElem;
				}
				m_pParent->m_bRed = false;
				m_pGrandParent->m_bRed = true;
				tree.rotate(m_pGrandParent, m_sGrandPlace, gbdir);
				return;
			}
		}
	}
}

