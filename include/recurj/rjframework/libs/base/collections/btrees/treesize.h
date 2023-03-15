#pragma once
#include <recurj/rjframework/libs/base/collections/btrees/rbtree.h>
#include <recurj/rjframework/libs/base/collections/btrees/rbtreeleafsize.h>
namespace rjBase::BTree {

	class RJBASE_API CTreeSize: public CRJRBTree {
	public:
		struct SCache {
			inline operator CRJRBTreeLeafSize*() const {return m_pLastNode;}
			inline void Reset() const {m_pLastNode=nullptr;}
			inline void Set(CRJRBTreeLeafSize &leaf) const {m_pLastNode=&leaf;}
			inline bool Has(size_t key) const {return m_pLastNode != nullptr && m_pLastNode->m_clKey==key;}
			mutable CRJRBTreeLeafSize* m_pLastNode=nullptr;
		};
	public:
		CTreeSize();
		virtual~CTreeSize();
		void Clear() override;
		CRJRBTreeLeafSize* Lookup(size_t) const;
		bool Insert(CRJRBTreeLeafSize&);
		bool Delete(size_t);
	protected:
		bool onInsertFound(CRJRBTreeLeaf&, CRJRBTreeLeaf&) override;
	protected:
		SCache m_sCache;
	};
}

