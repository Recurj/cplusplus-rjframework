#pragma once
#include <recurj/rjframework/libs/base/collections//arrays/defs.h>
namespace rjBase {
	template<typename TElem,typename DataStore=CRJDataStorage>
	class CRJAPointers:public CRJArrayBase<TElem*, DataStore> {
	public:
		CRJAPointers(size_t sStep=CRJSizeDatastoreRegion, size_t sAlloc=0):
			CRJArrayBase<TElem*, DataStore>(sStep, sAlloc) {}
		virtual~CRJAPointers() {}
		bool Remove(size_t ind) {return this->IsValidIndex(ind)?RemoveChecked(ind):false;}
		bool RemoveChecked(size_t ind) {
			this->m_sCount--;
			this->m_clStorage.MoveLeft(ind, this->m_sCount-ind);
			return true;
		}
		bool Grow(size_t ind,const TElem& elem) {
			size_t need=ind+1;
			while (need<this->m_clStorage.GetVolume()) {
				if (!this->m_clStorage.Alloc(this->m_sAddStep)) return false;
			};
			return this->Set(ind,(TElem*)& elem);
		}
		bool Insert(size_t ind,const TElem& elem) {
			size_t last=this->m_sCount;
			if (this->checkVolume()&&
				this->IsValidIndex(ind)) {
				this->m_clStorage.MoveRight(ind, last-ind,(Something)& elem);
				return true;
			}
			return false;
		}
		bool Push(const TElem& elem) {
			size_t ind=this->m_sCount;
			if (!this->checkVolume()) return false;
			return this->m_clStorage.Set(ind,(Something)& elem);
		}
		bool Pop() {
			return (this->m_sCount > 0)? RemoveChecked(this->m_sCount - 1) :false;
		}
		bool Exist(const TElem& f) const {
			SRJArrayFind finder;
			return findValue(& f, finder);
		}
		bool Exist(const TElem& f, SRJArrayFind& finder) const {
			return findValue(& f, finder);
		}
	protected:
		bool findValue(const TElem* f, SRJArrayFind& finder) const {
			Something elem;
			size_t c=this->m_sCount;
			while (c>0) {
				c--;
				if (this->m_clStorage.Get(c,& elem)&&(TElem*) elem==f) 
					return finder.WasFound(c, elem);
			}
			return false;
		}
	};
	template<typename TElem,typename DataStore=CRJDataStorage>
	class CRJAPointersAllocated:public CRJAPointers<TElem, DataStore> {
	public:
		using CRJAPointers<TElem, DataStore>::CRJAPointers;
		void Clear() override {
			while (this->m_sCount > 0) {
				this->m_sCount--;
				drop(this->m_sCount);
			}
			CRJAPointers::Clear();
		}
	protected:
		void drop(size_t ind) {
			TElem* elem = (*this)[ind];
			if (elem) {
				try {
					FreeElem(elem);
				}
				catch (...) {}
			}
		}
		void FreeElem(TElem* p) {
			if (!p) return ;
			try {
				delete p;
			}
			catch(...) {}
			p=nullptr;
		}
	};
}

