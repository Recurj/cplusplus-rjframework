#pragma once
#include <recurj/rjframework/modules/defs/moddefbase.h>
namespace rjBase::BTree {
	constexpr size_t cRJTreeIndexCatalog = 32;
	constexpr size_t cRJTreeIndexShift = 5;
	class RJBASE_API CRJTreeIndex {
	public:
		enum class AddResult { Error, OK, NeedNode };
		class CNode;
		class CNodeCatalog;
		CRJTreeIndex();
		virtual ~CRJTreeIndex();
		void Clear();
		bool Add(Something);
		inline const CNode& GetRoot() const {return *m_pRoot;}
		class CNode {
		public:
			CNode();
			virtual ~CNode();
			virtual Something Find(size_t) const  = 0;
			virtual AddResult Add(CRJTreeIndex&, Something) = 0;
		};
		class CNodeEmpty :public CNode {
		public:
			CNodeEmpty();
			virtual~CNodeEmpty();
			Something Find(size_t) const override;
			AddResult Add(CRJTreeIndex&, Something) override;
		};
		class CNodeCatalogLeaves :public CNode {
			friend class CNodeLeaf;
		public:
			CNodeCatalogLeaves(Something);
			virtual~CNodeCatalogLeaves();
			Something Find(size_t) const override;
			AddResult Add(CRJTreeIndex&, Something) override;
			void* operator new(size_t);
			void operator delete(void*);
		protected:
			void cleanup();
			AddResult createRoot(CRJTreeIndex&, Something);
		private:
			AddResult placeAvailable(Something);
		protected:
			static CRJAllocatorRuntime m_clHeap;
			Something m_pNodes[cRJTreeIndexCatalog];
			size_t m_sCount;
		};
		class CNodeCatalog :public CNode {
			friend class CNodeLeaf;
		public:
			CNodeCatalog(CNode*, CNode*, size_t);
			CNodeCatalog(CNode&, size_t);
			virtual~CNodeCatalog();
			Something Find(size_t) const override;
			AddResult Add(CRJTreeIndex&, Something) override;
			void* operator new(size_t);
			void operator delete(void*);
		protected:
			void cleanup();
			AddResult createRoot(CRJTreeIndex&, Something);
		protected:
			static CRJAllocatorRuntime m_clHeap;
			CNode* m_pNodes[cRJTreeIndexCatalog];
			size_t m_sCount, m_sLevel;
		};
	protected:
		void setRoot(CNode&);
		bool isItRoot(CNode*) const;
		CNode* createBranch(Something, size_t);
	private:
		static CNodeEmpty s_Empty;
		CNode* m_pRoot;
	};
}

