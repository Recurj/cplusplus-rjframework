#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
namespace rjBase {
	class RJBASE_API CRJStringByteVarHeap :public CRJStringByteVar {
	public:
		static void FreeBuffer(uint8_t*);
	protected:
		using CRJStringByteVar::CRJStringByteVar;
		static CRJAllocatorRuntime m_clHeap;
	};
	template<size_t VOLUME>
	class CRJStringByteVarHeapBuffer :public CRJStringByteVarHeap {
	public:
		CRJStringByteVarHeapBuffer()noexcept :
			CRJStringByteVarHeap(VOLUME) {
			SetHead(m_szBuffer);
		}
		CRJStringByteVarHeapBuffer(const CRJStringByte& clStr) :
			CRJStringByteVarHeap(VOLUME) {
			SetHead(m_szBuffer);
			Set(clStr);
		}
		virtual~CRJStringByteVarHeapBuffer() {}
	protected:
		void reset() {
			SetHeadAndLength(m_szBuffer,0);
		}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringByteVarHeapBuffer)
	protected:
		uint8_t m_szBuffer[VOLUME];
	};

	template<size_t VOLUME, size_t PAGE = VOLUME>
	class CRJStringByteVarHeapBufferAllocated :public CRJStringByteVarHeapBuffer<VOLUME> {
	public:
		CRJStringByteVarHeapBufferAllocated()noexcept :
			m_pAllocated(nullptr),
			m_sPage(PAGE) {}
		CRJStringByteVarHeapBufferAllocated(const CRJStringByte& s)noexcept :
			m_pAllocated(nullptr),
			m_sPage(PAGE) {
			this->Set(s);
		}
		CRJStringByteVarHeapBufferAllocated(const char* p)noexcept :
			m_pAllocated(nullptr),
			m_sPage(PAGE) {
			this->SetLabel(p);
		}
		virtual~CRJStringByteVarHeapBufferAllocated() {
			ResetAllocation();
		}
		bool HasNeededSize(size_t s) override {
			return (this->m_sSize >= s) ?
				true : AttachBuffer(s);
		}
		void ResetAllocation() {
			if (IsAllocated()) {
				this->m_clHeap.Free(m_pAllocated);
				this->SetHead(this->m_szBuffer);
				this->m_sSize = VOLUME;
				this->SetEmpty();
			}
		}
		inline CRJStringByteVarHeapBufferAllocated& SetPageSize(size_t p) {
			m_sPage = p;
			return *this;
		}
		inline bool IsAllocated() const {
			return m_pAllocated != nullptr;
		}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringByteVarHeapBufferAllocated)
	private:
		bool AttachBuffer(size_t s) {
			try {
				s = PAGE == 1 ? s : this->m_clHeap.GetSizeAligned(m_sPage, s);
				if (this->m_clHeap.Realloc(s, &m_pAllocated, this->GetHeadForRead(), this->m_sLength)) {
					this->SetHead(m_pAllocated);
					this->m_sSize = s;
					return true;
				}
			}
			catch (...) {}
			return false;
		}
	protected:
		void Attach(CRJStringByteVarHeapBufferAllocated&& s) {
			m_pAllocated = s.m_pAllocated;
			this->m_sSize = s.m_sSize;
			this->SetHead(m_pAllocated);
			this->m_sLength = s.m_sLength;
			s.ResetAllocation();
		}
	protected:
		uint8_t* m_pAllocated;
		size_t m_sPage;
	};
	RJBASE_TEMPLATE template
		class RJBASE_API CRJStringByteVarHeapBufferAllocated<cSizeChainDefault>;
	using CRJStringByteExpanded = RJBASE_API CRJStringByteVarHeapBufferAllocated<cSizeChainDefault>;
	class RJBASE_API CRJStringByteAllocated :public CRJStringByteVarHeapBufferAllocated<1, 1> {
	public:
		CRJStringByteAllocated() noexcept;
		CRJStringByteAllocated(const CRJStringByte&);
		CRJStringByteAllocated(const char*);
		CRJStringByteAllocated(const CRJStringByteAllocated&);
		CRJStringByteAllocated(CRJStringByteAllocated&&);
		CRJStringByteAllocated& operator=(const CRJStringByteAllocated&);
		virtual~CRJStringByteAllocated();
	};
	class RJBASE_API CRJStringByteAllocatedRef: public CRJStringByteAllocated {
	public:
		CRJStringByteAllocatedRef(const CRJStringByte&) noexcept;
		CRJStringByteAllocatedRef() noexcept;
		virtual ~CRJStringByteAllocatedRef();
		void SetReference(const CRJStringByte&);
	protected:
	};
}

