#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/modules/defs/moddefstorageos.h>
using namespace rjBase;
constexpr size_t REGISTRY_KEY_DUMMY=0;
constexpr size_t REGISTRY_KEY_LOCAL_MACHINE=1;
constexpr size_t REGISTRY_KEY_CURRENT_USER=2;
using IRJRegistryKeyBool=IRJFunctorBool<CRJStringByteVar&>;
using IRJRegistryValueBool=IRJFunctorBool<CRJStringByteVar&, CRJStringByteVar&>;
template<typename TFunc>
using CRJRegistryKeyFunctorBool=CRJFunctorBool<IRJRegistryKeyBool, TFunc, CRJStringByteVar&>;
template<typename TFunc>
using CRJRegistryValueFunctorBool=CRJFunctorBool<IRJRegistryValueBool, TFunc, CRJStringByteVar&,
CRJStringByteVar&>;
class RJSTORAGEOS_API CRJAtomRegistry {
public:
	CRJAtomRegistry(size_t);
	virtual~CRJAtomRegistry();
	virtual void Reset(size_t);
	bool CreateC(const CRJStringByte&);
	bool CreateC(const CRJAtomRegistry&,const CRJStringByte&);
	bool OpenReadC(const CRJStringByte&);
	bool OpenEnumC(const CRJStringByte&);
	bool OpenAllC(const CRJStringByte&);
	bool OpenReadC(const CRJAtomRegistry&,const CRJStringByte&);
	bool OpenEnumC(const CRJAtomRegistry&,const CRJStringByte&);
	bool OpenAllC(const CRJAtomRegistry&,const CRJStringByte&);
	bool ForEachKey(IRJRegistryKeyBool&);
	bool ForEachValue(IRJRegistryValueBool&);
	bool WriteC(const CRJStringByte&,const CRJStringByte&);
	bool ReadC(const CRJStringByte&, CRJStringByteVar&);
	bool DropValueC(const CRJStringByte&);
	bool DropKeyC(const CRJStringByte&);
	bool CheckKeyC(const CRJStringByte&);
	void Release();
	bool RenameKey(const CRJStringByte&,const CRJStringByte&);
	inline size_t GetKeyPart() const {
		return m_sKeyPart;
	}
	inline HKEY GetHandle() const {
		return m_hKey;
	}
	static bool IsExistC(size_t,const CRJStringByte&);
protected:
	HKEY GetModeKey();
	void AttachHandle(HKEY);
	void DetachHandle();
private:
	_CLASS_COPY_AND_ASSIGN_DELETE(CRJAtomRegistry)
protected:
	HKEY m_hKey;
	size_t m_sKeyPart;
};
class RJSTORAGEOS_API CRJAtomRegistryCached:public CRJAtomRegistry {
public:
	CRJAtomRegistryCached();
	virtual~CRJAtomRegistryCached();
	bool Get(size_t,const CRJStringByte&,const CRJStringByte&, CRJStringByteVar&);
protected:
	CRJStringByteExpanded m_clPathLast;
};

