#pragma once
#include <recurj/rjframework/libs/base/collections/btrees/rbtree.h>
#include <recurj/rjframework/libs/base/collections/btrees/rbtreeleafint64.h>
namespace rjBase::BTree {
	class RJBASE_API CTreeInt64 : public CRJRBTree {
	public:
		struct SCache {
			inline operator CRJRBTreeLeafInt64* () const { return m_pLastNode; }
			inline void Reset() const { m_pLastNode = nullptr; }
			inline void Set(CRJRBTreeLeafInt64 &leaf) const { m_pLastNode = &leaf; }
			inline bool Has(int64_t key) const { return m_pLastNode != nullptr && m_pLastNode->m_clKey == key; }
			mutable CRJRBTreeLeafInt64* m_pLastNode = nullptr;
		};
		CTreeInt64();
		virtual~CTreeInt64();
		void Clear() override;
		CRJRBTreeLeafInt64* Lookup(int64_t) const;
		bool Insert(CRJRBTreeLeafInt64&);
		bool Delete(int64_t);
	protected:
		bool onInsertFound(CRJRBTreeLeaf&, CRJRBTreeLeaf&) override;
	protected:
		SCache m_sCache;
	};
}

