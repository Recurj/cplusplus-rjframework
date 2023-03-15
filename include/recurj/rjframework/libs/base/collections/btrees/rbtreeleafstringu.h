#pragma once
#include <recurj/rjframework/libs/base/collections/btrees/rbtree.h>
#include <recurj/rjframework/libs/base/strings/unicode/allocated.h>
namespace rjBase::BTree {
	class RJBASE_API CRJRBTreeLeafStringUnicode : public CRJRBTreeLeaf {
	public:
		CRJRBTreeLeafStringUnicode(const CRJStringUnicode&);
		virtual ~CRJRBTreeLeafStringUnicode();
		int64_t Compare(const CRJRBTreeLeaf&) const override;
		CRJStringUnicodeAllocatedRef m_clKey;
	protected:
		CRJRBTreeLeafStringUnicode();
	};
	class RJBASE_API CRJRBTreeCompareStringUnicodeRef : public CRJRBTreeLeafStringUnicode {
	public:
		CRJRBTreeCompareStringUnicodeRef(const CRJStringUnicode&);
		virtual ~CRJRBTreeCompareStringUnicodeRef();
	};
}

