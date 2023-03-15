#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
namespace rjBase {
	class RJBASE_API RJFormat {
	public:
		static const CRJStringByte OK;
		static const CRJStringByte Failure;
		static const CRJStringByte Separators[10];
		//	     public static string Data(size_t v) = > v.ToString() + Separators[0];
		//	     public static string Data(long v) = > v.ToString() + Separators[0];
	};
	class IRJFormatReader {
	public:
		virtual bool start(size_t)=0;
		virtual bool onRead(size_t,const CRJStringByte&)=0;
		virtual bool total(size_t)=0;
	};
	class RJBASE_API RJFormatReaderStart:public IRJFormatReader {
	public:
		bool start(size_t) override;
	};
	class RJBASE_API RJFormatReader:public RJFormatReaderStart {
	public:
		RJFormatReader(size_t);
		bool total(size_t) override;
	protected:
		const size_t _fields;
	};
	class RJBASE_API RJFormatReaderLevel:public IRJFormatReader {
	public:
		bool start(size_t l) override;
		size_t NextLevel();
	protected:
		size_t _level=0;
	};
	class RJFormatReaderArrayElem:public IRJFormatReader {
	public:
		size_t m_sSeq=0;
	};
	class RJFormatReaderArrayElemStart:public RJFormatReaderArrayElem {
		bool start(size_t l) override;
	};
}

