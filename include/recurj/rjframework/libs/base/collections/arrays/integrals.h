#pragma once
#include <recurj/rjframework/libs/base/collections//arrays/defs.h>
namespace rjBase {
	template<typename TElem,typename DataStore=CRJDataStorage>
	class CRJIntegralsDeletable:public CRJArrayBase<TElem, DataStore> {
	public:
		CRJIntegralsDeletable(size_t sStep=CRJSizeDatastoreRegion, size_t sAlloc=0):
			CRJArrayBase<TElem, DataStore>(sStep, sAlloc) {
//			if constexpr(!std::is_integral<TElem>::value) static_assert(false,"Must be integral type");
		}
		virtual~CRJIntegralsDeletable() {}
		bool Remove(size_t ind) {
			return this->IsValidIndex(ind)?
				RemoveChecked(ind):
				false;
		}
		bool RemoveChecked(size_t ind) {
			this->m_sCount--;
			this->m_clStorage.MoveLeft(ind, this->m_sCount-ind);
			return true;
		}
	};
	template<typename TElem,typename DataStore=CRJDataStorage>
	class CRJIntegralsPlain:public CRJIntegralsDeletable<TElem, DataStore> {
	public:
		using CRJIntegralsDeletable<TElem, DataStore>::CRJIntegralsDeletable;
		bool Grow(size_t ind, TElem elem) {
			size_t need=ind+1;
			while (need<this->m_clStorage.GetVolume()) {
				if (!this->m_clStorage.DataAlloc(this->m_sAddStep)) return false;
			};
			return ArrayUpdate(ind, elem);
		}
		bool Insert(size_t ind, TElem elem) {
			size_t lastpos= this->m_sCount;
			if (this->checkVolume()&&this->IsValidIndex(ind)) {
				this->m_clStorage.MoveRight(ind, lastpos-ind,(Something) elem);
				return true;
			}
			return false;
		}
		bool Push(TElem elem) {
			size_t ind= this->m_sCount;
			if (!this->checkVolume()) return false;
			return this->m_clStorage.Set(ind,(Something)(size_t) elem);
		}
		bool Exist(TElem f) {
			SRJArrayFind finder;
			return find(f, finder);
		}
	protected:
		bool find(TElem f, SRJArrayFind& finder) {
			TElem elem;
			bool bComp=false, bFound=false;
			size_t c= this->m_sCount;
			while (c>0) {
				c--;
				elem=this->m_clStorage.ElemGet(c, bFound);
				if (bFound&& (TElem) elem==f) return finder.WasFound(c, elem);
			}
			return false;
		}
	};
	template<typename TElem,typename DataStore=CRJDataStorage>
	class CRJIntegrals:public CRJIntegralsDeletable<TElem, DataStore> {
	public:
		using CRJIntegralsDeletable<TElem, DataStore>::CRJIntegralsDeletable;
		struct SRJValueCache:public SRJArrayFind {
			SRJValueCache() {Reset();
			}
			inline bool IsItCached(TElem f) {return (m_bFound)?	m_pValue==(Something)(size_t) f:false;}
			inline bool Set(const SRJArrayFind& st) {m_pValue=st.m_pValue;m_sIndex=st.m_sIndex;return true;}
			inline bool Removed(size_t s) {if (m_sIndex>s) m_sIndex--;return true;}
			inline bool Inserted(size_t s) {if (m_sIndex>s) m_sIndex++;return true;}
		};
		bool Insert(TElem f,bool bExist=false) {
			SRJArrayFind finder;
			if (!find(f, finder)) {
				size_t c= this->m_sCount;
				if (this->checkVolume()) {
					this->m_clStorage.MoveRight(finder.m_sIndex, c-finder.m_sIndex,(Something)(size_t) f);
					return true;
				}
			}
			return bExist;
		}
		bool Insert(TElem f, SRJValueCache& stCache,bool bExist=false) {
			if (!stCache.IsItCached(f)) {
				SRJArrayFind finder;
				if (!find(f, finder)) {
					size_t lastpos= this->m_sCount;
					if (this->checkVolume()) {
						this->m_clStorage.MoveRight(finder.m_sIndex, lastpos-finder.m_sIndex, f);
						return stCache.Inserted(finder.m_sIndex);
					}
				}
			}
			return bExist;
		}
		bool Exist(TElem f) {
			SRJArrayFind finder;
			return find(f, finder);
		}
		bool Exist(TElem f, SRJValueCache& stCache) {
			if (stCache.IsItCached(f)) return true;
			SRJArrayFind finder;
			return find(f, finder)?	stCache.Set(finder):false;
		}
		bool Remove(TElem elem) {
			SRJArrayFind finder;
			return (find(elem, finder))?this->RemoveChecked(finder.m_sIndex):false;
		}
		bool Remove(TElem elem, SRJValueCache& stCache) {
			if (stCache.IsItCached(elem)) {
				size_t s=stCache.m_sIndex;
				stCache.Reset();
				return this->RemoveChecked(s);
			}
			SRJArrayFind finder;
			return (find(elem, finder))?stCache.Removed(finder.m_sIndex):false;
		}
	protected:
		bool find(TElem f, SRJArrayFind& finder) {
			Something elem;
			int64_t lComp;
			size_t i, firstpos=0, lastpos=this->m_sCount;
			while (firstpos<lastpos) {
				i=(lastpos-firstpos)>>1;
				i+=firstpos;
				if (!this->m_clStorage.Get(i,& elem)) return false;
				lComp=(int64_t) elem-(int64_t) f;
				if (!lComp) return finder.WasFound(i,(Something) elem);
				else if (lComp>0) lastpos=i;
				else firstpos=i+1;
			}
			finder.m_sIndex=firstpos;
			return false;
		}
	};
}

