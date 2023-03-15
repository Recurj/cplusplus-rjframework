#pragma once
#include <recurj/rjframework/libs/base/formats/simple/defs.h>
namespace rjBase {
	class RJBASE_API RJFormatReaderEmpty:public RJFormatReader {
	public:
		RJFormatReaderEmpty();
		bool onRead(size_t,const CRJStringByte&) override;
	};
	class RJBASE_API RJFormatReaderString:public IRJFormatReader {
		RJFormatReaderString(CRJStringByteVar&);
		bool start(size_t) override;
		bool onRead(size_t,const CRJStringByte&) override;
		bool total(size_t c) override;
		const CRJStringByte& Value() const;
	protected:
		CRJStringByteVar& m_clValue;
	};
	class RJFormatReaderArrayElement:public RJFormatReaderLevel {
	public:
		RJFormatReaderArrayElement(RJFormatReaderArrayElem&);
		bool start(size_t) override;
		bool onRead(size_t,const CRJStringByte&) override;
		bool total(size_t c) override;
		size_t Count=0;
	protected:
		RJFormatReaderArrayElem& _elemParser;
	};
	class RJFormatReaderArray:public RJFormatReaderLevel {
	public:
		RJFormatReaderArray(RJFormatReaderArrayElem& f);
		bool start(size_t) override;
		bool onRead(size_t,const CRJStringByte&) override;
		bool total(size_t c) override;
		size_t Count=0;
	protected:
		bool _parseRecords(const CRJStringByte&);
	protected:
		RJFormatReaderArrayElem& _elemParser;
	};
}

