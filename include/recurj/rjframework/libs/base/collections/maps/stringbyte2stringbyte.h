#pragma once
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/collections/btrees/treestringbyte.h>
namespace rjBase {
	class RJBASE_API CMapStringByte2StringByte:public BTree::CTreeStringByte {
	public:
		CMapStringByte2StringByte();
		virtual~CMapStringByte2StringByte();
		class RJBASE_API CElem:public BTree::CRJRBTreeLeafStringByte {
			friend class CMapStringByte2StringByte;
		public:
			CElem(const CRJStringByte&,const CRJStringByte&);
			virtual~CElem();
			const CRJStringByte& Key() const;
			const CRJStringByte& GetValue();
			void*operator new(size_t);
			void operator delete(void*);
			static CRJAllocatorRuntime m_clHeap;
		protected:
			CRJStringByteAllocated m_clValue;
		};
		bool Remove(const CRJStringByte&);
		bool Exist(const CRJStringByte&);
		bool Map(const CRJStringByte&,const CRJStringByte&);
		bool Lookup(const CRJStringByte&, CRJStringByteFlex&);
		CElem* Node(const CRJStringByte&);
	};
}

