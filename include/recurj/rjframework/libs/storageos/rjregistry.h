#pragma once
#include <recurj/rjframework/modules/defs/moddefstorageos.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/storageos/rjatomregistry.h>
using namespace rjBase;
class RJSTORAGEOS_API CRJRegistry {
public:
	CRJRegistry(const CRJStringByte&);
	CRJRegistry(const CRJStringByte&,const CRJStringByte&);
	virtual~CRJRegistry();
	void SetKeyPart(size_t);
	bool SetPath(const CRJStringByte&);
	bool Get(const CRJStringByte&, CRJStringByteVar&);
	bool Get(const CRJStringByte&,const CRJStringByte&, CRJStringByteVar&);
	bool GetInt64(const CRJStringByte&,const CRJStringByte&, int64_t&);
	int64_t GetInt64Default(const CRJStringByte&,const CRJStringByte&, int64_t);
	bool GetInt32(const CRJStringByte&,const CRJStringByte&, int32_t&);
	int32_t GetInt32Default(const CRJStringByte&,const CRJStringByte&, int32_t);
	bool GetSize(const CRJStringByte&, size_t&);
	size_t GetSizeDefault(const CRJStringByte&, size_t);
	bool GetSize(const CRJStringByte&,const CRJStringByte&, size_t&);
	size_t GetSizeDefault(const CRJStringByte&,const CRJStringByte&, size_t);
	bool GetBool(const CRJStringByte&,bool&);
	bool GetBoolDefault(const CRJStringByte&,bool);
	bool GetBool(const CRJStringByte&,const CRJStringByte&,bool&);
	bool GetBoolDefault(const CRJStringByte&,const CRJStringByte&,bool);
	bool Put(const CRJStringByte&,const CRJStringByte&);
	bool Put(const CRJStringByte&,const CRJStringByte&,const CRJStringByte&);
	bool PutInt64(const CRJStringByte&, int64_t);
	bool PutInt64(const CRJStringByte&,const CRJStringByte&, int64_t);
	bool PutInt32(const CRJStringByte&, int32_t);
	bool PutInt32(const CRJStringByte&,const CRJStringByte&, int32_t);
	bool PutSize(const CRJStringByte&, size_t);
	bool PutSize(const CRJStringByte&,const CRJStringByte&, size_t);
	bool PutBool(const CRJStringByte&,bool);
	bool PutBool(const CRJStringByte&,const CRJStringByte&,bool);
	bool DelVar(const CRJStringByte&,const CRJStringByte&);
	bool DelKey(const CRJStringByte&);
	bool DelKey(const CRJStringByte&,const CRJStringByte&);
	bool RenameKey(const CRJStringByte&,const CRJStringByte&,bool);
	bool RenameKey(const CRJStringByte&,const CRJStringByte&,const CRJStringByte&,bool);
	bool Rename(const CRJStringByte&,const CRJStringByte&);
	bool Rename(const CRJStringByte&,const CRJStringByte&,const CRJStringByte&);
	bool DirectPut(const CRJStringByte&,const CRJStringByte&,const CRJStringByte&);
	bool DirectGet(const CRJStringByte&,const CRJStringByte&, CRJStringByteVar&);
	inline size_t GetKeyPart() const {
		return m_sKeyPart;
	}
	inline const CRJStringByte& GetRoot() const {
		return m_clRoot;
	}
	static CRJStringByte s_clRegPathOpened;
protected:
	CRJRegistry();
	bool addPath(const CRJStringByte&);
	bool get(const CRJStringByte&, CRJStringByteVar&);
	bool putc(const CRJStringByte&,const CRJStringByte&);
	bool del(const CRJStringByte&);
	class CRJRegistryPath final {
	public:
		CRJRegistryPath(CRJStringByteVar&);
		~CRJRegistryPath();
		bool SetPath(const CRJStringByte&);
		bool SetSection(const CRJStringByte&);
		CRJStringByteVar& m_clRoot;
		CRJStringByteDefault m_clSection;
		size_t m_sPathLen;
	};
protected:
	CRJAtomRegistryCached m_clRead;
	CRJStringByteDefault m_clRoot;
	size_t m_sKeyPart;
};

