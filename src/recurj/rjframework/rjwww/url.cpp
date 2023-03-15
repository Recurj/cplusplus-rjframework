#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
#include <recurj/rjframework/libs/www/url.h>
using namespace rjWWW;
CRJURL::CRJURL(){}
CRJURL::~CRJURL() {}
CRJStringByteVar& CRJURL::Decode() {
	CRJChainFinder clFinder;
	size_t i, j, l, s;
	bool b;
	uint8_t* pData=GetHeadForUpdate();
	Replace('+',' ');
	b=FindPos('%', clFinder);
	while (b) {
		i=clFinder.m_sFound;
		l=GetLength();
		s=GetLength()-i;
		if (s>2) {
			j=i+1;
			pData[i]=CRJByteConvertor::PairHex2Byte((uint8_t) pData[j],(uint8_t) pData[j+1]);
			if (s>3) _MEMMOVE_BYTE_POINTER(& pData[j],& pData[j+2], s-3);
			SetLengthSafed(l-2);
		}
		b=FindPosNext('%', clFinder);
	}
	return *this;
}

