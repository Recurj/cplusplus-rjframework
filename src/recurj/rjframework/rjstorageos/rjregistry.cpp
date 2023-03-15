#include "stdafx.h"
#include <recurj/rjframework/libs/storageos/rjregistry.h>
#include <recurj/rjframework/libs/base/application/application.h>
CRJStringByte CRJRegistry::s_clRegPathOpened("SOFTWARE\\RECURJ");
CRJRegistry::CRJRegistryPath::CRJRegistryPath(CRJStringByteVar& s):
	m_clRoot(s),
m_sPathLen(s.GetLength()) {}
CRJRegistry::CRJRegistryPath::~CRJRegistryPath() {
	m_clRoot.SetLengthSafed(m_sPathLen);
}
bool CRJRegistry::CRJRegistryPath::SetPath(const CRJStringByte& clPath) {
	return m_clRoot.CatLabel("\\", 1).Cat(clPath).OK();
}
bool CRJRegistry::CRJRegistryPath::SetSection(const CRJStringByte& clSect) {
	CRJChainFinder f;
	if (clSect.FindPosBack('\\', f)) {
		size_t p=f.m_sFound+1;
		m_clSection.SetLabel(clSect.GetHeadForReadC()+p, clSect.GetLength()-p);
		m_clRoot.CatSingle('\\').CatLabel(clSect.GetHeadForReadC(), f.m_sFound);
	}
	else m_clSection.Set(clSect);
	return m_clSection.MakeCLabel().OK();
}
CRJRegistry::CRJRegistry():
	m_clRoot(CRJRegistry::s_clRegPathOpened),
m_sKeyPart(REGISTRY_KEY_LOCAL_MACHINE) {}
CRJRegistry::CRJRegistry(const CRJStringByte& path):
	m_clRoot(CRJRegistry::s_clRegPathOpened),
m_sKeyPart(REGISTRY_KEY_LOCAL_MACHINE) {
	addPath(path);
}
CRJRegistry::CRJRegistry(const CRJStringByte& path,const CRJStringByte& clRoot):
	m_clRoot(clRoot),
m_sKeyPart(REGISTRY_KEY_LOCAL_MACHINE) {
	addPath(path);
}
CRJRegistry::~CRJRegistry() {}
bool CRJRegistry::SetPath(const CRJStringByte& s) {
	m_clRoot.Set(s);
	if (!m_clRoot.GetLength()) m_clRoot.Set(CRJRegistry::s_clRegPathOpened);
	else if (m_clRoot.IsLastThat('\\')) m_clRoot.TruncRight(1);
	return m_clRoot.OK();
}
void CRJRegistry::SetKeyPart(size_t sPart) {
	m_sKeyPart=sPart;
}
bool CRJRegistry::Put(const CRJStringByte& clSect,const CRJStringByte& val) {
	CRJStringByteLong v(val);
	CRJRegistryPath path(m_clRoot);
	return path.SetSection(clSect)&&
		putc(path.m_clSection, v.MakeCLabel());
}
bool CRJRegistry::Put(const CRJStringByte& clPath,const CRJStringByte& clSect,const CRJStringByte& val) {
	CRJStringByteLong v(val);
	CRJRegistryPath path(m_clRoot);
	return path.SetPath(clPath)&&
		path.SetSection(clSect)&&
		putc(path.m_clSection, v.MakeCLabel());
}
bool CRJRegistry::PutInt64(const CRJStringByte& clSect, int64_t val) {
	CRJStringByteShort s;
	CRJRegistryPath path(m_clRoot);
	return path.SetSection(clSect)&&
		putc(path.m_clSection, s.SetInt64(val).MakeCLabel());
}
bool CRJRegistry::PutInt64(const CRJStringByte& clPath,const CRJStringByte& clSect, int64_t val) {
	CRJStringByteShort s;
	CRJRegistryPath path(m_clRoot);
	return path.SetPath(clPath)&&
		path.SetSection(clSect)&&
		putc(path.m_clSection, s.SetInt64(val).MakeCLabel());
}
bool CRJRegistry::PutInt32(const CRJStringByte& clSect, int32_t val) {
	CRJStringByteShort s;
	CRJRegistryPath path(m_clRoot);
	return path.SetSection(clSect)&&
		putc(path.m_clSection, s.SetInt64(val).MakeCLabel());
}
bool CRJRegistry::PutInt32(const CRJStringByte& clPath,const CRJStringByte& clSect, int32_t val) {
	CRJStringByteShort s;
	CRJRegistryPath path(m_clRoot);
	return path.SetPath(clPath)&&
		path.SetSection(clSect)&&
		putc(path.m_clSection, s.SetInt64(val).MakeCLabel());
}
bool CRJRegistry::PutSize(const CRJStringByte& clSect, size_t val) {
	CRJStringByteShort s;
	CRJRegistryPath path(m_clRoot);
	return path.SetSection(clSect)&&
		putc(path.m_clSection, s.SetSize(val).MakeCLabel());
}
bool CRJRegistry::PutSize(const CRJStringByte& clPath,const CRJStringByte& clSect, size_t val) {
	CRJStringByteShort s;
	CRJRegistryPath path(m_clRoot);
	return path.SetPath(clPath)&&
		path.SetSection(clSect)&&
		putc(path.m_clSection, s.SetSize(val).MakeCLabel());
}
bool CRJRegistry::PutBool(const CRJStringByte& clSect,bool b) {
	CRJStringByteShort s;
	CRJRegistryPath path(m_clRoot);
	return path.SetSection(clSect)&&
		putc(path.m_clSection, s.SetSingle(b?
		'1':'0').MakeCLabel());
}
bool CRJRegistry::PutBool(const CRJStringByte& clPath,const CRJStringByte& clSect,bool b) {
	CRJStringByteShort s;
	CRJRegistryPath path(m_clRoot);
	return path.SetPath(clPath)&&
		path.SetSection(clSect)&&
		putc(path.m_clSection, s.SetSingle(b?
		'1':'0').MakeCLabel());
}
bool CRJRegistry::Get(const CRJStringByte& clSect, CRJStringByteVar& val) {
	CRJRegistryPath path(m_clRoot);
	return (path.SetSection(clSect))?
		get(path.m_clSection, val):
		false;
}
bool CRJRegistry::Get(const CRJStringByte& clPath,const CRJStringByte& clSect, CRJStringByteVar& val) {
	CRJRegistryPath path(m_clRoot);
	return (path.SetPath(clPath)&&
		path.SetSection(clSect))?
		get(path.m_clSection, val):
		false;
}
bool CRJRegistry::GetInt64(const CRJStringByte& clPath,const CRJStringByte& clSect, int64_t& val) {
	CRJStringByteShort s;
	CRJRegistryPath path(m_clRoot);
	return path.SetPath(clPath)&&
		path.SetSection(clSect)&&
		get(path.m_clSection, s)&&
		s.IsValidInt64(val);
}
int64_t CRJRegistry::GetInt64Default(const CRJStringByte& clPath,const CRJStringByte& clSect, int64_t def) {
	int64_t v;
	return GetInt64(clPath, clSect, v)?
		v:def;
}
bool CRJRegistry::GetInt32(const CRJStringByte& clPath,const CRJStringByte& clSect, int32_t& val) {
	CRJStringByteShort s;
	int64_t v;
	CRJRegistryPath path(m_clRoot);
	if (path.SetPath(clPath)&&
		path.SetSection(clSect)&&
		get(path.m_clSection, s)&&
		s.IsValidInt64(v)) {
		val=(int32_t) v;
		return true;
	}
	return false;
}
bool CRJRegistry::GetBool(const CRJStringByte& clSect,bool& val) {
	CRJStringByteShort s;
	CRJRegistryPath path(m_clRoot);
	return path.SetSection(clSect)&&
		get(path.m_clSection, s)&&
		s.IsValidBool(val);
}
bool CRJRegistry::GetBoolDefault(const CRJStringByte& clSect,bool def) {
	bool b;
	return GetBool(clSect, b)?
		b:def;
}
bool CRJRegistry::GetBool(const CRJStringByte& clPath,const CRJStringByte& clSect,bool& val) {
	CRJStringByteShort s;
	CRJRegistryPath path(m_clRoot);
	return path.SetPath(clPath)&&
		path.SetSection(clSect)&&
		get(path.m_clSection, s)&&
		s.IsValidBool(val);
}
bool CRJRegistry::GetBoolDefault(const CRJStringByte& clPath,const CRJStringByte& clSect,bool def) {
	bool b;
	return GetBool(clPath, clSect, b)?
		b:def;
}
bool CRJRegistry::GetSize(const CRJStringByte& clSect, size_t& val) {
	CRJStringByteShort s;
	CRJRegistryPath path(m_clRoot);
	return path.SetSection(clSect)&&
		get(path.m_clSection, s)&&
		s.IsValidSize(val);
}
size_t CRJRegistry::GetSizeDefault(const CRJStringByte& clSect, size_t def) {
	size_t v;
	return GetSize(clSect, v)?
		v:def;
}
bool CRJRegistry::GetSize(const CRJStringByte& clPath,const CRJStringByte& clSect, size_t& val) {
	CRJStringByteShort s;
	CRJRegistryPath path(m_clRoot);
	return path.SetPath(clPath)&&
		path.SetSection(clSect)&&
		get(path.m_clSection, s)&&
		s.IsValidSize(val);
}
size_t CRJRegistry::GetSizeDefault(const CRJStringByte& clPath,const CRJStringByte& clSect, size_t def) {
	size_t v;
	return GetSize(clPath, clSect, v)?
		v:def;
}
bool CRJRegistry::DelVar(const CRJStringByte& clPath,const CRJStringByte& clSect) {
	CRJRegistryPath path(m_clRoot);
	return path.SetPath(clPath)&&
		path.SetSection(clSect)&&
		del(path.m_clSection);
}
bool CRJRegistry::DelKey(const CRJStringByte& clPath,const CRJStringByte& clKey) {
	CRJRegistryPath path(m_clRoot);
	return path.SetPath(clPath)&&
		DelKey(clKey);
}
bool CRJRegistry::DelKey(const CRJStringByte& clKeyName) {
	CRJAtomRegistry clKey(m_sKeyPart);
	if (clKey.OpenAllC(m_clRoot.MakeCLabel())) {
		CRJStringByteDefault t(clKeyName);
		return clKey.DropKeyC(t.MakeCLabel());
	}
	return false;
}
bool CRJRegistry::RenameKey(const CRJStringByte& clPath,const CRJStringByte& clOld,const CRJStringByte& clNew,bool bDelete) {
	try {
		CRJRegistryPath path(m_clRoot);
		if (path.SetPath(clPath)) {
			CRJAtomRegistry clKey(m_sKeyPart);
			if (clKey.OpenAllC(m_clRoot.MakeCLabel())) {
				if (clKey.RenameKey(clOld, clNew)) {
					if (bDelete) {
						CRJStringByteDefault t(clOld);
						clKey.DropKeyC(t.MakeCLabel());
					}
					return true;
				}
			}
		}
	}
	catch(...) {}
	return false;
}
bool CRJRegistry::RenameKey(const CRJStringByte& clOld,const CRJStringByte& clNew,bool bDelete) {
	return RenameKey(CRJStringByte::GetEmptyString(), clOld, clNew, bDelete);
}
bool CRJRegistry::Rename(const CRJStringByte& clPath,const CRJStringByte& clOldName,const CRJStringByte& clNewName) {
	CRJRegistryPath path(m_clRoot);
	return (path.SetPath(clPath))?
		Rename(clOldName, clNewName):
		false;
}
bool CRJRegistry::Rename(const CRJStringByte& clOldName,const CRJStringByte& clNewName) {
	CRJStringByteDefault s;
	CRJStringByteExpanded val;
	if (get(s.Set(clOldName).MakeCLabel(), val)) {
		if (putc(s.Set(clNewName).MakeCLabel(), val.MakeCLabel())) {
			if (del(s.Set(clOldName).MakeCLabel())) return true;
		}
	}
	return false;
}
bool CRJRegistry::DirectPut(const CRJStringByte& clPath,const CRJStringByte& clSect,const CRJStringByte& str) {
	CRJStringByteDefault s;
	CRJAtomRegistry clKey(m_sKeyPart);
	CRJStringByteLong v(str);
	return clKey.CreateC(m_clRoot.Set(clPath).MakeCLabel())&&
		clKey.WriteC(s.Set(clSect).MakeCLabel(), v.MakeCLabel());
}
bool CRJRegistry::DirectGet(const CRJStringByte& clPath,const CRJStringByte& clSect, CRJStringByteVar& str) {
	CRJStringByteDefault s;
	CRJAtomRegistry clKey(m_sKeyPart);
	return clKey.OpenReadC(m_clRoot.Set(clPath).MakeCLabel())&&
		clKey.ReadC(s.Set(clSect).MakeCLabel(), str);
}
int32_t CRJRegistry::GetInt32Default(const CRJStringByte& clPath,const CRJStringByte& clSect, int32_t def) {
	int32_t v;
	return GetInt32(clPath, clSect, v)?
		v:def;
}
bool CRJRegistry::addPath(const CRJStringByte& name) {
	if (name.GetLength()) {
		if (!m_clRoot.GetLength()) m_clRoot.Set(CRJRegistry::s_clRegPathOpened);
		m_clRoot.CatSingle('\\').Cat(name);
		if (m_clRoot.IsLastThat('\\')) m_clRoot.TruncRight(1);
	}
	return m_clRoot.OK();
}
bool CRJRegistry::putc(const CRJStringByte& section,const CRJStringByte& val) {
	CRJAtomRegistry clKey(m_sKeyPart);
	return clKey.CreateC(m_clRoot.MakeCLabel())&&
		clKey.WriteC(section, val);
}
bool CRJRegistry::get(const CRJStringByte& section, CRJStringByteVar& val) {
	return m_clRead.Get(m_sKeyPart, m_clRoot, section, val.SetEmpty());
}
bool CRJRegistry::del(const CRJStringByte& section) {
	CRJAtomRegistry clKey(m_sKeyPart);
	if (clKey.OpenAllC(m_clRoot.MakeCLabel())) return clKey.DropValueC(section);
	return false;
}

