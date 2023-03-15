#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <mutex>
namespace rjBase {
	class RJBASE_API CRJApplicationMemory {
	public:
		virtual IRJAllocator* AppAllocatorRuntime(const SAllocatorRuntimeInfo&);
		static IRJAllocator& GetAllocatorDefault();
	protected:
		CRJApplicationMemory();
		virtual ~CRJApplicationMemory();
		virtual CRJAllocator* AppAllocatorRuntimeAny(const SAllocatorRuntimeInfo&);
		virtual CRJAllocator* AppAllocatorRuntimeFixed(const SAllocatorRuntimeInfo&);
		virtual CRJAllocator* AppAllocatorRuntimeCatalog(const SAllocatorRuntimeInfo&);
	private:
		void Register(CRJAllocator&);
		void Release();
		static CRJAllocatorDefault m_clAllocatorDefault;
		std::mutex m_clMutex;
		CRJListBase< CRJAllocator> m_clAllocators;
	};

}

