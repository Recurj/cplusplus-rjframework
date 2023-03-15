#pragma once
#include <recurj/rjframework/libs/base/memory/allocatorpolicy.h>
#include <recurj/rjframework/libs/base/os/hheap.h>
#include <mutex>
namespace rjBase {
	class RJBASE_API CRJAllocatorAny:public CRJAllocatorPolicy {
	public:
		CRJAllocatorAny(size_t, size_t);
		virtual~CRJAllocatorAny();
		uint8_t* Alloc(size_t) override;
		size_t Free(uint8_t*) override;
		size_t GetSize(uint8_t*) override;
		bool Validate(uint8_t*) override;
	protected:
		void preRelease() override;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJAllocatorAny)
	};
	class RJBASE_API CRJAllocatorAnyNative:public CRJAllocatorAny {
		friend class CRJApplicationMemory;
	public:
		virtual~CRJAllocatorAnyNative();
	protected:
		CRJAllocatorAnyNative(size_t, size_t, size_t lOpts=0, size_t lIniSize=0, size_t lMaxSize=0);
	private:
		RJHeap m_clOSAllocator;
	};
};

