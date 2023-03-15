#pragma once
#include <recurj/rjframework/datalength.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
namespace rjBase {
	class CRJStringByteVar;
	class RJBASE_API CRJByteReader :public CRJStringByteFlex {
	public:
		class CHeaderBuffer :public CRJStringByteFlex {
		public:
			CHeaderBuffer() {}
			CRJStringByteFlex m_clParams;
		};
		CRJByteReader();
		CRJByteReader(const uint8_t*, size_t);
		CRJByteReader(const CRJStringByte&);
		virtual~CRJByteReader();
		virtual void Restart(const uint8_t*, size_t);
		void Restart(const CRJStringByte& s) {
			Restart(s.GetHeadForRead(), s.GetLength());
		}
		void Reloaded(size_t);
		operator bool() const {
			return m_bOK;
		}
		bool Error() {
			m_bOK = false;
			m_sOffset = 0;
			return false;
		}
		void Reset() {
			m_bOK = true;
			m_sOffset = 0;
		}
		CRJByteReader& Data(CRJStringByteFlex&, size_t);
		CRJByteReader& StringVar(CRJStringByteVar&);
		CRJByteReader& StringFlex(CRJStringByteFlex&, size_t);
		CRJByteReader& StringFlex(CRJStringByteFlex&);
		CRJByteReader& Byte(uint8_t&);
		CRJByteReader& UInt64(uint64_t&);
		CRJByteReader& Size(size_t&);
		CRJByteReader& UInt32(uint32_t&);
		CRJByteReader& UInt16(uint16_t&);
		CRJByteReader& Int64(int64_t&);
		CRJByteReader& Int32(int32_t&);
		CRJByteReader& Int16(int16_t&);
		CRJByteReader& Skip(size_t);
		CRJByteReader& Move(size_t);
		bool FormatBuffer(size_t, CHeaderBuffer&);
		bool Separator(CRJStringByteFlex&, uint8_t, bool bExist = false);
		bool TailClear(CRJStringByteFlex&);
		CRJStringByteFlex& TailCopy(CRJStringByteFlex&) const;
		bool LineCRLF(CRJStringByteFlex&);
		inline size_t GetOffset() const {
			return m_sOffset;
		}
		inline size_t GetTailSize() const {
			return m_sLength - m_sOffset;
		}
		inline bool HasData() const {
			return m_sLength > m_sOffset;
		}
		inline const uint8_t* Head() const {
			return GetHeadByOffset(m_sOffset);
		}
	protected:
		using lpFuncFormatBlockHeader = bool(CRJByteReader::*)(CHeaderBuffer&);
		inline uint8_t GetByte(size_t lOffset) { return (*this)[lOffset]; }
		inline bool HasData(size_t lLen) { return m_sLength >= lLen + m_sOffset; }
		inline CRJByteReader& SetError() { m_bOK = false;	return *this; }
		bool FormatOutputData_CTECH(CHeaderBuffer&);
		bool FormatOutputData_TRANS(CHeaderBuffer&);
		bool FormatOutputData_BASES(CHeaderBuffer&);
		bool FormatOutputData_INTEL(CHeaderBuffer&);
		bool FormatOutputData_POS(CHeaderBuffer&);
		bool FormatOutputData_PLAIN(CHeaderBuffer&);
		bool FormatOutputData_RUCARD(CHeaderBuffer&);
		bool FormatOutputData_TRANSMASTER(CHeaderBuffer&);
		bool FormatOutputData_VISTA(CHeaderBuffer&);
		bool FormatOutputData_HTTP(CHeaderBuffer&);
		bool FormatOutputData_TLS(CHeaderBuffer&);
		bool FormatOutputData_BAD(CHeaderBuffer&);
	protected:
		static lpFuncFormatBlockHeader s_stTableFormat[cSizeFormatHeader];
		size_t m_sOffset;
		bool m_bOK;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJByteReader)
	};
	using IRJByteReader = IRJFunctor<CRJByteReader&>;
	using IRJByteReaderBool = IRJFunctorBool<CRJByteReader&>;
	template<typename TFunc>
	using CRJByteReaderFunctor = CRJFunctor<IRJByteReader, TFunc, CRJByteReader&>;
	template<typename TFunc>
	using CRJByteReaderFunctorBool = CRJFunctorBool<IRJByteReaderBool, TFunc, CRJByteReader&>;
}

