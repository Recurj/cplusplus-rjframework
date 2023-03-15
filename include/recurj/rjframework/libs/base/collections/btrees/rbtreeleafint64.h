#pragma once
#include <recurj/rjframework/libs/base/collections/btrees/rbtree.h>
namespace rjBase::BTree {
	class RJBASE_API CRJRBTreeLeafInt64 : public CRJRBTreeLeaf {
	public:
		CRJRBTreeLeafInt64(int64_t);
		virtual~CRJRBTreeLeafInt64();
		int64_t Compare(const CRJRBTreeLeaf&) const override;
		int64_t m_clKey;
	};
}

