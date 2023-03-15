#include "stdafx.h"
#include <recurj/rjframework/libs/base/collections/btrees/rbtreeleafsize.h>
#include <recurj/rjframework/libs/base/collections/btrees/rbtreeleafint64.h>
#include <recurj/rjframework/libs/base/collections/btrees/rbtreeleafstringb.h>
#include <recurj/rjframework/libs/base/collections/btrees/rbtreeleafstringu.h>
using namespace rjBase::BTree;
CRJRBTreeLeafSize::CRJRBTreeLeafSize(size_t key) :m_clKey(key)
{}
CRJRBTreeLeafSize::~CRJRBTreeLeafSize()
{}
int64_t CRJRBTreeLeafSize::Compare(const CRJRBTreeLeaf& leaf) const {
	return rjcompare(m_clKey, ((CRJRBTreeLeafSize&)leaf).m_clKey);
}
CRJRBTreeLeafInt64::CRJRBTreeLeafInt64(int64_t key) :m_clKey(key)
{}
CRJRBTreeLeafInt64::~CRJRBTreeLeafInt64()
{}
int64_t CRJRBTreeLeafInt64::Compare(const CRJRBTreeLeaf& leaf) const {
	return rjcompare(m_clKey, ((CRJRBTreeLeafInt64&)leaf).m_clKey);
}
CRJRBTreeLeafStringByte::CRJRBTreeLeafStringByte(const CRJStringByte& s) :
	m_clKey(s)
{}
CRJRBTreeLeafStringByte::CRJRBTreeLeafStringByte()
{}
CRJRBTreeLeafStringByte::~CRJRBTreeLeafStringByte()
{}
int64_t CRJRBTreeLeafStringByte::Compare(const CRJRBTreeLeaf& leaf) const {
	return m_clKey.Compare(((CRJRBTreeLeafStringByte&)leaf).m_clKey);
}
CRJRBTreeCompareStringByteRef::CRJRBTreeCompareStringByteRef(const CRJStringByte& s)
{
	m_clKey.SetReference(s);
}
CRJRBTreeCompareStringByteRef::~CRJRBTreeCompareStringByteRef()
{
}
CRJRBTreeLeafStringUnicode::CRJRBTreeLeafStringUnicode(const CRJStringUnicode& s) :
	m_clKey(s)
{}
CRJRBTreeLeafStringUnicode::CRJRBTreeLeafStringUnicode()
{}
CRJRBTreeLeafStringUnicode::~CRJRBTreeLeafStringUnicode()
{}
int64_t CRJRBTreeLeafStringUnicode::Compare(const CRJRBTreeLeaf& leaf) const {
	return m_clKey.Compare(((CRJRBTreeLeafStringUnicode&)leaf).m_clKey);
}
CRJRBTreeCompareStringUnicodeRef::CRJRBTreeCompareStringUnicodeRef(const CRJStringUnicode& s) 
{
	m_clKey.SetReference(s);
}
CRJRBTreeCompareStringUnicodeRef::~CRJRBTreeCompareStringUnicodeRef()
{}
