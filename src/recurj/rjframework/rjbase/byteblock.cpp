#include "stdafx.h"
#include <local/rj/libs/base/datatools/byteblock.h>
#include <local/rj/libs/base/datatools/datatools.h>
using namespace rjBase;
const uint8_t CRJBlockByte::s_undefValue=0;
CRJBlockByte::CRJBlockByte() {
	UnDefined();
}
CRJBlockByte::CRJBlockByte(const uint8_t* p, size_t s) {
#ifdef _DEBUG
	if (!p) CRJDataTools::DebugBreak();
#endif 
	m_pBlock=(uint8_t*) p;
	m_sSize=s;
}
CRJBlockByte::CRJBlockByte(const CRJBlockByte& d) {
	m_pBlock=d.m_pBlock;
	m_sSize=d.m_sSize;
}
CRJBlockByte::CRJBlockByte(const CRJStringByte& s) {
	m_pBlock=s.GetHeadForRead();
	m_sSize=s.GetLength();
}
CRJBlockByte::~CRJBlockByte() {}
bool CRJBlockByteMovable::HasSymbol(uint8_t& ch, size_t c) {
	if (m_sSize>c) {
		ch=m_pBlock[c];
		return true;
	}
	return false;
}

