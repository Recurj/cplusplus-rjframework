#pragma once
#include <recurj/rjframework/libs/base/collections/btrees/rbtree.h>
#include <recurj/rjframework/libs/base/collections/btrees/rbtreeleafstringu.h>
namespace rjBase::BTree {
	class RJBASE_API CTreeStringUnicode : public CRJRBTree {
	public:
		struct SCache {
			inline operator CRJRBTreeLeafStringUnicode* () const { return m_pLastNode; }
			inline void Reset() const { m_pLastNode = nullptr; }
			inline void Set(CRJRBTreeLeafStringUnicode& leaf) const { m_pLastNode = &leaf; }
			inline bool Has(const CRJStringUnicode& key) const { return m_pLastNode != nullptr && m_pLastNode->m_clKey == key; }
			mutable CRJRBTreeLeafStringUnicode* m_pLastNode = nullptr;
		};
	public:
		CTreeStringUnicode();
		virtual~CTreeStringUnicode();
		void Clear() override;
		CRJRBTreeLeafStringUnicode* Lookup(const CRJStringUnicode&) const;
		bool Insert(CRJRBTreeLeafStringUnicode&);
		bool Delete(const CRJStringUnicode&);
	protected:
		bool onInsertFound(CRJRBTreeLeaf&, CRJRBTreeLeaf&) override;
	protected:
		SCache m_sCache;
	};
}

