#include "stdafx.h"
#include <local/rj/libs/storageos/rjregistrycopy.h>
 typedef CSeptadaAtomRegistryScannerKeys<CRJWinRegistryCopy>CSeptadaRegistryScannerKeys;
typedef CSeptadaAtomRegistryScannerValues<CRJWinRegistryCopy>CSeptadaRegistryScannerValues;
CRJWinRegistryCopy::CRJWinRegistryCopy(int32_t lMode):
	m_clKeySrc(lMode),
m_clKeyDst(lMode) {}
CRJWinRegistryCopy::~CRJWinRegistryCopy() {}
bool CRJWinRegistryCopy::OnScanKeys(CSeptadaAtomRegistry& clPart, CRJStringByte& clKey,void*) {
	CRJWinRegistryCopy clCopy(clPart.GetKeyPart());
	return clCopy.Do(m_clKeySrc, clKey, m_clKeyDst, clKey);
}
bool CRJWinRegistryCopy::OnScanValues(CSeptadaAtomRegistry&, CRJStringByte& clName, CRJStringByte& clValue, int32_t dw,void*) {
	switch (dw) {
	case REG_DWORD:
	case REG_DWORD_BIG_ENDIAN:clValue.SetLengthSafed(4);
		break;
	case REG_QWORD:clValue.SetLengthSafed(8);
		break;
	}
	return m_clKeyDst.Write(clName, clValue,(uint32_t) dw);
}
bool CRJWinRegistryCopy::Do(CSeptadaAtomRegistry& clSrcParent, CRJStringByte& clSrcKey, CSeptadaAtomRegistry& clDstParent, CRJStringByte& clDstKey) {
	try {
		clSrcKey.UpCase();
		clDstKey.UpCase();
		if ((clDstKey.GetLength())&&
			(clSrcKey.GetLength())&&
			(m_clKeyDst.Create(clDstParent, clDstKey))&&
			(m_clKeySrc.OpenEnum(clSrcParent, clSrcKey))) {
			CSeptadaRegistryScannerKeys clKeys(*this,& CRJWinRegistryCopy::OnScanKeys);
			CSeptadaRegistryScannerValues clValues(*this,& CRJWinRegistryCopy::OnScanValues);
			if (m_clKeySrc.ForEachKey(clKeys)) return m_clKeySrc.ForEachValue(clValues);
		}
	}
	catch(...) {}
	return false;
}

