#pragma once
#include <recurj/rjframework/libs/net/netsocketudp.h>
#include <recurj/rjframework/libs/base/datatools/bytewriter.h>
#include <recurj/rjframework/libs/base/datatools/bytereader.h>
namespace rjNet::dns {
	constexpr size_t DNS_PORT=53;
	constexpr uint16_t DNSTYPE_A=0x0100;
	constexpr uint16_t DNSTYPE_NS=0x0200;
	constexpr uint16_t DNSTYPE_MD=0x0300;
	constexpr uint16_t DNSTYPE_MF=0x0400;
	constexpr uint16_t DNSTYPE_CNAME=0x0500;
	constexpr uint16_t DNSTYPE_SOA=0x0600;
	constexpr uint16_t DNSTYPE_MB=0x0700;
	constexpr uint16_t DNSTYPE_MG=0x0800;
	constexpr uint16_t DNSTYPE_MR=0x0900;
	constexpr uint16_t DNSTYPE_NULL=0x0A00;
	constexpr uint16_t DNSTYPE_WKS=0x0B00;
	constexpr uint16_t DNSTYPE_PTR=0x0C00;
	constexpr uint16_t DNSTYPE_HINFO=0x0D00;
	constexpr uint16_t DNSTYPE_MINFO=0x0E00;
	constexpr uint16_t DNSTYPE_MX=0x0F00;
	constexpr uint16_t DNSTYPE_TXT=0x1000;
	constexpr uint16_t DNSTYPE_AXFR=0xFC00;
	constexpr uint16_t DNSTYPE_MAILB=0xFD00;
	constexpr uint16_t DNSTYPE_MAILA=0xFE00;
	constexpr uint16_t DNSTYPE_ANY=0xFF00;
	constexpr uint16_t DNSClass_IN=0x0100;
	constexpr uint16_t DNSClass_CS=0x0200;
	constexpr uint16_t DNSClass_H=0x0300;
	constexpr uint16_t DNSClass_HS=0x0400;
	constexpr uint16_t DNSClass_ANY=0xFF00;
	class RJNET_API CRJNetDNSLookup:public CRJSocketUDP {
	public:
		CRJNetDNSLookup(IRJSocketHolder&, const CRJStringByte&);
		virtual~CRJNetDNSLookup();
		int GetMXRecord(uint16_t,const CRJStringByte&, IRJStringByteBool&);
	public:
		SRJSocketProcessMetrics sockRead;
	protected:
		using lpFuncReply=bool(CRJNetDNSLookup::*)(CRJByteReader&);
		CRJStringByte& dnsQueryMX(uint16_t,const CRJStringByte&, CRJStringByteVar&);
		int dnsReply(const CRJStringByte&, IRJByteReaderBool&);
		void queryHeader(CRJByteWriter&, uint16_t);
		void queryName(CRJByteWriter&,const CRJStringByte&);
		bool readName(CRJByteReader&, CRJStringByteVar&);
		bool skipName(CRJByteReader&);
	};
}

