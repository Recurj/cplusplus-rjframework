#pragma once
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
namespace rjBase {
	class RJBASE_API RJFormatBuilder {
	public:
		RJFormatBuilder(CRJStringByteVar&, size_t l=0);
		RJFormatBuilder& Reset();
		RJFormatBuilder& OK();
		RJFormatBuilder& Failure(int64_t);
		RJFormatBuilder& Enter();
		RJFormatBuilder& Leave();
		RJFormatBuilder& Elem(const CRJStringByte&);
		RJFormatBuilder& ElemBool(bool);
		RJFormatBuilder& ElemInt64(int64_t);
		RJFormatBuilder& ElemInt32(int32_t);
		RJFormatBuilder& ElemSize(size_t);
		RJFormatBuilder& ElemKey(int64_t);
		RJFormatBuilder& ElemDouble(double);
		RJFormatBuilder& ElemReference(int64_t,const CRJStringByte&);
		const CRJStringByte& Result();
		bool NoErrors();
	protected:
		CRJStringByteVar& m_clValue;
		size_t m_sLevel;
		bool m_bOK;
	};
}

