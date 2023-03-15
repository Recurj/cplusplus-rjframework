#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
using namespace rjBase;
RJ_ALLOCATOR_CATALOG_OUTSIDE(CRJStringByteVarHeap, Allocator::Region::BStrings, AllocatorBStrings::VarAllocated)
void CRJStringByteVarHeap::FreeBuffer(uint8_t*p) {
	m_clHeap.Free(p);
}

CRJStringByteAllocated::CRJStringByteAllocated()noexcept
{}
CRJStringByteAllocated::CRJStringByteAllocated(const CRJStringByte& s):
	CRJStringByteVarHeapBufferAllocated(s) {}
CRJStringByteAllocated::CRJStringByteAllocated(const CRJStringByteAllocated& s):
	CRJStringByteVarHeapBufferAllocated((CRJStringByte&) s) {}
CRJStringByteAllocated::CRJStringByteAllocated(const char* p):
	CRJStringByteVarHeapBufferAllocated(p) {}
CRJStringByteAllocated::~CRJStringByteAllocated() {}
CRJStringByteAllocated& CRJStringByteAllocated::operator=(const CRJStringByteAllocated& s) {
	Set(s);
	return *this;
}
CRJStringByteAllocated::CRJStringByteAllocated(CRJStringByteAllocated&& s)
{
	Attach(std::move(s));
}
CRJStringByteAllocatedRef::CRJStringByteAllocatedRef(const CRJStringByte&s) noexcept:
	CRJStringByteAllocated(s)
{}
CRJStringByteAllocatedRef::CRJStringByteAllocatedRef() noexcept
{}
CRJStringByteAllocatedRef::~CRJStringByteAllocatedRef()
{}
void CRJStringByteAllocatedRef::SetReference(const CRJStringByte& s)
{
	SetHeadAndLength(s.GetHeadForRead(), s.GetLength());
}
