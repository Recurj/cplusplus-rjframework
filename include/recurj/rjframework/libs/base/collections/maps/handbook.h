#pragma once
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/collections/btrees/treeint64.h>
namespace rjBase {
	class RJBASE_API CMapHandbook:public BTree::CTreeInt64 {
	public:
		CMapHandbook();
		virtual~CMapHandbook();
		class RJBASE_API CElem:public BTree::CRJRBTreeLeafInt64 {
			friend class CMapHandbook;
		public:
			CElem(int64_t,const CRJStringByte&);
			virtual~CElem();
			const CRJStringByte& GetValue();
			void*operator new(size_t);
			void operator delete(void*);
			static CRJAllocatorRuntime m_clHeap;
		protected:
			CRJStringByteAllocated m_clValue;
		};
		bool Remove(int64_t);
		bool Exist(int64_t);
		bool Map(int64_t,const CRJStringByte&);
		bool Lookup(int64_t, CRJStringByteFlex&);
		CElem* Node(int64_t);
	};
}

