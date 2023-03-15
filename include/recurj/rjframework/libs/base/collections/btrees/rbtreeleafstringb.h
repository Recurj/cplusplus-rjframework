#pragma once
#include <recurj/rjframework/libs/base/collections/btrees/rbtree.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
namespace rjBase::BTree {
	class RJBASE_API CRJRBTreeLeafStringByte : public CRJRBTreeLeaf {
	public:
		CRJRBTreeLeafStringByte(const CRJStringByte &);
		virtual ~CRJRBTreeLeafStringByte();
		int64_t Compare(const CRJRBTreeLeaf&) const override;
		CRJStringByteAllocatedRef m_clKey;
	protected:
		CRJRBTreeLeafStringByte();
	};
	class RJBASE_API CRJRBTreeCompareStringByteRef : public CRJRBTreeLeafStringByte {
	public:
		CRJRBTreeCompareStringByteRef(const CRJStringByte&);
		virtual ~CRJRBTreeCompareStringByteRef();
	};
}

