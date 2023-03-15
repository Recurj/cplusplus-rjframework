#include "stdafx.h"
#include <local/rj/libs/base/strings/byte/constant.h>
using namespace rjBase;
const CRJStringByteConstant CRJStringByteConstant::s_WordYES((uint8_t*)"YES", 3);
const CRJStringByteConstant CRJStringByteConstant::s_WordTRUE((uint8_t*)"TRUE", 4);
const CRJStringByteConstant CRJStringByteConstant::s_WordNO((uint8_t*)"NO", 2);
const CRJStringByteConstant CRJStringByteConstant::s_WordFALSE((uint8_t*)"FALSE", 5);
const CRJStringByteConstant CRJStringByteConstant::s_Word1((uint8_t*)"1", 1);
const CRJStringByteConstant CRJStringByteConstant::s_Word0((uint8_t*)"0", 1);
const CRJStringByteConstant CRJStringByteConstant::s_WordN((uint8_t*)"N", 1);
const CRJStringByteConstant CRJStringByteConstant::s_WordY((uint8_t*)"Y", 1);
const CRJStringByteConstant CRJStringByteConstant::s_clEmptyStringChar((uint8_t*)"", 0);
#ifdef _rjOS_FAMILY_UNIX
 const CRJStringByteConstant CRJStringByteConstant::s_LineNext((uint8_t*)"\x0A", 1);
#endif 
#ifdef _rjOS_WIN
 const CRJStringByteConstant CRJStringByteConstant::s_LineNext((uint8_t*)"\x0D\x0A", 2);
#endif 
 const CRJStringByteConstant CRJStringByteConstant::s_LineFeed((uint8_t*)"\x0A", 1);
const CRJStringByteConstant CRJStringByteConstant::s_CRLF((uint8_t*)"\x0D\x0A", 2);
CRJStringByteConstant::CRJStringByteConstant(const uint8_t* p, size_t lLen):
	CRJStringByte((const uint8_t*) p, lLen) {
	m_lLength=lLen;
}
CRJStringByteConstant::CRJStringByteConstant(const CRJStringByte& str):
	CRJStringByte(str.GetHeadForRead(), str.GetSize()) {
	m_lLength=str.GetLength();
}
CRJStringByteConstantC::CRJStringByteConstantC(const char* p, size_t l):
	m_clOutside((const uint8_t*) p, l) {}
const char* CRJStringByteConstantC::GetC() const {
	return (const char*) m_clOutside.GetHeadForRead();
}

