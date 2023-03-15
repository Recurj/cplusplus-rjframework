#include "stdafx.h"
#include <recurj/rjframework/libs/base/os/hlibrary.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <recurj/rjframework/rjsecuritybox.h>
#include <recurj/rjframework/libs/base/application/application.h>
#include <recurj/rjframework/libs/base/datatools/bytepacker.h>
#include <recurj/rjframework/errorcodes.h>
using namespace rjBase;
#ifdef _rjOS_FAMILY_UNIX
CRJStringUnicode RJLibrary::s_libExt(".a");
#endif 
#ifdef _rjOS_WIN
CRJStringUnicode RJLibrary::s_libExt(L".dll");
#endif 
RJLibrary::RJLibrary():
	CRJResult(DataResutRegionLibrary) {
	m_hLibrary=nullptr;
}
RJLibrary::~RJLibrary() {
	Release();
}
RJLibrary::RJLibrary(RJLibrary&&clLib) {
	Swap(std::move(clLib));
}
RJLibrary& RJLibrary::operator=(RJLibrary&&clLib) {
	if (& clLib!=this) Swap(std::move(clLib));
	return *this;
}
void RJLibrary::Swap(RJLibrary&&clLibrary) {
	Attach(clLibrary.GetHandle());
	clLibrary.Detach();
}
void RJLibrary::Release() {
	try {
		if (m_hLibrary) {
#ifdef _rjOS_FAMILY_UNIX
			::dlclose(m_hLibrary);
#endif 
#ifdef _rjOS_WIN
			::FreeLibrary(m_hLibrary);
#endif 
			m_hLibrary=nullptr;
		}
		else Result(eHLibraryErrors::osLibraryNotLoaded);
	}
	catch(...) {}
}
bool RJLibrary::Load(const CRJFileName& clLibPath) {
	try {
		CRJStringExpanded fn;
		fn.Set(clLibPath);
		if (fn.GetLength()>0) {
			CRJStringSymbol szPath[256];
			if (fn.GetC(szPath,sizeof(szPath))) {
#ifdef _rjOS_FAMILY_UNIX
				m_hLibrary=::dlopen((const char*) szPath, RTLD_LOCAL| RTLD_LAZY);
#endif 
#ifdef _rjOS_WIN
				m_hLibrary=::LoadLibraryW((const wchar_t*) szPath);
#endif 
				if (m_hLibrary) return true;
				else Result(eHLibraryErrors::osLibraryNotLoaded);
			}
		}
	}
	catch(...) {}
	return false;
}
void* RJLibrary::GetInterface(const CRJStringByte& clInterfaceName) {
	void* rc=nullptr;
	try {
		if (!clInterfaceName.GetLength()) Result(eHLibraryErrors::osLibraryInterfaceEmpty);
		else if (!m_hLibrary) Result(eHLibraryErrors::osLibraryNotLoaded);
		else {
			uint8_t szName[256];
			if (clInterfaceName.GetC(szName,sizeof(szName))) {
#ifdef _rjOS_FAMILY_UNIX
				rc=::dlsym(m_hLibrary,(const char*) szName);
#endif 
#ifdef _rjOS_WIN
				rc=(void*)::GetProcAddress(m_hLibrary,(const char*) szName);
#endif 
				if (!rc) {
					CRJLoggerThread::ErrorOs(EErrorOs::AppErrDllLoadInteface, clInterfaceName);
					Result(eHLibraryErrors::osLibraryInterfaceNotFound);
				}
			}
		}
	}
	catch(...) {}
	return rc;
}
CRJFileName& RJLibrary::AddLibExt(CRJFileName& fn) {
	return (CRJFileName&) fn.Cat(RJLibrary::s_libExt);
}
bool RJLibrary::ReFormat(CRJStringByteExpanded& src) {
	size_t l=src.GetLength();
	if ((l& 1)==0) {
		CRJStringByteDefault v;
		CRJStringExpanded s;
		CRJFileName ufn;
		RJLibrary clLib;
		CRJStringSymbol* pLib=s.GetHeadForUpdate();
		uint8_t* pVal=v.GetHeadForUpdate();
		SECURITY_BOX_LIB(pLib);
		CRJApplication::Current().AppGetPath(ufn).Cat(s.SetLength(SIZE_SECURITY_BOX_LIB));
		SECURITY_BOX_DECRYPT(pVal);
		l>>=1;
		CRJBytePacker::PackMachine(src.GetHeadForUpdate(), src.GetHeadForRead(), l);
		if (clLib.Load(ufn)) {
			funcDecryptor cl=(funcDecryptor) clLib.GetInterface(v.SetLength(SIZE_SECURITY_BOX_DECRYPT));
			if (cl) {
				src.SetLength(l);
				return cl(src);
			}
		}
	}
	return false;
}

