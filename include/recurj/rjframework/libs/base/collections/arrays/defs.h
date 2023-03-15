#pragma once
#include <recurj/rjframework/libs/base/storage/list.h>
 template<typename T>
struct is_delete_placement {
	static const bool value=false;
};
namespace rjBase {
	template<typename TElem>
	constexpr TElem ArrayNoValue() {
		if constexpr(std::is_pointer<TElem>::value) return nullptr;
		else return 0;
	}
	struct SRJArrayFind {
		inline void Reset() {
			m_bFound=false;
		}
		bool WasFound(size_t ind, Something pElem) {
			m_bFound=true;
			m_pValue=pElem;
			m_sIndex=ind;
			return true;
		}
		bool NotFound(size_t ind) {
			m_sIndex=ind;
			return false;
		}
		Something m_pValue;
		size_t m_sIndex;
		bool m_bFound;
	};
	template<typename TElem,typename Storage=CRJDataStorage>
	class CRJArrayIterator {
	public:
		explicit CRJArrayIterator(const Storage& store, size_t sIndex):
			m_pStore(& store),
		m_sIndex(sIndex) {}
		CRJArrayIterator(const CRJArrayIterator& o)=default;
		CRJArrayIterator&operator=(const CRJArrayIterator& o)=default;
		~CRJArrayIterator()=default;
		CRJArrayIterator&operator++() {
			++m_sIndex;
			return *this;
		}
		TElem operator*() const {
			Something p=nullptr;
			return m_pStore->Get(m_sIndex,& p)?
				(TElem) p:ArrayNoValue<TElem>();
		}
		bool operator!=(const CRJArrayIterator& other) const {
			return m_sIndex!=other.m_sIndex;
		}
		bool operator==(const CRJArrayIterator& other) const {
			return m_sIndex==other.m_sIndex;
		}
	private:
		const Storage* m_pStore;
		size_t m_sIndex;
	};
	template<typename TElem,typename Storage=CRJDataStorage>
	class CRJArrayBase {
	public:
		using iterator=CRJArrayIterator<TElem, Storage>;
		iterator begin() {
			return iterator(m_clStorage, 0);
		}
		iterator end() {
			return iterator(m_clStorage, m_sCount);
		}
		iterator begin() const {
			return iterator(m_clStorage, 0);
		}
		iterator end() const {
			return iterator(m_clStorage, m_sCount);
		}
		virtual void Clear() {
			m_sCount=0;
			this->m_clStorage.Clear();
		}
		bool Get(size_t ind, TElem& val) {
			if (this->IsValidIndex(ind)) {
				val=(*this)[ind];
				return true;
			}
			return false;
		}
		TElem Get(size_t ind,bool& bValid) {
			bValid=this->IsValidIndex(ind);
			return bValid?(*this)[ind]:ArrayNoValue<TElem>();
		}
		TElem operator[](size_t ind) const {
			Something p=nullptr;
			return this->m_clStorage.Get(ind,&p)? (TElem) p:ArrayNoValue<TElem>();
		}
		bool Set(size_t ind, TElem elem) {
			return this->IsValidIndex(ind)?
				this->m_clStorage.Set(ind,(Something) elem):
				false;
		}
		bool SetChecked(size_t ind, TElem elem) {
			return this->m_clStorage.Set(ind,(Something) elem);
		}
		void Allocate(size_t sAlloc) {
			this->m_clStorage.Alloc(sAlloc);
		}
		inline bool IsValidIndex(size_t ind) const {
			return ind<m_sCount;
		}
		inline size_t Count() const {
			return m_sCount;
		}
		inline void Trunc(size_t c) {
			if (c < m_sCount) {
				m_sCount = c;
			}
		}
	protected:
		CRJArrayBase(size_t sStep, size_t sAlloc) {
			m_sCount=0;
			m_sAddStep=sStep;
			Allocate(sAlloc);
		}
		virtual~CRJArrayBase() {
			Clear();
		}
		bool checkVolume() {
			if (m_sCount==this->m_clStorage.GetVolume()) {
				if (!this->m_clStorage.Alloc(m_sAddStep)) return false;
			}
			m_sCount++;
			return true;
		}
	protected:
		Storage m_clStorage;
		size_t m_sCount, m_sAddStep;
	};
}

