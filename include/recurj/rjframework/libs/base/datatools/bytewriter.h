#pragma once
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/libs/base/memory/allocator.h>
namespace rjBase {
	class RJBASE_API CRJByteWriter : public CRJStringByteVarAssigned {
	public:
		class CHeaderBuffer:public CRJStringByteFlex {
		public:
			CHeaderBuffer(uint8_t* p, size_t s):
				CRJStringByteFlex(p, s),
			m_pParams(nullptr) {}
			CHeaderBuffer(const CRJStringByteFlex& v):
				CRJStringByteFlex(v),
			m_pParams(nullptr) {}
			const uint8_t* m_pParams;
		};
		CRJByteWriter(CRJStringByteVar &);
		virtual~CRJByteWriter();
		operator bool() const {
			return m_bOK;
		}
		CRJByteWriter& Error() {
			m_bOK=false;
			return *this;
		}
		CRJByteWriter& ResetWriter() {
			m_bOK=true;
			SetEmpty();
			return *this;
		}
		CRJByteWriter& Data(const uint8_t*, size_t);
		CRJByteWriter& Data(const CRJStringByte&);
		CRJByteWriter& StringShort(const CRJStringByte&);
		CRJByteWriter& String(const CRJStringByte&);
		CRJByteWriter& Label(const char*, size_t);
		CRJByteWriter& Byte(uint8_t);
		CRJByteWriter& UInt64(uint64_t);
		CRJByteWriter& UInt32(uint32_t);
		CRJByteWriter& UInt16(uint16_t);
		CRJByteWriter& Int64(int64_t);
		CRJByteWriter& Int32(int32_t);
		CRJByteWriter& Int16(int16_t);
		CRJByteWriter& Size(size_t);
		CRJByteWriter& FormatBuffer(size_t,const CHeaderBuffer&);
		CRJByteWriter& SetLengthBaseFormat();
		using lpFuncFormatBlockHeader=bool(CRJByteWriter::*)(const CHeaderBuffer&);
	protected:
		bool FormatOutputData_CTECH(const CHeaderBuffer&);
		bool FormatOutputData_TRANS(const CHeaderBuffer&);
		bool FormatOutputData_BASES(const CHeaderBuffer&);
		bool FormatOutputData_INTEL(const CHeaderBuffer&);
		bool FormatOutputData_POS(const CHeaderBuffer&);
		bool FormatOutputData_PLAIN(const CHeaderBuffer&);
		bool FormatOutputData_RUCARD(const CHeaderBuffer&);
		bool FormatOutputData_TRANSMASTER(const CHeaderBuffer&);
		bool FormatOutputData_VISTA(const CHeaderBuffer&);
		bool FormatOutputData_HTTP(const CHeaderBuffer&);
		bool FormatOutputData_TLS(const CHeaderBuffer&);
		bool FormatOutputData_BAD(const CHeaderBuffer&);
	protected:
		static lpFuncFormatBlockHeader s_stTableFormat[cSizeFormatHeader];
		bool m_bOK=true;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJByteWriter)
	};
}

