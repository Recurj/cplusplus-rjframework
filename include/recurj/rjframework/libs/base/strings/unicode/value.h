#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/strings/unicode/string.h>
namespace rjBase {
	class RJBASE_API CRJStringUnicodeValue :public CRJStringUnicode {
	public:
		CRJStringUnicodeValue() noexcept;
		CRJStringUnicodeValue(const CRJStringUnicode&);
		CRJStringUnicodeValue(const wchar_t*);
		CRJStringUnicodeValue(CRJStringUnicodeValue&&);
		CRJStringUnicodeValue& operator=(const CRJStringUnicode&);
		virtual~CRJStringUnicodeValue();
		inline bool IsAllocated() const {
			return m_pAllocated != nullptr;
		}
	private:
		bool alloc(const CRJStringUnicode&);
		void notallocated();
		void Reset();
		void resetAllocation();
	private:
		static CRJAllocatorRuntime m_clHeap;
		wchar_t* m_pAllocated;
		size_t m_sSize;
	};
}

