#pragma once
#include <recurj/rjframework/libs/base/formats/simple/defs.h>
#include <recurj/rjframework/libs/base/collections//maps/handbook.h>
namespace rjBase {
	class RJBASE_API RJFormatReaderHandbook:public RJFormatReaderStart {
	public:
		RJFormatReaderHandbook(CMapHandbook&);
		bool onRead(size_t,const CRJStringByte&) override;
		bool total(size_t) override;
		CMapHandbook& GHandbook();
	protected:
		CMapHandbook& m_clHandbook;
		int64_t _id=0;
		CRJStringByteAllocated _value;
	};
}

