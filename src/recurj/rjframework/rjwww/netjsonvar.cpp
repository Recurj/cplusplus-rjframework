#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/www/json/builder.h>
using namespace rjWWW;
CRJStringByte CJSONBuilder::s_clJSONEscaped("\\\"");
CRJStringByte CJSONBuilder::s_clJSONTrue("true");
CRJStringByte CJSONBuilder::s_clJSONFalse("false");
CRJStringByte CJSONBuilder::s_clJSONNull("null");
CRJStringByte CJSONBuilder::s_clJSONEmptyString("\"\"");
bool CJSONBuilder::IsValueNull(const CRJStringByte& s) {
	return s==CJSONBuilder::s_clJSONNull;
}
bool CJSONBuilder::IsValueTrue(const CRJStringByte& s) {
	return s==CJSONBuilder::s_clJSONTrue;
}
bool CJSONBuilder::IsValuefalse(const CRJStringByte& s) {
	return s==CJSONBuilder::s_clJSONFalse;
}
CJSONBuilder::CJSONBuilder() {
	HasNeededSize(CRJWWWWJSONPreAllocated);
}
CJSONBuilder::~CJSONBuilder() {}
CJSONBuilder& CJSONBuilder::AddNull(const CRJStringByte& f) {
	Cat(f).CatSingle(':').Cat(CJSONBuilder::s_clJSONNull).CatSingle(',');
	return *this;
}
CJSONBuilder& CJSONBuilder::AddBool(const CRJStringByte& f,bool bValue) {
	Cat(f).CatSingle(':').Cat(bValue?
		CJSONBuilder::s_clJSONTrue:CJSONBuilder::s_clJSONFalse).CatSingle(',');
	return *this;
}
CJSONBuilder& CJSONBuilder::AddInt64(const CRJStringByte& f, int64_t lValue) {
	Cat(f).CatSingle(':').CatInt64(lValue).CatSingle(',');
	return *this;
}
CJSONBuilder& CJSONBuilder::AddSize(const CRJStringByte& f, size_t lValue) {
	Cat(f).CatSingle(':').CatSize(lValue).CatSingle(',');
	return *this;
}
CJSONBuilder& CJSONBuilder::AddStringConstant(const CRJStringByte& f,const CRJStringByte& v) {
	Cat(f).CatSingle(':').Cat(v).CatLabel(",", 1);
	return *this;
}
CJSONBuilder& CJSONBuilder::AddStringSimple(const CRJStringByte& f,const CRJStringByte& v) {
	Cat(f).CatLabel(":\"", 2).Cat(v).CatLabel("\",", 2);
	return *this;
}
CJSONBuilder& CJSONBuilder::AddStringVar(const CRJStringByte& f,const CRJStringByte& v) {
	Cat(f).CatSingle(':');
	return AddStringVarInArray(v);
}
CJSONBuilder& CJSONBuilder::AddArrayData(const CRJStringByte& f, size_t c, int64_t a[]) {
	if (!c) Cat(f).CatLabel(":[],", 4);
	else {
		Cat(f).CatLabel(":[", 2);
		for (size_t i=0; i<c; i++) CatInt64(a[i]).CatSingle(',');
		TruncRight(1).CatLabel("],", 2);
	}
	return *this;
}
CJSONBuilder& CJSONBuilder::AddNullInArray() {
	Cat(CJSONBuilder::s_clJSONNull).CatSingle(',');
	return *this;
}
CJSONBuilder& CJSONBuilder::AddBoolInArray(bool bValue) {
	Cat(bValue?
		CJSONBuilder::s_clJSONTrue:CJSONBuilder::s_clJSONFalse).CatSingle(',');
	return *this;
}
CJSONBuilder& CJSONBuilder::AddInt64InArray(int64_t lValue) {
	CatInt64(lValue).CatSingle(',');
	return *this;
}
CJSONBuilder& CJSONBuilder::AddSizeInArray(size_t lValue) {
	CatSize(lValue).CatSingle(',');
	return *this;
}
CJSONBuilder& CJSONBuilder::AddStringConstantInArray(const CRJStringByte& v) {
	Cat(v).CatLabel(",", 1);
	return *this;
}
CJSONBuilder& CJSONBuilder::AddStringVarInArray(const CRJStringByte& v) {
	CRJChainFinderSet fs;
	CRJStringByteFlex s(v);
	CatSingle('"');
	while (s.FindPosInSet(CJSONBuilder::s_clJSONEscaped, fs)) {
		CatData(s.GetHeadForRead(), fs.m_sFound).CatSingle('\\').CatSingle(s.GetHeadForRead()[fs.m_sFound]);
		s.TruncLeft(fs.m_sFound+1);
	}
	Cat(s).CatLabel("\",", 2);
	return *this;
}
CJSONBuilder& CJSONBuilder::OpenObject(const CRJStringByte& f) {
	Cat(f).CatLabel(":{", 2);
	return *this;
}
CJSONBuilder& CJSONBuilder::OpenObjectWithoutBrackets(const CRJStringByte& f) {
	CatSingle('"').Cat(f).CatLabel("\":{", 3);
	return *this;
}
CJSONBuilder& CJSONBuilder::OpenObject() {
	CatLabel("{", 1);
	return *this;
}
CJSONBuilder& CJSONBuilder::OpenArray(const CRJStringByte& f) {
	Cat(f).CatLabel(":[", 2);
	return *this;
}
CJSONBuilder& CJSONBuilder::OpenArray() {
	CatSingle('[');
	return *this;
}
CJSONBuilder& CJSONBuilder::CloseArrayEmpty() {
	CatLabel("],", 2);
	return *this;
}
CJSONBuilder& CJSONBuilder::CloseArray() {
	TruncRight(1).CatLabel("],", 2);
	return *this;
}
CJSONBuilder& CJSONBuilder::CloseObjectEmpty() {
	CatLabel("},", 2);
	return *this;
}
CJSONBuilder& CJSONBuilder::CloseObject() {
	TruncRight(1).CatLabel("},", 2);
	return *this;
}
CJSONBuilder& CJSONBuilder::NextObject() {
	TruncRight(1).CatLabel("},{", 3);
	return *this;
}
bool CJSONBuilder::FinalObjectSafed() {
	if (OK()) {
		SetAt(GetLength()-1,'}');
		return true;
	}
	return false;
}
bool CJSONBuilder::FinalArraySafed() {
	if (OK()) {
		SetAt(GetLength()-1,']');
		return true;
	}
	return false;
}
bool CJSONBuilder::FinalObject() {
	if (OK()) {
		size_t s=GetLength();
		if (s>1) return SetAt(s-1,'}').OK()&&
			IsFirstThat('{');
		else if (s==1) {
			CatSingle('}');
			return IsFirstThat('{');
		}
		else SetStringVarError(eUChainErrors::BadLength);
	}
	return false;
}
bool CJSONBuilder::FinalArray() {
	if (OK()) {
		size_t s=GetLength();
		if (s>1) return SetAt(GetLength()-1,']').OK()&&
			IsFirstThat('[');
		else if (s==1) {
			CatSingle(']');
			return IsFirstThat('[');
		}
		else SetStringVarError(eUChainErrors::BadLength);
	}
	return false;
}

