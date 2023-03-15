#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/strings/unicode/value.h>
using namespace rjBase;
RJ_ALLOCATOR_CATALOG_OUTSIDE(CRJStringUnicodeValue, Allocator::Region::UStrings, AllocatorUStrings::StaticAllocated);
CRJStringUnicodeValue::CRJStringUnicodeValue() noexcept :
	m_pAllocated(nullptr),
	m_sSize(0)
{}
CRJStringUnicodeValue::CRJStringUnicodeValue(const CRJStringUnicode& s) :
	m_pAllocated(nullptr),
	m_sSize(0) {
	alloc(s);
}
CRJStringUnicodeValue::~CRJStringUnicodeValue() {
	notallocated();
}
CRJStringUnicodeValue::CRJStringUnicodeValue(const wchar_t* s) :
	m_pAllocated(nullptr),
	m_sSize(0) {
	alloc(s);
}
CRJStringUnicodeValue::CRJStringUnicodeValue(CRJStringUnicodeValue&& s) {
	m_pAllocated = s.m_pAllocated;
	m_sSize = s.m_sSize;
	m_sLength = s.m_sLength;
	SetHead(m_pAllocated);
	s.resetAllocation();
}
CRJStringUnicodeValue& CRJStringUnicodeValue::operator=(const CRJStringUnicode& s)
{
	if (&s != this) {
		alloc(s);
	}
	return *this;
}
void CRJStringUnicodeValue::notallocated() {
	if (IsAllocated()) Reset();
	else SetConstantUndefined();
}
void CRJStringUnicodeValue::Reset() {
	m_clHeap.Free((uint8_t *)m_pAllocated);
	resetAllocation();
}
void CRJStringUnicodeValue::resetAllocation() {
	SetConstantUndefined();
	m_pAllocated = nullptr;
	m_sSize = 0;
}
bool CRJStringUnicodeValue::alloc(const CRJStringUnicode& str) {
	try {
		size_t l=str.GetLength();
		if (!l) {
			notallocated();
			SetConstantDefinedEmpty();
			return true;
		}
		else {
			size_t v=l+1;
			if (m_clHeap.Realloc(v*sizeof(wchar_t),(uint8_t**)& m_pAllocated,(const uint8_t*) str.GetHeadForRead(), v*sizeof(wchar_t))) {
				m_sSize=l;
				m_pAllocated[l]=0x00;
				SetHeadAndLength(m_pAllocated, l);
			}
		}
	}
	catch(...) {}
	return false;
}
