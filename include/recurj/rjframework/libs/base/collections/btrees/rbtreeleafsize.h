#pragma once
#include <recurj/rjframework/libs/base/collections/btrees/rbtree.h>
namespace rjBase::BTree {
	class RJBASE_API CRJRBTreeLeafSize : public CRJRBTreeLeaf {
	public:
		CRJRBTreeLeafSize(size_t);
		virtual~CRJRBTreeLeafSize();
		int64_t Compare(const CRJRBTreeLeaf&) const override;
		size_t m_clKey;
	};
}

