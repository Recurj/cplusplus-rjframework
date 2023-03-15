#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/strings/unicode/var.h>
namespace rjBase {
	class RJBASE_API CRJStringUnicodeVarHeap :public CRJStringUnicodeVar {
	public:
		using CRJStringUnicodeVar::CRJStringUnicodeVar;
	protected:
		static CRJAllocatorRuntime m_clHeap;
	};
	template<size_t VOLUME>
	class CRJStringUnicodeVarHeapBuffer :public CRJStringUnicodeVarHeap {
	public:
		CRJStringUnicodeVarHeapBuffer()noexcept :
			CRJStringUnicodeVarHeap(VOLUME) {
			SetHead(m_szBuffer);
		}
		CRJStringUnicodeVarHeapBuffer(const CRJStringUnicode& clStr) :
			CRJStringUnicodeVarHeap(VOLUME) {
			SetHead(m_szBuffer);
			Set(clStr);
		}
		virtual~CRJStringUnicodeVarHeapBuffer() {}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringUnicodeVarHeapBuffer)
	protected:
		wchar_t m_szBuffer[VOLUME];
	};
	template<size_t VOLUME, size_t PAGE=VOLUME>
	class CRJStringUnicodeVarBufferAllocated:public CRJStringUnicodeVarHeapBuffer<VOLUME> {
	public:
		CRJStringUnicodeVarBufferAllocated()noexcept:
			m_pAllocated(nullptr),
			m_sPage(PAGE) {}
		CRJStringUnicodeVarBufferAllocated(const CRJStringUnicode& s)noexcept:
		m_pAllocated(nullptr),m_sPage(PAGE) {this->Set(s);}
		CRJStringUnicodeVarBufferAllocated(const wchar_t* p)noexcept:
		m_pAllocated(nullptr),m_sPage(PAGE) {this->SetLabel(p);}
		virtual~CRJStringUnicodeVarBufferAllocated() {
			ResetAllocation();
		}
		bool HasNeededSize(size_t s) override {
			return (m_pAllocated!=nullptr&&	this->m_sSize>=s)?true:AttachBuffer(s);
		}
		void ResetAllocation() {
			if (IsAllocated()) {
				this->m_clHeap.Free((uint8_t*)m_pAllocated);
				this->SetHead(this->m_szBuffer);
				this->m_sSize=VOLUME;
				this->SetEmpty();
			}
		}
		inline bool IsAllocated() const {
			return m_pAllocated!=nullptr;
		}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJStringUnicodeVarBufferAllocated)
	private:
		bool AttachBuffer(size_t s) {
			try {
				s= PAGE==1?s:this->m_clHeap.GetSizeAligned(PAGE, s);
				if (this->m_clHeap.Realloc(s*sizeof(wchar_t),(uint8_t **) & m_pAllocated, (uint8_t*)this->GetHeadForRead(), this->m_sLength * sizeof(wchar_t))) {
					this->SetHead(m_pAllocated);
					this->m_sSize=s;
					return true;
				}
			}
			catch(...) {}
			return false;
		}
	protected:
		void Attach(CRJStringUnicodeVarBufferAllocated&&s) {
			m_pAllocated=s.m_pAllocated;
			this->m_sSize=s.m_sSize;
			this->m_sLength=s.m_sLength;
			s.ResetAllocation();
		}
	protected:
		wchar_t* m_pAllocated;
		size_t m_sPage;
	};
	RJBASE_TEMPLATE template
		class RJBASE_API CRJStringUnicodeVarBufferAllocated<cSizeChainDefault>;
	using CRJStringUnicodeExpanded=RJBASE_API CRJStringUnicodeVarBufferAllocated<cSizeChainDefault>;
	class RJBASE_API CRJStringUnicodeAllocated:public CRJStringUnicodeVarBufferAllocated<1, 1> {
	public:
		CRJStringUnicodeAllocated()noexcept;
		CRJStringUnicodeAllocated(const CRJStringUnicode&);
		CRJStringUnicodeAllocated(const wchar_t*);
		CRJStringUnicodeAllocated(const CRJStringUnicodeAllocated&);
		CRJStringUnicodeAllocated(CRJStringUnicodeAllocated&&);
		CRJStringUnicodeAllocated&operator=(const CRJStringUnicodeAllocated&);
		virtual~CRJStringUnicodeAllocated();
	};
	class RJBASE_API CRJStringUnicodeAllocatedRef : public CRJStringUnicodeAllocated {
	public:
		CRJStringUnicodeAllocatedRef(const CRJStringUnicode&) noexcept;
		CRJStringUnicodeAllocatedRef() noexcept;
		virtual ~CRJStringUnicodeAllocatedRef();
		void SetReference(const CRJStringUnicode&);
	};
}

