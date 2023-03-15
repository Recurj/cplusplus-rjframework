#include "stdafx.h"
#include <local/rj/libs/storageos/rjregistryscanner.h>
#include <local/rj/libs/storageos/rjatomregistry.h>
 typedef CSeptadaAtomRegistryScannerKeys<CRJWinRegistryScanner>CSeptadaRegistryScannerKeys;
typedef CSeptadaAtomRegistryScannerValues<CRJWinRegistryScanner>CSeptadaRegistryScannerValues;
CRJWinRegistryScanner::CRJWinRegistryScanner() {}
CRJWinRegistryScanner::~CRJWinRegistryScanner() {}
void CRJWinRegistryScanner::Scan(const CRJStringByte& clRoot,const CRJStringByte& clPath) {
	try {
		CRJAtomRegistry clKey(REGISTRY_KEY_LOCAL_MACHINE);
		if (clKey.OpenEnumC(m_clCurrPath.Set(clRoot).CatSingle('\\').Cat(clPath).MakeCLabel())) {
			m_clRegistry.SetRoot(m_clCurrPath);
			m_clCurrPath.SetEmpty();
			Scan(clKey);
		}
	}
	catch(...) {}
}
bool CRJWinRegistryScanner::OnScanKeys(CRJAtomRegistry& clPart, CRJStringByte& clKey,void*) {
	CRJAtomRegistry clDst(REGISTRY_KEY_LOCAL_MACHINE);
	if (clDst.OpenEnum(clPart, clKey)) {
		int32_t l=m_clCurrPath.GetLength();
		if (l>0) m_clCurrPath.CatChar('\\');
		m_clCurrPath.Cat(clKey);
		Scan(clDst);
		m_clCurrPath.SetLengthWithTruncate(l);
	}
	return true;
}
bool CRJWinRegistryScanner::OnScanValues(CRJAtomRegistry&, CRJStringByte& clName, CRJStringByte& clValue, int32_t,void*) {
	if (clName.GetAsciiAtPosSafed(0)!='_') OnFound(m_clCurrPath, clName, clValue);
	return true;
}
void CRJWinRegistryScanner::Scan(CRJAtomRegistry& clKey) {
	try {
		CSeptadaRegistryScannerKeys clKeys(*this,& CRJWinRegistryScanner::OnScanKeys);
		CSeptadaRegistryScannerValues clValues(*this,& CRJWinRegistryScanner::OnScanValues);
		clKey.ForEachKey(clKeys);
		clKey.ForEachValue(clValues);
	}
	catch(...) {}
}
bool CRJWinRegistryScanner::OnFound(const CRJStringByte& str,const CRJStringByte& clName,const CRJStringByte& clValue) {
	try {
		CSeptadaStringDefault n, s;
		s.SetDefaultChar('_').Cat(clName);
		m_clRegistry.Get(str, s, n);
		_SZCOPY_STRING(m_stRecord.szName, clName);
		_SZCOPY_STRING(m_stRecord.szPath, str);
		_SZCOPY_STRING(m_stRecord.szNote, n);
		_SZCOPY_STRING(m_stRecord.szParam, clValue);
		return OnReady();
	}
	catch(...) {}
	return true;
}
bool CRJWinRegistryScanner::OnReady() {
	return true;
}

