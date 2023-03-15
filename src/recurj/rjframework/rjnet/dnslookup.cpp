#include "stdafx.h"
#include <recurj/rjframework/libs/net/tools/dnslookup.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
using namespace rjNet::dns;
CRJNetDNSLookup::CRJNetDNSLookup(IRJSocketHolder& holder, const CRJStringByte& clHost):
	CRJSocketUDP(holder){
	SocketUDPAlloc().SocketUDPBindClient();
	SocketConnect(DNS_PORT, clHost);
}
CRJNetDNSLookup::~CRJNetDNSLookup() {}
int CRJNetDNSLookup::GetMXRecord(uint16_t id,const CRJStringByte& name, IRJStringByteBool& func) {
	try {
		CRJStringByteExpanded buf;
		auto call=[this,& func](CRJByteReader& reader)->bool {
			uint16_t t;
			CRJStringByteExpanded s;
			if (skipName(reader)&&
				reader.UInt16(t)) {
				if (CRJByteConvertor::IsBigEndian()) t=CRJByteConvertor::BigEndianReverse16(t);
				return t==DNSTYPE_MX&&
					reader.Skip(10)&&
					readName(reader, s)&&
					func(s);
			}
			return true;
		};
		CRJNetSend sockSend(*this);
		CRJByteReaderFunctorBool<decltype(call)>cb(call);
		CRJStringByteFlex block(dnsQueryMX(id, name, buf));
		if (SocketSend(block, sockSend)&&
			SocketReadAvailable(buf, sockRead)) {
			return dnsReply(buf, cb);
		}
	}
	catch(...) {}
	return 0;
}
CRJStringByte& CRJNetDNSLookup::dnsQueryMX(uint16_t id,const CRJStringByte& name, CRJStringByteVar& buf) {
	CRJByteWriter writer(buf);
	queryHeader(writer, id);
	queryName(writer, name);
	if (CRJByteConvertor::IsBigEndian()) {
		writer.UInt16(CRJByteConvertor::BigEndianReverse16(DNSTYPE_MX));
		writer.UInt16(CRJByteConvertor::BigEndianReverse16(DNSClass_IN));
	}
	else {
		writer.UInt16(DNSTYPE_MX);
		writer.UInt16(DNSClass_IN);
	}
	return buf;
}
int CRJNetDNSLookup::dnsReply(const CRJStringByte& r, IRJByteReaderBool& func) {
	CRJByteReader reader(r);
	uint16_t i, qc, ac;
	if (reader.Skip(4)&&
		reader.UInt16(qc)&&
		reader.UInt16(ac)&&
		reader.Skip(4)) {
		if (!CRJByteConvertor::IsBigEndian()) {
			qc=CRJByteConvertor::BigEndianReverse16(qc);
			ac=CRJByteConvertor::BigEndianReverse16(ac);
		}
		for (i=0; i<qc; i++) {
			if (!skipName(reader)||
				reader.Skip(4)) return -1;
		}
		for (i=0; i<ac; i++) {
			if (!func(reader)) return i+1;
		}
		return ac;
	}
	return 0;
}
void CRJNetDNSLookup::queryHeader(CRJByteWriter& writer, uint16_t id) {
	writer.UInt16(id);
	writer.Byte(1);
	writer.Byte(0);
	writer.Byte(0);
	writer.Byte(1);
	writer.UInt16(0);
	writer.UInt32(0);
}
void CRJNetDNSLookup::queryName(CRJByteWriter& writer,const CRJStringByte& name) {
	auto call=[& writer](CRJStringByteFind& s)->void {
		writer.StringShort(s);
	};
	CRJStringByteFunctorSplit<decltype(call)>cb(call);
	name.Split('.', cb);
	writer.Byte(0);
}
bool CRJNetDNSLookup::readName(CRJByteReader& reader, CRJStringByteVar& name) {
	uint8_t l;
	CRJStringByteFlex elem;
	while (reader.Byte(l)) {
		if (l==0)break;
		if ((l& 0xC0)==0xC0) {
			uint8_t t;
			if (reader.Byte(t)) {
				size_t a=t;
				size_t p=reader.GetOffset();
				size_t ls=l;
				reader.Move(((ls& 0x3F)<<8)+a);
				readName(reader, name);
				reader.Move(p);
			}
			break;
		}
		if (name.GetLength()>0) name.CatSingle('.');
		if (reader.StringFlex(elem, l)) name.Cat(elem);
		else break;
	}
	return reader;
}
bool CRJNetDNSLookup::skipName(CRJByteReader& reader) {
	uint8_t l;
	while (reader.Byte(l)) {
		if (l==0)break;
		if ((l& 0xC0)==0xC0) {
			return reader.Skip(1);
		}
		if (!reader.Skip(l)) return true;
	}
	return reader;
}

