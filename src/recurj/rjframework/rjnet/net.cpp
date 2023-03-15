#include "stdafx.h"
#include <recurj/rjframework/libs/net/netbase.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/net/netsocket.h>
#include <recurj/rjframework/libs/net/dataapi/netsessions.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
using namespace rjNet;
const CRJStringByte CRJNet::s_localHost("localhost");
const SRJSocketProcessMetrics SRJSocketProcessMetrics::s_stSendMetrics(3, 100);
#ifdef _rjOS_FAMILY_UNIX
int64_t CRJNet::GetNetLastError() {
	return (int64_t) errno;
}
bool CRJNet::Open() {
	return true;
}
void CRJNet::Close() {}
#endif 
#ifdef _rjOS_WIN
int64_t CRJNet::GetNetLastError() {
	return (int64_t)::WSAGetLastError();
}
bool CRJNet::Open() {
	try {
		WSAData wsaData;
		_MEMCLR_BYTE(wsaData);
		return (!(::WSAStartup(MAKEWORD(2, 2),& wsaData)));
	}
	catch(...) {}
	return false;
}
void CRJNet::Close() {
	try {
		::WSACleanup();
	}
	catch(...) {}
}
#endif 
CRJByteReader& CRJByteReaderSessions::ReadHeader(SDataApiHeaderNetSessions& s) {
	if (m_bOK&&HasData(sizeof(s))) {
		const uint8_t* p = GetHeadByOffset(m_sOffset);
		s.lModule=*((uint16_t*)p);
		m_sOffset+=sizeof(s.lModule);
		p += sizeof(s.lModule);
		s.lRegion=*((uint16_t*)p);
		m_sOffset+=sizeof(s.lRegion);
		p += sizeof(s.lRegion);
		s.lCmd=*((uint16_t*)p);
		m_sOffset+=sizeof(s.lCmd);
		p += sizeof(s.lCmd);
		s.lOptions=*((uint16_t*)p);
		m_sOffset+=sizeof(s.lOptions);
		if (CRJByteConvertor::IsBigEndian()) {
			s.lModule=CRJByteConvertor::BigEndianSet16(s.lModule);
			s.lRegion=CRJByteConvertor::BigEndianSet16(s.lRegion);
			s.lCmd=CRJByteConvertor::BigEndianSet16(s.lCmd);
			s.lOptions=CRJByteConvertor::BigEndianSet16(s.lOptions);
		}
		return *this;
	}
	return SetError();
}
CRJByteWriter& CRJByteWriterSessions::WriteHeader(const SDataApiHeaderNetSessions& h) {
	return WriteHeader(h.lModule, h.lRegion, h.lCmd, h.lOptions);
}
CRJByteWriter& CRJByteWriterSessions::WriteHeader(uint16_t m, uint16_t r, uint16_t cmd, uint16_t opts) {
	m_bOK=SetLabel("\x00\x00", 2);
	if (CRJByteConvertor::IsBigEndian()) {
		m=CRJByteConvertor::BigEndianSet16(m);
		r=CRJByteConvertor::BigEndianSet16(r);
		cmd=CRJByteConvertor::BigEndianSet16(cmd);
		opts=CRJByteConvertor::BigEndianSet16(opts);
	}
	m_bOK=CatData((uint8_t*)& m,sizeof(m)).CatData((uint8_t*)& r,sizeof(r)).CatData((uint8_t*)& cmd,sizeof(cmd)).CatData((uint8_t*)& opts,sizeof(opts)).OK();
	return *this;
}

