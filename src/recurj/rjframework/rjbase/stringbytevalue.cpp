#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/strings/byte/value.h>
using namespace rjBase;
RJ_ALLOCATOR_CATALOG_OUTSIDE(CRJStringByteValue, Allocator::Region::BStrings, AllocatorBStrings::StaticAllocated);
CRJStringByteValue::CRJStringByteValue() noexcept :
	m_pAllocated(nullptr),
	m_sSize(0) 
{}
CRJStringByteValue::CRJStringByteValue(const CRJStringByte& s):
m_pAllocated(nullptr),
m_sSize(0) {
	alloc(s);
}
CRJStringByteValue::~CRJStringByteValue() {
	notallocated();
}
CRJStringByteValue::CRJStringByteValue(const char* s) :
	m_pAllocated(nullptr),
	m_sSize(0) {
	alloc(s);
}
CRJStringByteValue::CRJStringByteValue(CRJStringByteValue&& s) {
	m_pAllocated = s.m_pAllocated;
	m_sSize = s.m_sSize;
	m_sLength = s.m_sLength;
	SetHead(m_pAllocated);
	s.resetAllocation();
}
CRJStringByteValue& CRJStringByteValue::operator=(const CRJStringByte& s)
{
	if (&s != this) {
		alloc(s);
	}
	return *this;
}
bool CRJStringByteValue::alloc(const CRJStringByte& str) {
	try {
		size_t l=str.GetLength();
		if (!l) {
			notallocated();
			SetConstantDefinedEmpty();
			return true;
		}
		else {
			size_t v=l+1;
			if (m_clHeap.Realloc(v,& m_pAllocated,str.GetHeadForRead(), v)) {
				m_sSize=v;
				m_pAllocated[l]=0x00;
				SetHeadAndLength(m_pAllocated, l);
			}
		}
	}
	catch(...) {}
	return false;
}
void CRJStringByteValue::notallocated() {
	if (IsAllocated()) Reset();
	else SetConstantUndefined();
}
void CRJStringByteValue::Reset() {
	m_clHeap.Free(m_pAllocated);
	resetAllocation();
}
void CRJStringByteValue::resetAllocation() {
	SetConstantUndefined();
	m_pAllocated = nullptr;
	m_sSize = 0;
}

