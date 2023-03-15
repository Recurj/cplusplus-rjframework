#include "stdafx.h"
#include <local/rj/libs/base/datatools/byteblockvar.h>
using namespace rjBase;
CRJBlockByteVar::CRJBlockByteVar() {
	setLength();
}
CRJBlockByteVar::CRJBlockByteVar(const CRJBlockByte& v):
	CRJBlockByte(v) {
	setLength();
}
CRJBlockByteVar::CRJBlockByteVar(const CRJStringByte& s):
	CRJBlockByte(s) {
	setLength();
}
CRJBlockByteVar::CRJBlockByteVar(const uint8_t* p, size_t s, size_t u):
	CRJBlockByte(p, s) {
	setLength(u);
}
CRJBlockByteVar& CRJBlockByteVar::Replace(const uint8_t* p, size_t s, size_t u) {
	Set(p, s);
	setLength(u);
	return *this;
}
CRJBlockByteVar& CRJBlockByteVar::Replace(const CRJBlockByte& b, size_t u) {
	Set(b);
	setLength(u);
	return *this;
}
CRJBlockByteVar& CRJBlockByteVar::Replace(const CRJStringByte& s) {
	Set(s);
	setLength(s.GetLength());
	return *this;
}

void CRJBlockByteVar::reset() {
	UnDefined();
	setLength(0);
}
CRJBlockByte& CRJBlockByteVar::GetBlockReadByLength(CRJBlockByte& st) {
	return st.Set(m_pBlock, m_sLength);
}
void CRJBlockByteVar::setLength(size_t u) {
	m_sLength=u;
}
CRJBlockByteVar& CRJBlockByteVar::clear() {
	::memset((void*) m_pBlock, 0x00, m_sSize);
	return *this;
}
void CRJBlockByteVar::WriteChecked(uint8_t ch) {
	((uint8_t*) m_pBlock)[m_sLength]=ch;
	m_sLength++;
}

