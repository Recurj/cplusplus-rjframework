#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/os/hhandle.h>
namespace rjBase {
	class RJBASE_API RJHeap:public IRJAllocator {
	public:
		RJHeap();
		virtual~RJHeap();
		bool Create(size_t, size_t, size_t);
		virtual bool Release();
		uint8_t* Alloc(size_t) override;
		size_t Free(uint8_t*) override;
		size_t GetSize(uint8_t*) override;
		bool Validate(uint8_t*) override;
	protected:
#ifdef _rjOS_WIN
		class RJHeapOS:public RJHandle {
		public:
			virtual bool HandleDestroy();
			bool Create(size_t, size_t, size_t);
			uint8_t* Alloc(size_t);
			size_t Free(uint8_t*);
			size_t Size(uint8_t*);
			bool Validate(uint8_t*);
		};
	private:
		RJHeapOS m_clOS;
#endif 
		_CLASS_COPY_AND_ASSIGN_DELETE(RJHeap)
	};
}

