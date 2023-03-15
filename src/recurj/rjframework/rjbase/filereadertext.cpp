#include "stdafx.h"
#include <recurj/rjframework/libs/base/file/filereadertext.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
using namespace rjBase;
CRJFileReaderText::CRJFileReaderText() {}
CRJFileReaderText::~CRJFileReaderText() {}
bool CRJFileReaderText::Scan(CRJStringByteVar& s,bool bFormat, IRJFileReader& clFunc) {
	while (ReadlnFormat(s, bFormat)) {
		if (!clFunc(s)) return false;
	}
	return true;
}
bool CRJFileReaderText::ReadlnFormat(CRJStringByteVar& clStr,bool bFormat) {
	while (ReadlnData(clStr)) {
		if (!bFormat||
			!clStr.IsFirstThat('#')) return true;
	}
	return false;
}
bool CRJFileReaderText::ReadlnData(CRJStringByteVar& clStr) {
	try {
		if (IsOpened()) {
			uint8_t* p=clStr.GetHeadForUpdate();
			size_t l=clStr.GetSize();
			if (l>1) {
				CRJChainFinderSet clFound;
				if (ReadAvailable(p, l)) {
					clStr.SetLength(l);
					if (clStr.FindPosInSet(CRJStringByte::s_CRLF, clFound)) {
						clStr.SetLengthSafed(clFound.m_sFound);
						if ((clFound.m_sFound<(l-1))&&
							(clStr[clFound.m_sFound]==0x0d)&&
							(clStr[clFound.m_sFound+1]==0x0A)) clFound.m_sFound++;
						OffsetCurrentRead((filepos_t)(clFound.m_sFound+1-l));
					}
					return true;
				}
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJFileReaderText::OffsetCurrentRead(filepos_t k) {
	try {
		if (IsOpened()) {
			return SetPos(k, SEEK_CUR);
		}
	}
	catch(...) {}
	return false;
}

