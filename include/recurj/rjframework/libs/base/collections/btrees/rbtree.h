/*
* Red-Black tree implementation
* Basic rule
* 1. No red node has a red child.
* 2. Every simple path from a given node to one of its non-branching node descendants
* contains the same number of black nodes.
*/
#pragma once
#include <recurj/rjframework/functors.h>
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
namespace rjBase::BTree {
	constexpr size_t NodeLeft = 0;
	constexpr size_t NodeRight = 1;
	constexpr size_t MaskLeft = 1;
	constexpr size_t MaskRight = 2;
	class CRJRBTree;
	class RJBASE_API CRJRBTreeLeaf {
	public:
		friend class CRJRBTree;
		CRJRBTreeLeaf();
		virtual ~CRJRBTreeLeaf();
		virtual int64_t Compare(const CRJRBTreeLeaf&) const = 0;
		using funcFree = void (*)(CRJRBTreeLeaf&);
		virtual funcFree GetFuncFree();
		static void FreeLeafDefault(CRJRBTreeLeaf&);
		static void FreeLeaf(CRJRBTreeLeaf&);
	};
	class RJBASE_API CRJRBTree {
	public:
		CRJRBTree();
		virtual ~CRJRBTree();
		virtual void Clear();
		//		bool Validate(CRJStringByteVar&);
		inline size_t Count() const { return m_sCount; }
		using IPredicate = IRJFunctorBool<CRJRBTreeLeaf&>;
		using IAction = IRJFunctor<CRJRBTreeLeaf&>;
		void Foreach(IAction&);
		bool ForeachWhileOk(IPredicate&);
		template<typename TFunc>
		using CAction = CRJFunctor<IAction, TFunc, CRJRBTreeLeaf&>;
		template<typename TFunc>
		using CPredicate = CRJFunctorBool<IPredicate, TFunc, CRJRBTreeLeaf&>;
	protected:
		class CNode {
		public:
			CNode(CRJRBTreeLeaf&, CNode*, bool);
			virtual ~CNode();
			void* operator new(size_t);
			void operator delete(void*);
			inline size_t getDir(CNode* p) const { return m_pChilds[NodeLeft] == p ? NodeLeft : NodeRight; }
			CNode* sidemost(size_t, size_t);
			bool childIsBlack(size_t);
			size_t childMasks();
			bool m_bRed;
			CRJRBTreeLeaf& m_clLeaf;
			CNode* m_pParent;
			CNode* m_pChilds[2];
			static void clear(CNode*);
		protected:
			static CRJAllocatorRuntime m_clHeap;
		};
	protected:
		virtual bool onInsertFound(CRJRBTreeLeaf&, CRJRBTreeLeaf&);
		struct SNodeInsert {
			SNodeInsert();
			void insertRebalance(CRJRBTree&, CNode*);
			CNode* m_pParent, * m_pGrandParent;
			size_t m_sParentPlace, m_sGrandPlace;
		};
		using dropfunc = bool (CRJRBTree::*)(CNode*, size_t, size_t);

		CRJRBTreeLeaf* find(CRJRBTreeLeaf&) const;
		bool insert(CRJRBTreeLeaf&);
		bool drop(CRJRBTreeLeaf&);
		bool dropTerminal(CNode*, size_t, size_t);
		bool dropSingleChild(CNode*, size_t, size_t);
		bool dropChilds(CNode*, size_t, size_t);
		void dropRebalance(CNode*, size_t);
		bool freeDeleted(CNode*);
		bool foreachWhileOk(CNode*, IPredicate&);
		void foreach(CNode*, IAction&);
		CNode* swap(CNode*, CNode*, size_t);
		void swapChild(CNode*, CNode*, size_t);
		CNode* rotate(CNode*, size_t, size_t);
#ifdef _DEBUG
		bool validate();
		static int64_t validate(CRJStringByteVar&, CNode*);
		static int64_t invalid(CRJStringByteVar&, const CRJStringByte&);
#endif 
		static bool red(CNode*);
	protected:
		static dropfunc s_dropTable[4];
		CNode* m_pRoot;
		size_t m_sCount;
	};
}

