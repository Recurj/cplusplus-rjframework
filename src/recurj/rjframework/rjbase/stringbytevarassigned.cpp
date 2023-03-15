#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/byte/var.h>
using namespace rjBase;
CRJStringByteVarAssigned::CRJStringByteVarAssigned() {}
CRJStringByteVarAssigned::CRJStringByteVarAssigned(size_t s):
	CRJStringByteVar(s) {}
CRJStringByteVarAssigned::CRJStringByteVarAssigned(uint8_t* p, size_t s):
	CRJStringByteVar(s) {
	SetHead(p);
}
CRJStringByteVarAssigned::CRJStringByteVarAssigned(const CRJStringByte& clValue):
	CRJStringByteVar(clValue.GetLength()) {
	SetHead(clValue.GetHeadForRead());
}
CRJStringByteVarAssigned& CRJStringByteVarAssigned::SetStorageWithData(const CRJStringByte& s) {
	m_sSize=s.GetLength();
	SetHeadAndLength(s.GetHeadForRead(), s.GetLength());
	return *this;
}
CRJStringByteVarAssigned& CRJStringByteVarAssigned::SetStorageWithData(uint8_t* p, size_t size, size_t len) {
	m_sSize=size;
	SetHeadAndLength(p, len);
	return *this;
}
CRJStringByteVarAssigned& CRJStringByteVarAssigned::SetStorageWithData(uint8_t* p, size_t s) {
	m_sSize=s;
	SetHeadAndLength(p, strlen((char*) p));
	return *this;
}
CRJStringByteVarAssigned& CRJStringByteVarAssigned::Move(size_t s) {
	m_pHead += s;
	m_sLength -= s;
	m_sSize -= s;
	return *this;
}
CRJStringByteVarAssigned& CRJStringByteVarAssigned::Back(size_t s) {
	m_pHead -= s;
	m_sLength += s;
	m_sSize += s;
	return *this;
}
CRJStringByteVarAssigned& CRJStringByteVarAssigned::Fill(uint8_t ch) {
	_MEMSET_BYTE_POINTER(GetHeadForUpdate(), ch, m_sSize);
	return *this;
}
void CRJStringByteVarAssigned::InitBuffer(uint8_t* p, size_t s) {
	m_sSize=s;
	SetHeadAndLength(p, 0);
}
CRJStringByteVarAssigned& CRJStringByteVarAssigned::SetStorage(uint8_t* p, size_t s) {
	InitBuffer(p, s);
	return *this;
}


CRJStringByteVarAssignedValue::CRJStringByteVarAssignedValue(uint8_t* p, size_t lSize, size_t lLen):
	CRJStringByteVarAssigned(lSize) {
	SetHeadAndLength(p, lLen);
}
CRJStringByteVarAssignedValue::CRJStringByteVarAssignedValue(const CRJStringByteVar& s):
	CRJStringByteVarAssigned(s.GetSize()) {
	SetHeadAndLength(s.GetHeadForRead(), s.GetLength());
}
CRJStringByteVarAssignedValue::CRJStringByteVarAssignedValue(uint8_t* p, size_t s):
	CRJStringByteVarAssigned(s) {
	p[s-1]=0x00;
	SetHeadAndLength(p, strlen((char*) p));
}

