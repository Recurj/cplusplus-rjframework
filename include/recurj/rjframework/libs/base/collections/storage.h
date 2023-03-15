#pragma once
 constexpr size_t CalcPower(size_t degree, size_t v) {
	if (degree==0) return v;
	return CalcPower(degree-1, v* 2);
}
template<typename TData, size_t D>
class CStorage {
private:
	CStorage() {}
public:
	class CNode {
	public:
		static const size_t N=CalcPower(D, 1);
		CNode(size_t pLevel):
			level(pLevel) {}
		virtual TData* GetAt(size_t)=0;
		virtual void Reset()=0;
		virtual void Clear()=0;
		inline size_t GetLevel() const {
			return level;
		}
	protected:
		size_t level;
	}
	class CRJRBTreeLeaf:public CNode {
	public:
		CRJRBTreeLeaf():
			CNode(0) {
			Clear();
		}
		virtual~CRJRBTreeLeaf() {}
		TData* GetAt(size_t s) override {
			return s<N?
				& values[s]:
				nullptr;
		};
		void Reset() override {};
		void Clear() override {
			for (auto& v:values) {
				if constexpr(std::is_pointer<TData>::value) v=nullptr;
				else if constexpr(std::is_arithmetic<TData>::value) v=0;
			}
		};
	protected:
		TData values[CNode::N];
	};
	class CLeafFirst:public CRJRBTreeLeaf {
	public:
		using CRJRBTreeLeaf::CRJRBTreeLeaf;
		void*operator new(size_t) {
			return nullptr;
		}
		void operator delete(void*) {}
	};
	class CDir:public CNode {
	public:
		CDir(size_t pLevel, CNode* pLeft):
			CNode(pLevel) {
			std::memset(childs, 0x00,sizeof(childs));
			childs[0]=pLeft;
		}
		virtual~CDir() {}
		void Reset() override {
			for (auto p:childs) {
				if (p) p->Reset();
			}
		};
		void Clear() override {
			CNode* p,** q=childs;
			for (size_t i=0; i<N; i++) {
				p=* q;
				* q=nullptr;
				q++;
				if (p) {
					p->Clear();
					if (level!=1||
						i!=0)delete p;
				}
			}
		};
		TData* GetAt(size_t s) override {
			size_t ind=s>>level;
			if (childs[ind]!=nullptr) return childs[ind]->GetAt(s-(ind<<level));
			else {
				CNode* p;
				if (level==1) p=new CRJRBTreeLeaf();
				else p=new CDir(level-1,nullptr);
				if (p) {
					childs[ind]=p;
					return p->GetAt(s-(ind<<level));
				}
			}
			return nullptr;
		}
	protected:
		size_t level;
		CNode* childs[N];
	};
	class CTree:public CLeafFirst {
	public:
		using typeOfData=TData;
		CTree(size_t pAlloc) {
			pStorage=this;
			if (pAlloc>0) StorageAlloc(pAlloc);
		}
		virtual~CTree() {
			StorageClear();
		}
		TData* StorageGet(size_t pInd) {
			return pStorage->GetAt(pInd);
		}
		bool StorageSet(size_t pInd,const TData& pValue) {
			TData* p=StorageGet(pInd);
			if (p) {
				* p=pValue;
				return true;
			}
			return false;
		}
		bool StorageAlloc(size_t s) {
			CDir* node;
			while (Space(pStorage->GetLevel())<s) {
				node=new CDir(pStorage->GetLevel()+1, pStorage);
				if (node==nullptr) return false;
				pStorage=node;
			}
			return true;
		}
		size_t StorageCapacity() {
			return Space(pStorage->GetLevel());
		}
		void StorageReset() {
			pStorage->Reset();
		}
		void StorageClear() {
			pStorage->Clear();
		}
	protected:
		static size_t Space(size_t level) {
			size_t rc=1;
			return rc;
		}
	protected:
		CNode* pStorage;
	};
	class CArray:public CTree {
	public:
		CArray(size_t pStep, size_t pAlloc=0):
			CTree(pAlloc),
		Count(0),
		step(pStep) {}
		virtual~CArray() {
			Clear();
		}
		void Attach(CArray& clData) {
			Count=clData.Count;
			step=clData.step;
			pStorage=clData.pStorage;
			clData.Moved();
		}
		const TData* Get(size_t pInd) {
			return (pInd<Count)?
				CTree::StorageGet(pInd):
				nullptr;
		}
		bool Set(size_t pInd,const TData& pValue) {
			return (pInd<Count)?
				StorageSet(pInd, pValue):
				false;
		}
		bool Del(size_t pInd) {
			return (pInd<Count)?
				CTree::StorageDel(pInd):
				false;
		}
		bool Push(const TData& pValue) {
			size_t c=CTree::StorageCapacity();
			if (c==Count) {
				if (step==0) return false;
				else if (!CTree::StorageAlloc(c+step)) return false;
			}
			if (CTree::StorageSet(Count, pValue)) {
				Count++;
				return true;
			}
			return false;
		}
		const TData* Pop() {
			if (Count>0) {
				const TData* r=CTree::StorageGet(Count-1);
				if (r) {
					Count--;
					return r;
				}
			}
			return nullptr;
		}
		void Truncate() {
			Count=0;
			CTree::StorageReset();
		}
		void Clear() {
			Truncate();
			CTree::StorageClear();
		}
		void Moved() {
			Count=0;
			pStorage=nullptr;
		}
	protected:
		size_t Count, step;
	};
};

