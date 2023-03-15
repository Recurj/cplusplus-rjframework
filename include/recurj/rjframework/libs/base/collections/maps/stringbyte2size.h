#pragma once
#include <recurj/rjframework/libs/base/strings/unicode/allocated.h>
#include <recurj/rjframework/libs/base/collections/btrees/treestringbyte.h>
namespace rjBase {
	class RJBASE_API CMapStringByte2Size:public rjBase::BTree::CTreeStringByte {
	public:
		CMapStringByte2Size();
		virtual~CMapStringByte2Size();
		class RJBASE_API CElem :public rjBase::BTree::CRJRBTreeLeafStringByte {
			friend class CMapStringByte2Size;
		public:
			CElem(const CRJStringByte&, size_t);
			virtual~CElem();
			const CRJStringByte& Key() const;
			size_t GetValue();
			void SetValue(size_t);
			void*operator new(size_t);
			void operator delete(void*);
			static CRJAllocatorRuntime m_clHeap;
		protected:
			size_t m_clValue;
		};
		bool Remove(const CRJStringByte&);
		bool Exist(const CRJStringByte&);
		bool Map(const CRJStringByte&, size_t);
		bool Lookup(const CRJStringByte&, size_t&);
	};
}

