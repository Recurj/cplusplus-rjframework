#include "stdafx.h"
#include <recurj/rjframework/libs/www/http/defs.h>
using namespace rjWWW;
void CHTTP::CLang::Define(const CRJStringByte& s) {
	if (GetLength()<2) {
		CRJStringByteFlex fs;
		CHTTP::CDataWeighted stTab[4];
		CHTTP::CDataTableWeighted clTab(stTab, 4);
		size_t f=clTab.Parse(s);
		for (size_t i=0; i<f; i++) {
			fs.SetFromData(stTab[i].pData, stTab[i].len);
			if (Known(fs)) return ;
		}
	}
}
bool CHTTP::CLang::Known(const CRJStringByte& clNet) {
	Set(clNet);
	if (GetLength()>2) SetLengthSafed(2);
	return true;
}

