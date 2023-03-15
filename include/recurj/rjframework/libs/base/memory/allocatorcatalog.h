#pragma once
#include <recurj/rjframework/libs/base/memory/allocatorpolicy.h>
#include <recurj/rjframework/libs/base/os/hheap.h>
#include <mutex>
#include <map>
namespace rjBase {
	class RJBASE_API CRJAllocatorCatalog:public CRJAllocatorPolicy {
	public:
		struct SRJAllocatorList {
			SRJAllocatorList* pNext;
			CRJAllocatorCatalog* pHeap;
			size_t sSize;
		};
		CRJAllocatorCatalog(size_t, size_t);
		virtual~CRJAllocatorCatalog();
		uint8_t* Alloc(size_t) override;
		size_t Free(uint8_t*) override;
		size_t GetSize(uint8_t*) override;
		bool Validate(uint8_t*) override;
	protected:
		void preRelease() override;
		bool addElem(SRJAllocatorList*, size_t);
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJAllocatorCatalog)
	private:
		std::map<size_t, SRJAllocatorList *> m_clMap;
	};
	class RJBASE_API CRJAllocatorCatalogNative:public CRJAllocatorCatalog {
		friend class CRJApplicationMemory;
	public:
		virtual~CRJAllocatorCatalogNative();
	protected:
		CRJAllocatorCatalogNative(size_t, size_t, size_t lOpts=0, size_t lIniSize=0, size_t lMaxSize=0);
	private:
		RJHeap m_clOSAllocator;
	};
};

