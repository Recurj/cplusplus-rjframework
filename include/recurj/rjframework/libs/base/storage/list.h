#pragma once
#include <recurj/rjframework/libs/base/storage/leaf.h>
namespace rjBase {
	class RJBASE_API CRJDataStorage : public CRJListAllocated <CRJDataStoreLeaf>{
	public:
		friend class CRJDataStoreNodeEmpty;
		CRJDataStorage();
		CRJDataStorage(size_t);
		virtual~CRJDataStorage();
		void Clear();
		bool Alloc(size_t);
		bool Get(size_t, Something*) const;
		bool Set(size_t, Something);
		void MoveLeft(size_t, size_t);
		void MoveRight(size_t, size_t, Something);
		inline size_t GetVolume() const {return m_sVolume;}
	protected:
		CRJDataStoreLeaf* find(size_t) const;
	protected:
		BTree::CRJTreeIndex m_clTree;
		mutable CRJDataStoreLeaf* m_pLast;
		mutable size_t m_sLast;
		size_t m_sVolume;
	};
}

