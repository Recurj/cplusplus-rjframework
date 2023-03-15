#include "stdafx.h"
#include <recurj/rjframework/libs/storageos/rjatomregistry.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/errorcodes.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#pragma comment(lib, "Shlwapi.lib")
CRJAtomRegistry::CRJAtomRegistry(size_t sMode) {
	m_sKeyPart=sMode;
	m_hKey=nullptr;
}
CRJAtomRegistry::~CRJAtomRegistry()
#pragma warning(suppress: 26447)
 {
	Release();
}
HKEY CRJAtomRegistry::GetModeKey() {
	switch (m_sKeyPart) {
	case REGISTRY_KEY_CURRENT_USER: return HKEY_CURRENT_USER;
		break;
	}
	return HKEY_LOCAL_MACHINE;
}
void CRJAtomRegistry::Reset(size_t lMode) {
	Release();
	m_sKeyPart=lMode;
}
void CRJAtomRegistry::Release() {
	try {
		if (m_hKey) {
			int32_t rc=::RegCloseKey(m_hKey);
			if (rc==ERROR_SUCCESS) {
				m_hKey=nullptr;
			}
			else CRJDataTools::DebugBreak();
		}
	}
	catch(...) {}
}
bool CRJAtomRegistry::CreateC(const CRJStringByte& clPath) {
	try {
		DWORD dw;
		Release();
		if (!::RegCreateKeyEx(GetModeKey(), clPath.GetHeadForReadC(), 0,nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,nullptr,& m_hKey,& dw)) return true;
		else CRJLoggerThread::ErrorOs(EErrorOs::AppErrRegistryCreate, clPath);
	}
	catch(...) {}
	return false;
}
bool CRJAtomRegistry::CreateC(const CRJAtomRegistry& clSrc,const CRJStringByte& clPath) {
	try {
		DWORD dw;
		Release();
		if (!::RegCreateKeyEx(clSrc.GetHandle(), clPath.GetHeadForReadC(), 0,nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,nullptr,& m_hKey,& dw)) return true;
		else CRJLoggerThread::ErrorOs(EErrorOs::AppErrRegistryCreate, clPath);
	}
	catch(...) {}
	return false;
}
bool CRJAtomRegistry::OpenReadC(const CRJStringByte& clPath) {
	try {
		Release();
		if (!::RegOpenKeyEx(GetModeKey(), clPath.GetHeadForReadC(), 0, KEY_READ,& m_hKey)) return true;
		else CRJLoggerThread::ErrorOs(EErrorOs::AppErrRegistryOpen, clPath);
	}
	catch(...) {}
	return false;
}
bool CRJAtomRegistry::OpenEnumC(const CRJStringByte& clPath) {
	try {
		Release();
		if (!::RegOpenKeyEx(GetModeKey(), clPath.GetHeadForReadC(), 0, KEY_ENUMERATE_SUB_KEYS| KEY_READ,& m_hKey)) return true;
		else CRJLoggerThread::ErrorOs(EErrorOs::AppErrRegistryOpen, clPath);
	}
	catch(...) {}
	return false;
}
bool CRJAtomRegistry::OpenAllC(const CRJStringByte& clPath) {
	try {
		Release();
		if (!::RegOpenKeyEx(GetModeKey(), clPath.GetHeadForReadC(), 0, KEY_ALL_ACCESS,& m_hKey)) return true;
		else CRJLoggerThread::ErrorOs(EErrorOs::AppErrRegistryOpen, clPath);
	}
	catch(...) {}
	return false;
}
bool CRJAtomRegistry::OpenReadC(const CRJAtomRegistry& cl,const CRJStringByte& clPath) {
	try {
		Release();
		if (!::RegOpenKeyEx(cl.GetHandle(), clPath.GetHeadForReadC(), 0, KEY_READ,& m_hKey)) return true;
		else CRJLoggerThread::ErrorOs(EErrorOs::AppErrRegistryOpen, clPath);
	}
	catch(...) {}
	return false;
}
bool CRJAtomRegistry::OpenEnumC(const CRJAtomRegistry& cl,const CRJStringByte& clPath) {
	try {
		Release();
		if (!::RegOpenKeyEx(cl.GetHandle(), clPath.GetHeadForReadC(), 0, KEY_ENUMERATE_SUB_KEYS| KEY_READ,& m_hKey)) return true;
		else CRJLoggerThread::ErrorOs(EErrorOs::AppErrRegistryEnum, clPath);
	}
	catch(...) {}
	return false;
}
bool CRJAtomRegistry::OpenAllC(const CRJAtomRegistry& cl,const CRJStringByte& clPath) {
	try {
		Release();
		if (!::RegOpenKeyEx(cl.GetHandle(), clPath.GetHeadForReadC(), 0, KEY_ALL_ACCESS,& m_hKey)) return true;
		else CRJLoggerThread::ErrorOs(EErrorOs::AppErrRegistryEnum, clPath);
	}
	catch(...) {}
	return false;
}
bool CRJAtomRegistry::ForEachKey(IRJRegistryKeyBool& cb) {
	try {
		if (m_hKey) {
			FILETIME stElemDTime;
			DWORD dw=0;
			CRJStringByteDefault str;
			uint32_t dwEnum=0;
			int32_t v=0;
			while (!v) {
				dw=(DWORD) str.GetSize()-2;
				v=::RegEnumKeyEx(m_hKey, dwEnum,(char*) str.GetHeadForUpdate(),& dw,nullptr,nullptr,nullptr,& stElemDTime);
				if (v)break;
				dwEnum++;
				if (dw>0) {
					str.SetLength(dw);
					if (!cb(str.MakeCLabel())) return false;
				}
			}
		}
	}
	catch(...) {}
	return true;
}
bool CRJAtomRegistry::ForEachValue(IRJRegistryValueBool& cb) {
	try {
		if (m_hKey) {
			CRJStringByteLong clValue;
			CRJStringByteDefault clName;
			DWORD dwElemType, dwNameLen, dwDataLen;
			uint32_t dwEnum=0;
			int32_t v=0;
			while (!v) {
				dwNameLen=(DWORD) clName.GetSize()-2;
				dwDataLen=(DWORD) clValue.GetSize()-2;
				v=::RegEnumValue(m_hKey, dwEnum,(char*) clName.GetHeadForUpdate(),& dwNameLen,nullptr,& dwElemType,(LPBYTE) clValue.GetHeadForUpdate(),& dwDataLen);
				if (v)break;
				dwEnum++;
				if (dwNameLen>0&&
					dwElemType==REG_SZ) {
					clName.SetLength(dwNameLen);
					if (dwElemType==REG_SZ) {
						clValue.SetLength(dwDataLen);
						if (!cb(clName.MakeCLabel(), clValue.MakeCLabel())) return false;
					}
				}
			}
		}
	}
	catch(...) {}
	return true;
}
bool CRJAtomRegistry::WriteC(const CRJStringByte& clPath,const CRJStringByte& clValue) {
	try {
		if (m_hKey) {
			if (!::RegSetValueEx(m_hKey, clPath.GetHeadForReadC(), 0, REG_SZ,(LPBYTE) clValue.GetHeadForReadC(),(uint32_t) clValue.GetLength())) return true;
			else CRJLoggerThread::ErrorOs(EErrorOs::AppErrRegistryWrite, clPath);
		}
	}
	catch(...) {}
	return false;
}
bool CRJAtomRegistry::ReadC(const CRJStringByte& clPath, CRJStringByteVar& clValue) {
	try {
		clValue.SetEmpty();
		if (m_hKey) {
			size_t lSize=clValue.GetSize();
			if (lSize>0) {
				DWORD dwLen, dwElemType;
				dwLen=(DWORD) lSize-1;
				dwElemType=REG_SZ;
				if (!::RegQueryValueEx(m_hKey, clPath.GetHeadForReadC(),nullptr,& dwElemType,(LPBYTE) clValue.GetHeadForUpdate(),& dwLen)) {
					if (dwLen>0&&
						dwElemType==REG_SZ) {
						clValue.SetLength(dwLen).TrimRight(0x00);
						return true;
					}
				}
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJAtomRegistry::DropValueC(const CRJStringByte& clPath) {
	try {
		if (m_hKey) {
			if (!::RegDeleteValue(m_hKey, clPath.GetHeadForReadC())) return true;
			else CRJLoggerThread::ErrorOs(EErrorOs::AppErrRegistryDropValue, clPath);
		}
	}
	catch(...) {}
	return false;
}
bool CRJAtomRegistry::DropKeyC(const CRJStringByte& clPath) {
	try {
		if (m_hKey) {
			if (!::SHDeleteKey(m_hKey, clPath.GetHeadForReadC())) return true;
			else CRJLoggerThread::ErrorOs(EErrorOs::AppErrRegistryDropKey, clPath);
		}
	}
	catch(...) {}
	return false;
}
void CRJAtomRegistry::AttachHandle(HKEY h) {
	m_hKey=h;
}
void CRJAtomRegistry::DetachHandle() {
	m_hKey=nullptr;
}
bool CRJAtomRegistry::IsExistC(size_t lPart,const CRJStringByte& clPath) {
	CRJAtomRegistry clCheck(lPart);
	return clCheck.OpenReadC(clPath);
}
bool CRJAtomRegistry::CheckKeyC(const CRJStringByte& clPath) {
	CRJAtomRegistry clCheck(m_sKeyPart);
	return clCheck.OpenReadC(*this, clPath);
}
bool CRJAtomRegistry::RenameKey(const CRJStringByte& clOld,const CRJStringByte& clNew) {
	try {
		CRJStringByteDefault clOldName(clOld),
		clNewName(clNew);
		clOldName.UpperCase().MakeCLabel();
		clNewName.UpperCase().MakeCLabel();
		if (clOldName!=clNewName&&
			CheckKeyC(clOldName)) {
			CRJAtomRegistry clDst(m_sKeyPart);
			if (clDst.CreateC(*this, clNewName)) {
				return ::RegCopyTree(m_hKey, clOldName.GetHeadForReadC(), clDst.GetHandle())==ERROR_SUCCESS;
			}
		}
	}
	catch(...) {}
	return false;
}
CRJAtomRegistryCached::CRJAtomRegistryCached():
	CRJAtomRegistry(REGISTRY_KEY_DUMMY) {}
CRJAtomRegistryCached::~CRJAtomRegistryCached() {}
bool CRJAtomRegistryCached::Get(size_t sPart,const CRJStringByte& path,const CRJStringByte& name, CRJStringByteVar& result) {
	if (m_sKeyPart!=sPart||
		path!=m_clPathLast) {
		if (!m_clPathLast.HasNeededSize(path.GetLength()+1)) return false;
		m_sKeyPart=sPart;
		m_clPathLast.Set(path).MakeCLabel();
		if (!OpenReadC(m_clPathLast)) return false;
	}
	return ReadC(name, result);
}

