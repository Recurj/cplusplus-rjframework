#include "stdafx.h"
#include <recurj/rjframework/libs/base/formats/simple/builder.h>
#include <recurj/rjframework/libs/base/formats/simple/defs.h>
#include <recurj/rjframework/libs/base/formats/simple/errors.h>
using namespace rjBase;
RJFormatBuilder::RJFormatBuilder(CRJStringByteVar& s, size_t l) :
	m_clValue(s) {
	m_sLevel = l;
	m_bOK = true;
}
const CRJStringByte& RJFormatBuilder::Result() {
	if (!m_bOK) Failure(RemoteAppError::build(RemoteAppRegion::Format, RemoteAppRegionFormat::Build));
	return m_clValue;
}
bool RJFormatBuilder::NoErrors() {
	return m_bOK;
}
RJFormatBuilder& RJFormatBuilder::Reset() {
	m_clValue.SetEmpty();
	m_bOK = true;
	return *this;
}

RJFormatBuilder& RJFormatBuilder::OK() {
	m_clValue.SetSingle('+').Cat(RJFormat::Separators[m_sLevel]);
	return *this;
}
RJFormatBuilder& RJFormatBuilder::Failure(int64_t err) {
	m_clValue.SetSingle('-').Cat(RJFormat::Separators[m_sLevel]).CatInt64(err);
	return *this;
}
RJFormatBuilder& RJFormatBuilder::Enter() {
	m_sLevel++;
	return *this;
}
RJFormatBuilder& RJFormatBuilder::Leave() {
	m_sLevel--;
	m_bOK = m_bOK && m_clValue.Cat(RJFormat::Separators[m_sLevel]).OK();
	return *this;
}
RJFormatBuilder& RJFormatBuilder::Elem(const CRJStringByte& v) {
	m_bOK = m_bOK && m_clValue.Cat(v).Cat(RJFormat::Separators[m_sLevel]).OK();
	return *this;
}
RJFormatBuilder& RJFormatBuilder::ElemBool(bool b) {
	m_bOK = m_bOK && m_clValue.CatSingle((b) ?
		'1' : '0').Cat(RJFormat::Separators[m_sLevel]).OK();
	return *this;
}
RJFormatBuilder& RJFormatBuilder::ElemInt64(int64_t v) {
	m_bOK = m_bOK && m_clValue.CatInt64(v).Cat(RJFormat::Separators[m_sLevel]).OK();
	return *this;
}
RJFormatBuilder& RJFormatBuilder::ElemInt32(int32_t v) {
	m_bOK = m_bOK && m_clValue.CatInt64(v).Cat(RJFormat::Separators[m_sLevel]).OK();
	return *this;
}
RJFormatBuilder& RJFormatBuilder::ElemSize(size_t v) {
	m_bOK = m_bOK && m_clValue.CatSize(v).Cat(RJFormat::Separators[m_sLevel]).OK();
	return *this;
}
RJFormatBuilder& RJFormatBuilder::ElemKey(int64_t v) {
	m_bOK = m_bOK && m_clValue.CatSingle('#').CatInt64(v).Cat(RJFormat::Separators[m_sLevel]).OK();
	return *this;
}
RJFormatBuilder& RJFormatBuilder::ElemReference(int64_t r, const CRJStringByte& val) {
	m_bOK = m_bOK && m_clValue.CatInt64(r).CatSingle('-').Cat(val).Cat(RJFormat::Separators[m_sLevel]).OK();
	return *this;
}

RJFormatBuilder& RJFormatBuilder::ElemDouble(double v) {
	m_bOK = m_bOK && m_clValue.CatDouble(v).Cat(RJFormat::Separators[m_sLevel]).OK();
	return *this;
}
