#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/os/hheap.h>
#include <mutex>
namespace rjBase {
	class RJBASE_API CRJAllocatorPolicy :public CRJAllocator {
	public:
		virtual ~CRJAllocatorPolicy();
		bool Release() override;
	protected:
		CRJAllocatorPolicy(size_t, size_t);
		virtual void preRelease() = 0;
		void Detach();
		std::mutex m_clMutex;
		IRJAllocator* m_pAllocator;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJAllocatorPolicy)
	};
};

