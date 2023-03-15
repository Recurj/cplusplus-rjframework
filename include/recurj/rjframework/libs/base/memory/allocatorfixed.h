#pragma once
#include <recurj/rjframework/libs/base/memory/allocatorpolicy.h>
#include <recurj/rjframework/libs/base/os/hheap.h>
#include <mutex>
namespace rjBase {
	class RJBASE_API CRJAllocatorFixed:public CRJAllocatorPolicy {
	public:
		struct SRJAllocatorList {
			SRJAllocatorList* pNext;
			CRJAllocatorFixed* pHeap;
		};
		CRJAllocatorFixed(size_t, size_t, size_t);
		virtual~CRJAllocatorFixed();
		uint8_t* Alloc(size_t) override;
		size_t Free(uint8_t*) override;
		size_t GetSize(uint8_t*) override;
		bool Validate(uint8_t*) override;
		size_t PreAllocate(size_t);
		bool SetElemSize(size_t);
		inline size_t GetElemSize() const {	return m_lElemSize;}
	protected:
		void preRelease() override;
		SRJAllocatorList* m_pFirstFree;
		size_t m_lElemSize;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJAllocatorFixed)
	};
	class RJBASE_API CRJAllocatorFixedNative:public CRJAllocatorFixed {
		friend class CRJApplicationMemory;
	public:
		virtual~CRJAllocatorFixedNative();
	protected:
		CRJAllocatorFixedNative(size_t, size_t, size_t, size_t lOpts=0, size_t lIniSize=0, size_t lMaxSize=0);
	private:
		RJHeap m_clOSAllocator;
	};
};

