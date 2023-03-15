#include "stdafx.h"
#include <recurj/rjframework/libs/base/formats/simple/readers.h>
#include <recurj/rjframework/libs/base/formats/simple/errors.h>
#include <recurj/rjframework/libs/base/formats/simple/readerhandbook.h>
#include <recurj/rjframework/libs/base/formats/simple/parser.h>
using namespace rjBase;
RJFormatReaderEmpty::RJFormatReaderEmpty():
	RJFormatReader(0) {}
bool RJFormatReaderEmpty::onRead(size_t,const CRJStringByte&) {
	return false;
}
const CRJStringByte& RJFormatReaderString::Value() const {
	return m_clValue;
}
bool RJFormatReaderString::start(size_t) {
	m_clValue.SetEmpty();
	return true;
}
bool RJFormatReaderString::onRead(size_t f,const CRJStringByte& v) {
	switch (f) {
	case 1:m_clValue.Set(v);
		break;
	default:
		return false;
	}
	return true;
}
bool RJFormatReaderString::total(size_t c) {
	return c==1;
}
RJFormatReaderHandbook::RJFormatReaderHandbook(CMapHandbook& h):
	m_clHandbook(h) {}
CMapHandbook& RJFormatReaderHandbook::GHandbook() {
	return m_clHandbook;
}
bool RJFormatReaderHandbook::onRead(size_t f,const CRJStringByte& v) {
	switch (f) {
	case 1: return v.IsValidInt64(_id);
	case 2:_value.Set(v);
		return true;
	}
	return false;
}
bool RJFormatReaderHandbook::total(size_t c) {
	if (c==2) {
		return m_clHandbook.Map(_id, _value);
	}
	return false;
}
RJFormatReaderArrayElement::RJFormatReaderArrayElement(RJFormatReaderArrayElem& parser):
_elemParser(parser) {}
bool RJFormatReaderArrayElement::start(size_t l) {
	_elemParser.m_sSeq=0;
	Count = 0;
	return RJFormatReaderLevel::start(l);
}
bool RJFormatReaderArrayElement::onRead(size_t,const CRJStringByte& v) {
	_elemParser.m_sSeq++;
	return RJFormatParser::Fields(v, NextLevel(), _elemParser)==RemoteAppError::No;
}
bool RJFormatReaderArrayElement::total(size_t c) {
	Count = c;
	return true;
}
RJFormatReaderArray::RJFormatReaderArray(RJFormatReaderArrayElem& fieldParser):
	_elemParser(fieldParser) {}
bool RJFormatReaderArray::start(size_t l) {
	Count=0;
	return RJFormatReaderLevel::start(l);
}
bool RJFormatReaderArray::onRead(size_t f,const CRJStringByte& v) {
	switch (f) {
	case 1: return _parseRecords(v);
	case 2: 
		size_t c;
		return v.IsValidSize(c) && c==Count;
	}
	return false;
}
bool RJFormatReaderArray::total(size_t c) {
	return c==2;
}
bool RJFormatReaderArray::_parseRecords(const CRJStringByte& value) {
	RJFormatReaderArrayElement reader(_elemParser);
	if (RJFormatParser::Fields(value, NextLevel(), reader) == RemoteAppError::No) {
		Count = reader.Count;
		return true;
	}
	return false;
}

