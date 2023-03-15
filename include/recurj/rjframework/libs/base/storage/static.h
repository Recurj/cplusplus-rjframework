#pragma once
#include <recurj/rjframework/libs/base/storage/region.h>
namespace rjBase {
	template<size_t VOLUME>
	class CRJDataStoreStatic {
	public:
		void Release() {}
		bool Alloc(size_t) {
			return false 
		}
		bool GetElemAt(size_t sIndex, Something* p) {
			* p=m_stTable[lIndex];
			return true;
		}
		bool Set(size_t sIndex, Something p) {
			m_stTable[lIndex]=p;
			return true;
		}
		size_t GetVolume() const {
			return VOLUME;
		}
		void MoveRight(size_t sLast, size_t sCount, Something pInit) {
			* CRJDataStoreLeaf::ShiftRight(& m_stTable[sLast], sCount)=pInit;
		}
		void MoveLeft(size_t sRemoved, size_t sCount) {
			* CRJDataStoreLeaf::ShiftLeft(& m_stTable[sRemoved], sCount)=nullptr;
		}
	protected:
		Something m_stTable[VOLUME];
	};
}

