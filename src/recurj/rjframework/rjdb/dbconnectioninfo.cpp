#include "stdafx.h"
#include <recurj/rjframework/libs/db/dbconnectioninfo.h>
#include <recurj/rjframework/libs/db/db.h>
#include <recurj/rjframework/libs/db/dbconnectioninfomap.h>
using namespace rjDb;
SDBConnectionOptions::SDBConnectionOptions():
	sDBConnectPause(60000),
sDBConnectRetry(3),
sDBTimeout(10),
sDBPacketSize(0),
sDBTrxnIsolation(SQL_TXN_READ_COMMITTED),
bSupport64BitsFields(true),
bAutoCommit(false),
bReadOnly(false) {}
SDBConnectionOptions::SDBConnectionOptions(const SDBConnectionOptions& st) {
	sDBConnectPause=st.sDBConnectPause;
	sDBConnectRetry=st.sDBConnectRetry;
	sDBTimeout=st.sDBTimeout;
	sDBPacketSize=st.sDBPacketSize;
	sDBTrxnIsolation=st.sDBTrxnIsolation;
	bSupport64BitsFields=st.bSupport64BitsFields;
	bAutoCommit=st.bAutoCommit;
	bReadOnly=st.bReadOnly;
}
void SDBConnectionOptions::Validate() {
	if (sDBConnectRetry<1) sDBConnectRetry=3;
	else if (sDBConnectRetry>10) sDBConnectRetry=10;
	if (sDBConnectPause<1) sDBConnectPause=1000;
	else if (sDBConnectPause>60000) sDBConnectPause=60000;
}
SDBConnectionOptions& SDBConnectionOptions::ConfigReadonly() {
	bReadOnly=true;
	bAutoCommit=true;
	return *this;
}
CRJDBConnectInfo::CRJDBConnectInfo()noexcept {}
CRJDBConnectInfo::CRJDBConnectInfo(const CRJDBConnectInfo& clData) {
	m_clDBConnect.Set(clData.m_clDBConnect);
	m_clDBSetup.Set(clData.m_clDBSetup);
	m_stOptions=clData.m_stOptions;
}
CRJDBConnectInfo::CRJDBConnectInfo(const CRJStringByte& connect,const CRJStringByte& setup,const SDBConnectionOptions& stOpts) {
	m_clDBConnect.Set(connect);
	m_clDBSetup.Set(setup);
	m_stOptions=stOpts;
}
CRJDBConnectInfo::~CRJDBConnectInfo() {}
bool CRJDBConnectInfo::DBConnectionInfoGet(const CRJStringByte&, CRJStringByteFlex& dsn, CRJStringByteFlex& setup, SDBConnectionOptions& opts) {
	setup.Set(m_clDBSetup);
	opts=m_stOptions;
	return dsn.Set(m_clDBConnect).GetLength()>0;
}
void CRJDBConnectInfo::operator=(const CRJDBConnectInfo& s) {
	m_clDBConnect.Set(s.m_clDBConnect);
	m_clDBSetup.Set(s.m_clDBSetup);
	m_stOptions=s.m_stOptions;
}
bool CRJDBConnectInfo::DBConnectionInfoAdd(const CRJStringByte&,const CRJStringByte& dsn,const CRJStringByte& setup, SDBConnectionOptions& opts) {
	m_clDBConnect.Set(dsn);
	m_clDBSetup.Set(setup);
	m_stOptions=opts;
	return true;
}
CRJDBCatalog::CRJDBCatalog() {}
CRJDBCatalog::~CRJDBCatalog() {
	m_clTable.Clear();
}
bool CRJDBCatalog::DBConnectionInfoAdd(const CRJStringByte& name,const CRJStringByte& connect,const CRJStringByte& setup, SDBConnectionOptions& stOpts) {
	CRJDBConnectInfo info(connect, setup, stOpts);
	return m_clTable.Map(name, info);
}
bool CRJDBCatalog::DBConnectionInfoGet(const CRJStringByte& name, CRJStringByteFlex& connect, CRJStringByteFlex& setup, SDBConnectionOptions& stOpts) {
	auto ret=m_clTable.Lookup(name);
	if (ret!=nullptr) {
		connect.Set(ret->m_clDBConnect);
		setup.Set(ret->m_clDBSetup);
		stOpts=ret->m_stOptions;
		return true;
	}
	return false;
}
bool CRJDBConnectInfoShared::DBConnectionInfoGet(const CRJStringByte& n, CRJStringByteFlex& dsn, CRJStringByteFlex& setup, SDBConnectionOptions& opts) {
	std::shared_lock lock(mutex);
	return CRJDBConnectInfo::DBConnectionInfoGet(n, dsn, setup, opts);
}
bool CRJDBConnectInfoShared::DBConnectionInfoAdd(const CRJStringByte& n,const CRJStringByte& dsn,const CRJStringByte& setup, SDBConnectionOptions& opts) {
	std::unique_lock lock(mutex);
	return CRJDBConnectInfo::DBConnectionInfoAdd(n, dsn, setup, opts);
}

