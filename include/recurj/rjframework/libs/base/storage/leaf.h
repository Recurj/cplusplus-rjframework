#pragma once
#include <recurj/rjframework/libs/base/storage/def.h>
namespace rjBase {
	class CRJDataStoreLeaf : public CRJListItem<CRJDataStoreLeaf>{
		friend class CRJDataStorage;
	public:
		virtual~CRJDataStoreLeaf();
		Something operator[](size_t) const;
		bool Set(size_t, Something);
		void ShiftRight(size_t, size_t, Something);
		void ShiftLeft(size_t, size_t, Something);
		Something ShiftRightFromLast(size_t, Something);
		Something ShiftLeftFromFirst(size_t, Something);
		void*operator new(size_t);
		void operator delete(void*);
		static Something* ShiftRight(Something*, size_t);
		static Something* ShiftLeft(Something*, size_t);
	protected:
		CRJDataStoreLeaf();
		static CRJAllocatorRuntime m_clHeap;
		Something m_stTable[CRJSizeDatastoreRegion];
	};
}

