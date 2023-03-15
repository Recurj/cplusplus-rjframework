#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/collections//chains/var.h>
 template<typename TData>
class CRJChainVarAllocated:public CRJChainVar<TData> {
public:
	CRJChainVarAllocated():CRJChainVar(0),
	m_clHeap(clAllocator),
	m_pAllocated(nullptr) {}
	virtual~CRJChainVarAllocated() {
		ResetAllocation();
	}
	virtual bool HasNeededSize(size_t s) {
		return (m_lSize>=s)?
			true:attachBuffer(s);
	}
	void ResetAllocation() {
		realeaseAllocated();
		SetVarUndefined();
	}
	inline bool IsAllocated() const {
		return m_pAllocated!=nullptr;
	}
private:
	bool attachBuffer(size_t s) {
		ResetAllocation();
		try {
			size_t v=CRJDataTools::SizeAlignDefault(s*sizeof(TData));
			if constexpr(std::is_pointer<TData>::value||
				std::is_arithmetic<TData>::value) {
				if (m_clHeap.Realloc(v,(void**)& m_pAllocated,(const void*) GetHeadForRead(), m_sLength*sizeof(TData))) {
					SetHead(m_pAllocated);
					m_lSize=s;
					SetEmpty();
					return true;
				}
			}
			else {
				TData* p=(TData*) m_clHeap.Alloc(v);
				if (p) {
					TData* q=p;
					size_t i;
					for (i=0; i<s; i++, q++)new(q) TData();
					m_pAllocated=p;
					SetHead(m_pAllocated);
					m_lSize=s;
					SetEmpty();
					return true;
				}
			}
		}
		catch(...) {}
		return false;
	}
	void realeaseAllocated() {
		try {
			if (m_pAllocated) {
				if constexpr(!std::is_pointer<TData>::value&&
					!std::is_arithmetic<TData>::value) {
					TData* p=(TData*) m_pAllocated;
					for (size_t i=0; i<m_lSize; i++, p++) p->~TData();
				}
				m_clHeap.Free(m_pAllocated);
			}
		}
		catch(...) {}
	}
private:
	static CRJAllocatorRuntime m_clHeap;
	TData* m_pAllocated;
};

