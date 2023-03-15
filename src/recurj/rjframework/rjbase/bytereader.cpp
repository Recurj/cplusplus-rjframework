#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/bytereader.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
using namespace rjBase;
CRJByteReader::lpFuncFormatBlockHeader CRJByteReader::s_stTableFormat[cSizeFormatHeader] = {
	&CRJByteReader::FormatOutputData_CTECH,&CRJByteReader::FormatOutputData_TRANS,&CRJByteReader::FormatOutputData_BASES,
	&CRJByteReader::FormatOutputData_BAD,&CRJByteReader::FormatOutputData_BAD,&CRJByteReader::FormatOutputData_INTEL,
	&CRJByteReader::FormatOutputData_POS,&CRJByteReader::FormatOutputData_PLAIN,&CRJByteReader::FormatOutputData_BAD,
	&CRJByteReader::FormatOutputData_RUCARD,&CRJByteReader::FormatOutputData_TRANSMASTER,
	&CRJByteReader::FormatOutputData_VISTA,&CRJByteReader::FormatOutputData_HTTP,&CRJByteReader::FormatOutputData_TLS,
	&CRJByteReader::FormatOutputData_BAD,&CRJByteReader::FormatOutputData_BAD,
};
CRJByteReader::CRJByteReader() :
	m_sOffset(0),
	m_bOK(true) {}

CRJByteReader::CRJByteReader(const CRJStringByte& s) :
	CRJStringByteFlex(s),
	m_sOffset(0),
	m_bOK(true) {}
CRJByteReader::CRJByteReader(const uint8_t* p, size_t s) :
	CRJStringByteFlex((uint8_t*)p, s),
	m_sOffset(0),
	m_bOK(true) {}
CRJByteReader::~CRJByteReader() {}
void CRJByteReader::Restart(const uint8_t* p, size_t l) {
	CRJStringByteFlex::SetFromData(p, l);
	m_sOffset = 0;
	m_bOK = true;
}
void CRJByteReader::Reloaded(size_t sLen) {
	SetLengthSafed(sLen);
	m_sOffset = 0;
	m_bOK = true;
}
CRJByteReader& CRJByteReader::Skip(size_t s) {
	if (m_bOK &&
		HasData(s)) {
		m_sOffset += s;
		return *this;
	}
	return SetError();
}
CRJByteReader& CRJByteReader::Move(size_t p) {
	if (m_bOK && m_sLength > p) {
		m_sOffset = p;
		return *this;
	}
	return SetError();
}
CRJByteReader& CRJByteReader::Data(CRJStringByteFlex& v, size_t s) {
	if (m_bOK &&
		HasData(s)) {
		v.SetFromData(GetHeadByOffset(m_sOffset), s);
		m_sOffset += s;
		return *this;
	}
	return SetError();
}
CRJByteReader& CRJByteReader::StringVar(CRJStringByteVar& v) {
	if (m_bOK &&
		HasData(2)) {
		size_t h, s = GetByte(m_sOffset);
		m_sOffset++;
		h = GetByte(m_sOffset);
		s = ((s << 8) & 0x0FF00) + h;
		m_sOffset++;
		if (!s) {
			v.SetLength(s);
			return *this;
		}
		if (HasData(s) &&
			v.HasNeededSize(s)) {
			_MEMMOVE_BYTE_POINTER(v.GetHeadForUpdate(), GetHeadByOffset(m_sOffset), s);
			m_sOffset += s;
			v.SetLength(s);
			return *this;
		}
	}
	return SetError();
}
CRJByteReader& CRJByteReader::StringFlex(CRJStringByteFlex& v, size_t s) {
	if (m_bOK &&
		HasData(s)) {
		v.SetFromData(GetHeadByOffset(m_sOffset), s);
		m_sOffset += s;
		return *this;
	}
	return SetError();
}
CRJByteReader& CRJByteReader::StringFlex(CRJStringByteFlex& v) {
	if (m_bOK &&
		HasData(2)) {
		size_t h, s = GetByte(m_sOffset);
		m_sOffset++;
		h = GetByte(m_sOffset);
		s = ((s << 8) & 0x0FF00) + h;
		m_sOffset++;
		if (!s) {
			v.SetEmpty();
			return *this;
		}
		if (HasData(s)) {
			v.SetFromData(GetHeadByOffset(m_sOffset), s);
			m_sOffset += s;
			return *this;
		}
	}
	return SetError();
}
CRJByteReader& CRJByteReader::Byte(uint8_t& b) {
	if (m_bOK &&
		HasData(1)) {
		b = GetByte(m_sOffset);
		m_sOffset++;
		return *this;
	}
	return SetError();
}
CRJByteReader& CRJByteReader::UInt64(uint64_t& v) {
	if (m_bOK &&
		HasData(sizeof(v))) {
		v = *((uint64_t*)GetHeadByOffset(m_sOffset));
		m_sOffset += sizeof(v);
		if (CRJByteConvertor::IsBigEndian()) v = CRJByteConvertor::BigEndianReverse64(v);
		return *this;
	}
	return SetError();
}
CRJByteReader& CRJByteReader::Size(size_t& v) {
	if (m_bOK &&
		HasData(sizeof(v))) {
		v = *((size_t*)GetHeadByOffset(m_sOffset));
		m_sOffset += sizeof(v);
		if (CRJByteConvertor::IsBigEndian()) v = CRJByteConvertor::BigEndianReverse64(v);
		return *this;
	}
	return SetError();
}
CRJByteReader& CRJByteReader::UInt32(uint32_t& v) {
	if (m_bOK &&
		HasData(sizeof(v))) {
		v = *((uint32_t*)GetHeadByOffset(m_sOffset));
		m_sOffset += sizeof(v);
		if (CRJByteConvertor::IsBigEndian()) v = CRJByteConvertor::BigEndianReverse32(v);
		return *this;
	}
	return SetError();
}
CRJByteReader& CRJByteReader::UInt16(uint16_t& v) {
	if (m_bOK &&
		HasData(sizeof(v))) {
		v = *((uint16_t*)GetHeadByOffset(m_sOffset));
		m_sOffset += sizeof(v);
		if (CRJByteConvertor::IsBigEndian()) v = CRJByteConvertor::BigEndianReverse16(v);
		return *this;
	}
	return SetError();
}
CRJByteReader& CRJByteReader::Int64(int64_t& v) {
	if (m_bOK &&
		HasData(1)) {
		uint8_t b = GetByte(m_sOffset);
		m_sOffset++;
		if (HasData(sizeof(v))) {
			v = *((int64_t*)GetHeadByOffset(m_sOffset));
			m_sOffset += sizeof(v);
			if (CRJByteConvertor::IsBigEndian()) v = (int64_t)CRJByteConvertor::BigEndianReverse64(v);
			if (b) v = -v;
			return *this;
		}
	}
	return SetError();
}
CRJByteReader& CRJByteReader::Int32(int32_t& v) {
	if (m_bOK &&
		HasData(1)) {
		uint8_t b = GetByte(m_sOffset);
		m_sOffset++;
		if (HasData(sizeof(v))) {
			v = *((int32_t*)GetHeadByOffset(m_sOffset));
			m_sOffset += sizeof(v);
			if (CRJByteConvertor::IsBigEndian()) v = CRJByteConvertor::BigEndianReverse32(v);
			if (b) v = -v;
			return *this;
		}
	}
	return SetError();
}
CRJByteReader& CRJByteReader::Int16(int16_t& v) {
	if (m_bOK &&
		HasData(1)) {
		uint8_t b = GetByte(m_sOffset);
		m_sOffset++;
		if (HasData(sizeof(v))) {
			v = *((int16_t*)GetHeadByOffset(m_sOffset));
			m_sOffset += sizeof(v);
			if (CRJByteConvertor::IsBigEndian()) v = CRJByteConvertor::BigEndianReverse16(v);
			if (b) v = -v;
			return *this;
		}
	}
	return SetError();
}
bool CRJByteReader::Separator(CRJStringByteFlex& s, uint8_t ch, bool bExist) {
	if (!HasData()) return false;
	size_t l = GetTailSize();
	const uint8_t* h = GetHeadByOffset(m_sOffset);
	const uint8_t* p = CRJStringByte::FindTopThat(h, l, ch);
	if (p == nullptr) {
		if (bExist) return false;
		s.SetFromData(h, l);
	}
	else {
		l = p - h;
		if (!l) s.SetEmpty();
		else s.SetFromData(h, l);
		l++;
	}
	m_sOffset += l;
	return true;
}
bool CRJByteReader::TailClear(CRJStringByteFlex& s) {
	if (m_bOK) {
		size_t l = GetTailSize();
		if (!l) s.SetEmpty();
		else {
			s.SetFromData(GetHeadByOffset(m_sOffset), l);
			m_sOffset = m_sLength;
		}
		return true;
	}
	s.SetEmpty();
	return false;
}
CRJStringByteFlex& CRJByteReader::TailCopy(CRJStringByteFlex& s) const {
	if (m_bOK && HasData()) s.SetFromData(GetHeadByOffset(m_sOffset), GetTailSize());
	else s.SetEmpty();
	return s;
}
bool CRJByteReader::LineCRLF(CRJStringByteFlex& clStr) {
	if (m_bOK && HasData(1)) {
		const uint8_t* h = GetHeadByOffset(m_sOffset);
		const uint8_t* p = CRJStringByte::FindTopThat(h, GetTailSize(), 0x0A);
		if (p &&
			p != h &&
			*(p - 1) == 0x0D) {
			size_t i = p - h;
			m_sOffset += (i + 1);
			i -= 1;
			if (i > 0) clStr.SetFromData(h, i);
			else clStr.SetEmpty();
			return true;
		}
	}
	return false;
}
bool CRJByteReader::FormatBuffer(size_t h, CHeaderBuffer& clRequest) {
	if (m_bOK) {
		if (h >= cSizeFormatHeader) return false;
		else {
			size_t l = m_sOffset;
			if ((this->*CRJByteReader::s_stTableFormat[h])(clRequest)) return true;
			m_sOffset = l;
			m_bOK = true;
		}
	}
	return false;
}
bool CRJByteReader::FormatOutputData_CTECH(CHeaderBuffer& clRequest) {
	uint16_t l;
	if (UInt16(l)) {
		l = CRJByteConvertor::BigEndianSet16(l);
		return (l >= 2) &&
			Data(clRequest, l - 2);
	}
	return false;
}
bool CRJByteReader::FormatOutputData_TRANS(CHeaderBuffer& clRequest) {
	uint16_t l;
	if (UInt16(l)) {
		l = CRJByteConvertor::BigEndianSet16(l);
		return (l >= 5) &&
			Data(clRequest.m_clParams, 5) &&
			Data(clRequest, l - 5);
	}
	return false;
}
bool CRJByteReader::FormatOutputData_BASES(CHeaderBuffer& clRequest) {
	uint16_t l;
	return UInt16(l) &&
		Data(clRequest, CRJByteConvertor::BigEndianSet16(l));
}
bool CRJByteReader::FormatOutputData_INTEL(CHeaderBuffer& clRequest) {
	uint16_t l;
	return UInt16(l) &&
		Data(clRequest, l);
}
bool CRJByteReader::FormatOutputData_POS(CHeaderBuffer& clRequest) {
	uint8_t ch;
	return Byte(ch) &&
		ch == CODE_STX &&
		Separator(clRequest, CODE_ETX, true);
}
bool CRJByteReader::FormatOutputData_PLAIN(CHeaderBuffer& clRequest) {
	size_t l = GetTailSize();
	clRequest.SetFromData(GetHeadByOffset(m_sOffset), l);
	m_sOffset += l;
	return true;
}
bool CRJByteReader::FormatOutputData_RUCARD(CHeaderBuffer& clRequest) {
	uint8_t ch;
	size_t s;
	return Byte(ch) &&
		ch == 0xFF &&
		Data(clRequest, 5) &&
		clRequest.IsValidSize(s) &&
		Data(clRequest, s);
}
bool CRJByteReader::FormatOutputData_TRANSMASTER(CHeaderBuffer& clRequest) {
	size_t s;
	return Data(clRequest, 6) &&
		clRequest.IsValidSize(s) &&
		Data(clRequest, s);
}
bool CRJByteReader::FormatOutputData_VISTA(CHeaderBuffer& clRequest) {
	size_t s;
	return Data(clRequest, 4) &&
		clRequest.IsValidSize(s) &&
		Data(clRequest, s);
}
bool CRJByteReader::FormatOutputData_HTTP(CHeaderBuffer& clRequest) {
	return LineCRLF(clRequest);
}
bool CRJByteReader::FormatOutputData_TLS(CHeaderBuffer& clRequest) {
	uint16_t l;
	return Data(clRequest.m_clParams, 3) &&
		UInt16(l) &&
		Data(clRequest, CRJByteConvertor::BigEndianSet16(l));
}
bool CRJByteReader::FormatOutputData_BAD(CHeaderBuffer&) {
	return false;
}

