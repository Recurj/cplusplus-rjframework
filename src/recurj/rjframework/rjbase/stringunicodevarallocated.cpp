#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/unicode/allocated.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
using namespace rjBase;

RJ_ALLOCATOR_CATALOG_OUTSIDE(CRJStringUnicodeVarHeap, Allocator::Region::UStrings, AllocatorUStrings::VarAllocated);

CRJStringUnicodeAllocated::CRJStringUnicodeAllocated() noexcept :
	CRJStringUnicodeVarBufferAllocated() {}
CRJStringUnicodeAllocated::CRJStringUnicodeAllocated(const CRJStringUnicode& s) :
	CRJStringUnicodeVarBufferAllocated(s) {}
CRJStringUnicodeAllocated::CRJStringUnicodeAllocated(const CRJStringUnicodeAllocated& s) :
	CRJStringUnicodeVarBufferAllocated((CRJStringUnicode&)s) {}
CRJStringUnicodeAllocated::CRJStringUnicodeAllocated(const wchar_t* p) :
	CRJStringUnicodeVarBufferAllocated(p) {}
CRJStringUnicodeAllocated::~CRJStringUnicodeAllocated() {}
CRJStringUnicodeAllocated& CRJStringUnicodeAllocated::operator=(const CRJStringUnicodeAllocated& s) {
	Set(s);
	return *this;
}
CRJStringUnicodeAllocated::CRJStringUnicodeAllocated(CRJStringUnicodeAllocated&& s)
{
	Attach(std::move(s));
}
CRJStringUnicodeAllocatedRef::CRJStringUnicodeAllocatedRef(const CRJStringUnicode& s)  noexcept :
	CRJStringUnicodeAllocated(s)
{}
CRJStringUnicodeAllocatedRef::CRJStringUnicodeAllocatedRef() noexcept
{}
CRJStringUnicodeAllocatedRef::~CRJStringUnicodeAllocatedRef()
{}
void CRJStringUnicodeAllocatedRef::SetReference(const CRJStringUnicode& s)
{
	SetHeadAndLength(s.GetHeadForRead(), s.GetLength());
}