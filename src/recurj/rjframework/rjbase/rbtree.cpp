#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections/btrees/rbtree.h>

using namespace rjBase::BTree;

CRJRBTreeLeaf::CRJRBTreeLeaf()
{}
CRJRBTreeLeaf::~CRJRBTreeLeaf()
{}
CRJRBTreeLeaf::funcFree CRJRBTreeLeaf::GetFuncFree()
{
	return &CRJRBTreeLeaf::FreeLeafDefault;
}
void CRJRBTreeLeaf::FreeLeafDefault(CRJRBTreeLeaf& node)
{
	delete& node;
}
void CRJRBTreeLeaf::FreeLeaf(CRJRBTreeLeaf& leaf) {
	funcFree func = leaf.GetFuncFree();
	if (func != nullptr) func(leaf);
}


CRJRBTree::CRJRBTree() : m_sCount(0), m_pRoot(nullptr)
{}
CRJRBTree::~CRJRBTree()
{
	Clear();
}
void CRJRBTree::Clear()
{}
bool CRJRBTree::ForeachWhileOk(IPredicate& cb)
{
	return foreachWhileOk(m_pRoot, cb);
}
void CRJRBTree::Foreach(IAction& cb){
	return foreach(m_pRoot, cb);
}

//bool CRJRBTree::Validate(CRJStringByteVar& s)
//{
//	return validate(s, m_pRoot) > 0;
//}
bool CRJRBTree::foreachWhileOk(CNode* p, IPredicate& cb)
{
	if (p != nullptr) {
		return foreachWhileOk(p->m_pChilds[NodeLeft], cb) && cb(p->m_clLeaf) &&
			foreachWhileOk(p->m_pChilds[NodeRight], cb);
	}
	return true;
}
void CRJRBTree::foreach(CNode* p, IAction& cb)
{
	if (p != nullptr) {
		foreach(p->m_pChilds[NodeLeft], cb);
		cb(p->m_clLeaf);
		foreach(p->m_pChilds[NodeRight], cb);
	}
}

bool CRJRBTree::onInsertFound(CRJRBTreeLeaf&, CRJRBTreeLeaf&)
{
	return false;
}

CRJRBTreeLeaf* CRJRBTree::find(CRJRBTreeLeaf& leaf) const
{
	int64_t c;
	CNode* p = m_pRoot;
	while (p != nullptr) {
		c = leaf.Compare(p->m_clLeaf);
		if (c == 0) return &p->m_clLeaf;
		p = p->m_pChilds[(c + 1) >> 1];
	}
	return nullptr;
}
bool CRJRBTree::red(CNode* p)
{
	return p != nullptr && p->m_bRed;
}
/*
Одиночный поворот в красно-чёрном дереве просто поворачивает узлы, как обычно,
затем "окрашивает" старую вершину в красный цвет, а новую - в чёрный.
Двойной поворот это просто два одиночных поворота.
*/
// LeftRotate	-> NodeRight,NodeLeft
// RightRotate	-> NodeLeft,NodeRight
CRJRBTree::CNode* CRJRBTree::rotate(CNode* x, size_t a, size_t b) {
	CNode* y = x->m_pChilds[a];
	x->m_pChilds[a] = y->m_pChilds[b];
	if (y->m_pChilds[b] != nullptr) y->m_pChilds[b]->m_pParent = x;
	y->m_pChilds[b] = x;
	y->m_pParent = x->m_pParent;
	if (x->m_pParent == nullptr) m_pRoot = y;
	else if (x->m_pParent->m_pChilds[NodeLeft] == x) x->m_pParent->m_pChilds[NodeLeft] = y;
	else x->m_pParent->m_pChilds[NodeRight] = y;
	x->m_pParent = y;
	return y;
}
CRJRBTree::CNode *CRJRBTree::swap(CNode *orig, CNode *moved,size_t path)
{
	CNode* p = orig->m_pParent;
	if (p == nullptr) m_pRoot = moved;
	else p->m_pChilds[path] = moved;
	moved->m_pParent = p;
	return moved;
}
void CRJRBTree::swapChild(CNode* orig, CNode* moved, size_t path)
{
	CNode* p = orig->m_pChilds[path];
	moved->m_pChilds[path] = p;
	p->m_pParent = moved;
}
#ifdef _DEBUG
int64_t CRJRBTree::validate(CRJStringByteVar& s, CNode* p) {
	if (p == nullptr) return 1;
	CNode* ln = p->m_pChilds[0];
	CNode* rn = p->m_pChilds[1];
	int64_t lh = 1, rh = 1;
	if (ln == nullptr) {
		if (rn == nullptr);
		else {
			if (!rn->m_bRed) return invalid(s, "Single node violation");
			if (rn->m_clLeaf.Compare(p->m_clLeaf) <= 0) return invalid(s, "Binary tree violation");
			else  rh = validate(s, rn);
		}
	}
	else if (rn == nullptr) {
		if (!ln->m_bRed) return invalid(s, "Single node violation");
		if (ln->m_clLeaf.Compare(p->m_clLeaf) >= 0) return invalid(s, "Binary tree violation");
		else lh = validate(s, ln);
	}
	else {
		if (ln->m_clLeaf.Compare(p->m_clLeaf) >= 0 ||
			rn->m_clLeaf.Compare(p->m_clLeaf) <= 0) return invalid(s,"Binary tree violation");
		lh = validate(s, ln);
		rh = validate(s, rn);
	}
	if (lh != 0 && rh != 0) {
		if (lh != rh) return invalid(s, "Black violation");
		if (red(p)) {
			if (red(ln) || red(rn)) return invalid(s, "Red violation");
			return lh;
		}
		return lh + 1;
	}
	return 0;
}
bool CRJRBTree::validate() {
	CRJStringByteExpanded s;
	int64_t h = validate(s, m_pRoot);
	if (h <= 0) {
		//CRJDataTools::DebugBreak();
		return false;
	}
	return true;
}
int64_t CRJRBTree::invalid(CRJStringByteVar& s, const CRJStringByte& err)
{
	s.Cat(err).CatCRLF();
	return 0;
}
#endif