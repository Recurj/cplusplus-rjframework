#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
namespace rjBase {
	class RJBASE_API CRJStringByteValue:public CRJStringByte {
	public:
		CRJStringByteValue() noexcept;
		CRJStringByteValue(const CRJStringByte&);
		CRJStringByteValue(const char*);
		CRJStringByteValue(CRJStringByteValue&&);
		CRJStringByteValue& operator=(const CRJStringByte&);
		virtual~CRJStringByteValue();
		inline bool IsAllocated() const {return m_pAllocated!=nullptr;}
	private:
		bool alloc(const CRJStringByte&);
		void notallocated();
		void Reset();
		void resetAllocation();
	private:
		static CRJAllocatorRuntime m_clHeap;
		uint8_t* m_pAllocated;
		size_t m_sSize;
	};
}

