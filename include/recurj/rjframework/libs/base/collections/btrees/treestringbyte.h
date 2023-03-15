#pragma once
#include <recurj/rjframework/libs/base/collections/btrees/rbtree.h>
#include <recurj/rjframework/libs/base/collections/btrees/rbtreeleafstringb.h>
namespace rjBase::BTree {
	class RJBASE_API CTreeStringByte : public CRJRBTree {
	public:
		struct SCache {
			inline operator CRJRBTreeLeafStringByte*() const {return m_pLastNode;}
			inline void Reset() const {m_pLastNode=nullptr;}
			inline void Set(CRJRBTreeLeafStringByte &leaf) const {m_pLastNode=&leaf;}
			inline bool Has(const CRJStringByte& key) const {return m_pLastNode!=nullptr&& m_pLastNode->m_clKey == key;}
			mutable CRJRBTreeLeafStringByte* m_pLastNode=nullptr;
		};
	public:
		CTreeStringByte();
		virtual~CTreeStringByte();
		void Clear() override;
		CRJRBTreeLeafStringByte* Lookup(const CRJStringByte&) const;
		bool Insert(CRJRBTreeLeafStringByte&);
		bool Delete(const CRJStringByte&);
	protected:
		bool onInsertFound(CRJRBTreeLeaf&, CRJRBTreeLeaf&) override;
	protected:
		SCache m_sCache;
	};
}

