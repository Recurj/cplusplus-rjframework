#include "stdafx.h"
#include <recurj/rjframework/libs/base/storage/list.h>
#include <recurj/rjframework/libs/base/storage/leaf.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
using namespace rjBase;

CRJDataStorage::CRJDataStorage():m_sVolume(0),m_pLast(nullptr),m_sLast((size_t)-1)
{}
CRJDataStorage::CRJDataStorage(size_t lAlloc):m_sVolume(0), m_pLast(nullptr), m_sLast((size_t)-1) {
	Alloc(lAlloc);
}
CRJDataStorage::~CRJDataStorage() 
{
	Clear();
}

void CRJDataStorage::Clear() {
	m_clTree.Clear();
	Reset();
	m_sVolume = 0;
}
bool CRJDataStorage::Alloc(size_t sCount) {
	if (sCount>0) {
		CRJDataStoreLeaf* pNode;
		size_t c=sCount/ CRJSizeDatastoreRegion;
		if ((sCount% CRJSizeDatastoreRegion)>0) c++;
		while (c>0) {
			pNode=new CRJDataStoreLeaf();
			if (pNode == nullptr) return false;
			if (!m_clTree.Add(pNode)) {
				delete pNode;
				return false;
			}
			Bot(*pNode);
			m_sVolume+=CRJSizeDatastoreRegion;
			c--;
		}
		return true;
	}
	return false;
}
bool CRJDataStorage::Get(size_t sIndex, Something* p) const {
	if (sIndex < m_sVolume) {
		CRJDataStoreLeaf* pNode = find(RJ_DATASTORE_TREE_KEY(sIndex));
		if (pNode) {
			*p = (*pNode)[RJ_DATASTORE_TREE_INDEX(sIndex)];
			return true;
		}
	}
	return false;
}
bool CRJDataStorage::Set(size_t sIndex, Something p) {
	if (sIndex < m_sVolume) {
		CRJDataStoreLeaf* pNode = find(RJ_DATASTORE_TREE_KEY(sIndex));
		return (pNode) ? pNode->Set(RJ_DATASTORE_TREE_INDEX(sIndex), p) : false;
	}
	return false;
}
void CRJDataStorage::MoveRight(size_t sIndex, size_t sCount, Something pInit) {
#ifdef _DEBUG
	if (!pInit) CRJDataTools::DebugBreak();
#endif 
	if (!sCount) Set(sIndex, pInit);
	else {
		size_t i=RJ_DATASTORE_TREE_INDEX(sIndex);
		size_t c=CRJSizeDatastoreRegion-i-1;
		CRJDataStoreLeaf* pNode= find(RJ_DATASTORE_TREE_KEY(sIndex));
		while (pNode) {
			if (c >= sCount) return pNode->ShiftRight(i, sCount, pInit);
			pInit = pNode->ShiftRightFromLast(c, pInit);
			sCount -= (c + 1);
			pNode = pNode->GetNext();
			if (sCount==0) {
				if (pNode) pNode->Set(0, pInit);
				return ;
			}
#ifdef _DEBUG
			if (!pInit) CRJDataTools::DebugBreak();
#endif 
			i=0;
			c=CRJSizeDatastoreRegion-1;
		}
	}
}
void CRJDataStorage::MoveLeft(size_t lRemoved, size_t sCount) {
	if (!sCount) Set(lRemoved,nullptr);
	else {
		Something pInit=nullptr;
		size_t sIndex =lRemoved+sCount;
		size_t i=RJ_DATASTORE_TREE_INDEX(sIndex);
		CRJDataStoreLeaf* pNode = find(RJ_DATASTORE_TREE_KEY(sIndex));
		while (pNode) {
			if (i>=sCount) {
				pNode->ShiftLeft(i, sCount, pInit);
				return ;
			}
			pInit=pNode->ShiftLeftFromFirst(i, pInit);
			sCount-=(i+1);
#ifdef _DEBUG
			if (!pInit) CRJDataTools::DebugBreak();
#endif 
			pNode = pNode->GetPrev();
			if (sCount<=0) {
				if (pNode) pNode->Set(CRJSizeDatastoreRegion-1, pInit);
				return ;
			}
			i=CRJSizeDatastoreRegion-1;
		}
	}
}
CRJDataStoreLeaf* CRJDataStorage::find(size_t s) const {
	if (m_pLast == nullptr || m_sLast != s) {
		m_pLast = (CRJDataStoreLeaf*)m_clTree.GetRoot().Find(s);
		m_sLast = s;
	}
	return m_pLast;
}



