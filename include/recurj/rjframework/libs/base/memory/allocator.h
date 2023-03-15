#pragma once
#include <recurj/rjframework/modules/defs/moddefbase.h>
#include <recurj/rjframework/libs/base/collections//lists/defs.h>
#include <recurj/rjframework/cpp.h>
#define RJ_ALLOCATOR(__type,__region,__index)  CRJAllocatorRuntime  __type::m_clHeap(__region, __index, sizeof(__type));\
void* __type::operator new(size_t s) { return __type::m_clHeap.Alloc(s); }\
void __type::operator delete(void* p) { __type::m_clHeap.Free((uint8_t *)p); }
#define RJ_ALLOCATORHEAP(__heap, __type,__region,__index)  CRJAllocatorRuntime  __type::__heap(__region, __index, sizeof(__type));\
void* __type::operator new(size_t s) { return __type::__heap.Alloc(s); }\
void __type::operator delete(void* p) { __type::__heap.Free((uint8_t *)p); }
#define RJ_ALLOCATOR_ANY(__type,__region,__index)  CRJAllocatorRuntime  __type::m_clHeap(__region, __index, 0 );\
void* __type::operator new(size_t s) { return __type::m_clHeap.Alloc(s); }\
void __type::operator delete(void* p) { __type::m_clHeap.Free((uint8_t *)p); }
#define RJ_ALLOCATOR_CATALOG(__type,__region,__index)  CRJAllocatorRuntime  __type::m_clHeap(__region, __index, AllocatorCatalog );\
void* __type::operator new(size_t s) { return __type::m_clHeap.Alloc(s); }\
void __type::operator delete(void* p) { __type::m_clHeap.Free((uint8_t *)p); }
#define RJ_ALLOCATOR_ANY_OUTSIDE(__type,__region,__index)  CRJAllocatorRuntime  __type::m_clHeap(__region, __index, 0 );
#define RJ_ALLOCATOR_CATALOG_OUTSIDE(__type,__region,__index)  CRJAllocatorRuntime  __type::m_clHeap(__region, __index,AllocatorCatalog);
namespace rjBase {
	const size_t AllocatorCatalog = 0x8000000000000000;
	using Something=void*;
	struct Allocator {
		enum Region:size_t {
			Unknown=0, Base, BStrings, UStrings,BTree, Map, Array,
			Storage, Task, Message, Net, Security, Lua, JS,
			WWW, XML, DB, App=1024
		};
	};
	struct AllocatorBase {
		enum Elem:size_t {
			HeapCatalog
		};
	};
	struct AllocatorBStrings {
		enum Elem :size_t {
			StaticAllocated,
			VarAllocated,
		};
	};

	struct AllocatorUStrings {
		enum Elem :size_t {
			StaticAllocated,
			VarAllocated,
		};
	};

	struct AllocatorBTree {
		enum Elem:size_t {
			HeapCatalogSize, Integral, Compound,
			CatalogIndex, CatalogLeavesIndex,
			RBTree
		};
	};
	struct AllocatorMap {
		enum Elem:size_t {
			HeapAllocatorCatalog, HeapStringByte2StringByte, HeapHandbook,
			App = 1024
		};
	};
	struct AllocatorStorage {
		enum Elem:size_t {
			Region,Path
		};
	};
	struct AllocatorTask {
		enum Elem:size_t {
			Info
		};
	};
	struct AllocatorDB {
		enum Elem:size_t {
			Info
		};
	};
	struct SAllocatorRuntimeInfo {
		SAllocatorRuntimeInfo(size_t r, size_t t, size_t s):
			m_sRegion(r),m_sIndex(t),m_sSize(s) {}
		SAllocatorRuntimeInfo(size_t r, size_t t) :
			m_sRegion(r), m_sIndex(t), m_sSize(0) {}
		size_t m_sRegion;
		size_t m_sIndex;
		size_t m_sSize;
	};
	class RJBASE_API IRJAllocator {
	public:
		virtual~IRJAllocator() {};
		virtual bool Release()=0;
		virtual uint8_t* Alloc(size_t)=0;
		virtual size_t Free(uint8_t*)=0;
		virtual size_t GetSize(uint8_t*)=0;
		virtual bool Validate(uint8_t*)=0;
	};
	class RJBASE_API CRJAllocatorDefault:public IRJAllocator {
	public:
		CRJAllocatorDefault()=default;
		virtual~CRJAllocatorDefault() {};
		bool Release() override;
		uint8_t* Alloc(size_t) override;
		size_t Free(uint8_t*) override;
		size_t GetSize(uint8_t*) override;
		bool Validate(uint8_t*) override;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJAllocatorDefault)
	};
	class RJBASE_API CRJAllocatorRuntime final {
	public:
		using funcMode=IRJAllocator&(CRJAllocatorRuntime::*)();
		IRJAllocator& GetAllocator();
		CRJAllocatorRuntime(size_t, size_t, size_t);
		virtual~CRJAllocatorRuntime();
		bool Release();
		uint8_t* Alloc(size_t);
		size_t Free(uint8_t*);
		size_t GetSize(uint8_t*);
		bool Validate(uint8_t*);
		size_t PreAllocate(size_t, size_t);
		bool Realloc(size_t, uint8_t**, const uint8_t* p, size_t);
		static size_t GetSizeAligned(size_t, size_t);
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJAllocatorRuntime)
	protected:
	private:
		IRJAllocator& ModeStartup();
		IRJAllocator& ModeWork();
	private:
		SAllocatorRuntimeInfo m_stInfo;
		IRJAllocator* m_pAllocator;
		funcMode m_clMode;
	};
	class RJBASE_API CRJAllocator :public CRJListItem< CRJAllocator>, public IRJAllocator {
		friend class CRJListAllocators;
		friend class CRJApplicationMemory;
	public:
		struct SRJAllocatorStatistic {
			int64_t lMaxCount;
			int64_t lMaxSize;
			int64_t lUseCount;
			int64_t lFreeCount;
			int64_t lUseSize;
			int64_t lFreeSize;
			int64_t lErrorCount;
			void* m_pUserData;
		};
		virtual bool Validate(void*) {
			return true;
		}
		void tsInUse(size_t);
		void tsNotUsed(size_t);
		void tsUse2Free(size_t);
		void tsFree2Use(size_t);
		uint8_t* tsNotAllocated();
		inline bool IsInUse() const {return m_stInfo.lUseCount != 0;}
	protected:
		CRJAllocator(size_t, size_t);
		virtual~CRJAllocator();
		void Reset();
	protected:
		SRJAllocatorStatistic m_stInfo;
		size_t m_sRegion, m_sIndex;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJAllocator)
	};
}

