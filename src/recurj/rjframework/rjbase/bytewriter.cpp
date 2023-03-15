#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/bytewriter.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
using namespace rjBase;
CRJByteWriter::lpFuncFormatBlockHeader CRJByteWriter::s_stTableFormat[cSizeFormatHeader]= {
	& CRJByteWriter::FormatOutputData_CTECH,& CRJByteWriter::FormatOutputData_TRANS,& CRJByteWriter::FormatOutputData_BASES,
	& CRJByteWriter::FormatOutputData_BAD,& CRJByteWriter::FormatOutputData_BAD,& CRJByteWriter::FormatOutputData_INTEL,
	& CRJByteWriter::FormatOutputData_POS,& CRJByteWriter::FormatOutputData_PLAIN,& CRJByteWriter::FormatOutputData_BAD,
	& CRJByteWriter::FormatOutputData_RUCARD,& CRJByteWriter::FormatOutputData_TRANSMASTER,
	& CRJByteWriter::FormatOutputData_VISTA,& CRJByteWriter::FormatOutputData_HTTP,& CRJByteWriter::FormatOutputData_TLS,
	& CRJByteWriter::FormatOutputData_BAD,& CRJByteWriter::FormatOutputData_BAD,
};
CRJByteWriter::CRJByteWriter(CRJStringByteVar& s) :m_bOK(true) { SetStorage(s); }
CRJByteWriter::~CRJByteWriter() {}
CRJByteWriter& CRJByteWriter::StringShort(const CRJStringByte& v) {
	if (m_bOK) {
		uint8_t s=(uint8_t) v.GetLength();
		m_bOK=CatSingle(s).CatData(v.GetHeadForRead(), s).OK();
	}
	return *this;
}
CRJByteWriter& CRJByteWriter::String(const CRJStringByte& v) {
	if (m_bOK) {
		uint16_t s=(uint16_t) v.GetLength();
		m_bOK=CatSingle((uint8_t)((s>>8)& 0xFF)).CatSingle((uint8_t)(s& 0xFF)).Cat(v).OK();
	}
	return *this;
}
CRJByteWriter& CRJByteWriter::Label(const char* b, size_t s) {
	if (m_bOK) {
		m_bOK=CatSingle((uint8_t)((s>>8)& 0xFF)).CatSingle((uint8_t)(s& 0xFF)).CatLabel(b, s).OK();
	}
	return *this;
}
CRJByteWriter& CRJByteWriter::Data(const uint8_t* p, size_t l) {
	if (m_bOK) m_bOK=CatData(p, l).OK();
	return *this;
}
CRJByteWriter& CRJByteWriter::Data(const CRJStringByte& s) {
	if (m_bOK) m_bOK=Cat(s).OK();
	return *this;
}
CRJByteWriter& CRJByteWriter::Byte(uint8_t b) {
	if (m_bOK) m_bOK=CatSingle(b).OK();
	return *this;
}
CRJByteWriter& CRJByteWriter::Size(size_t v) {
	if (m_bOK) {
		if (CRJByteConvertor::IsBigEndian()) v=CRJByteConvertor::BigEndianReverse64(v);
		m_bOK=CatData((uint8_t*)& v,sizeof(v)).OK();
	}
	return *this;
}
CRJByteWriter& CRJByteWriter::UInt64(uint64_t v) {
	if (m_bOK) {
		if (CRJByteConvertor::IsBigEndian()) v=CRJByteConvertor::BigEndianReverse64(v);
		m_bOK=CatData((uint8_t*)& v,sizeof(v)).OK();
	}
	return *this;
}
CRJByteWriter& CRJByteWriter::UInt32(uint32_t v) {
	if (m_bOK) {
		if (CRJByteConvertor::IsBigEndian()) v=CRJByteConvertor::BigEndianReverse32(v);
		m_bOK=CatData((uint8_t*)& v,sizeof(v)).OK();
	}
	return *this;
}
CRJByteWriter& CRJByteWriter::UInt16(uint16_t v) {
	if (m_bOK) {
		if (CRJByteConvertor::IsBigEndian()) v=CRJByteConvertor::BigEndianReverse16(v);
		m_bOK=CatData((uint8_t*)& v,sizeof(v)).OK();
	}
	return *this;
}
CRJByteWriter& CRJByteWriter::Int64(int64_t v) {
	if (m_bOK) {
		if (v<0) {
			v=abs(v);
			m_bOK=CatSingle(1).OK();
		}
		else m_bOK=CatSingle(0).OK();
		return UInt64(v);
	}
	return *this;
}
CRJByteWriter& CRJByteWriter::Int32(int32_t v) {
	if (m_bOK) {
		if (v<0) {
			v=abs(v);
			m_bOK=CatSingle(1).OK();
		}
		else m_bOK=CatSingle(0).OK();
		return UInt32(v);
	}
	return *this;
}
CRJByteWriter& CRJByteWriter::Int16(int16_t v) {
	if (m_bOK) {
		if (v<0) {
			v=(int16_t) abs(v);
			m_bOK=CatSingle(1).OK();
		}
		else m_bOK=CatSingle(0).OK();
		return UInt16(v);
	}
	return *this;
}
CRJByteWriter& CRJByteWriter::SetLengthBaseFormat() {
	*((uint16_t*) GetHeadForUpdate())=CRJByteConvertor::BigEndianSet16((uint16_t)(GetLength()-2));
	return *this;
}
CRJByteWriter& CRJByteWriter::FormatBuffer(size_t h,const CHeaderBuffer& clRequest) {
	if (m_bOK) {
		if (h>=cSizeFormatHeader) return Error();
		else {
			size_t l=GetLength();
			if (!(this->* CRJByteWriter::s_stTableFormat[h])(clRequest)) {
				SetLengthSafed(l);
				return Error();
			}
		}
	}
	return *this;
}
bool CRJByteWriter::FormatOutputData_CTECH(const CHeaderBuffer& clRequest) {
	return UInt16(CRJByteConvertor::BigEndianSet16((uint16_t)(clRequest.GetLength()+2)))&&
		Data(clRequest);
}
bool CRJByteWriter::FormatOutputData_TRANS(const CHeaderBuffer& clRequest) {
	return UInt16(CRJByteConvertor::BigEndianSet16((uint16_t)(clRequest.GetLength()+5)))&&
		Data(clRequest.m_pParams, 5)&&
		Data(clRequest);
}
bool CRJByteWriter::FormatOutputData_BASES(const CHeaderBuffer& clRequest) {
	return UInt16(CRJByteConvertor::BigEndianSet16((uint16_t)(clRequest.GetLength())))&&
		Data(clRequest);
}
bool CRJByteWriter::FormatOutputData_INTEL(const CHeaderBuffer& clRequest) {
	return UInt16((uint16_t)(clRequest.GetLength()))&&
		Data(clRequest);
}
bool CRJByteWriter::FormatOutputData_POS(const CHeaderBuffer& clRequest) {
	return Byte(CODE_STX)&&
		Data(clRequest)&&
		Byte(CODE_ETX);
}
bool CRJByteWriter::FormatOutputData_PLAIN(const CHeaderBuffer& clRequest) {
	return Data(clRequest);
}
bool CRJByteWriter::FormatOutputData_RUCARD(const CHeaderBuffer& clRequest) {
	char s[8];
	sprintf_s(s,sizeof(s),"\xFF"
	"%05d",(int) clRequest.GetLength());
	return Data((uint8_t*) s, 6)&&
		Data(clRequest);
}
bool CRJByteWriter::FormatOutputData_TRANSMASTER(const CHeaderBuffer& clRequest) {
	char s[8];
	sprintf_s(s,sizeof(s),"%06d",(int) clRequest.GetLength());
	return Data((uint8_t*) s, 6)&&
		Data(clRequest);
}
bool CRJByteWriter::FormatOutputData_VISTA(const CHeaderBuffer& clRequest) {
	char s[8];
	sprintf_s(s,sizeof(s),"%04d",(int) clRequest.GetLength());
	return Data((uint8_t*) s, 4)&&
		Data(clRequest);
}
bool CRJByteWriter::FormatOutputData_HTTP(const CHeaderBuffer& clRequest) {
	return Data(clRequest)&&
		Data(CRJStringByte::s_CRLF);
}
bool CRJByteWriter::FormatOutputData_TLS(const CHeaderBuffer& clRequest) {
	return Data(clRequest.m_pParams, 3)&&
		UInt16(CRJByteConvertor::BigEndianSet16((uint16_t)(clRequest.GetLength())))&&
		Data(clRequest);
}
bool CRJByteWriter::FormatOutputData_BAD(const CHeaderBuffer&) {
	return false;
}

