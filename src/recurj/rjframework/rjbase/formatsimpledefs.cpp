#include "stdafx.h"
#include <recurj/rjframework/libs/base/formats/simple/defs.h>
#include <recurj/rjframework/libs/base/formats/simple/errors.h>
using namespace rjBase;
const CRJStringByte RJFormat::OK("+\n0");
const CRJStringByte RJFormat::Failure("-\n0");
const CRJStringByte RJFormat::Separators[10]= {
	"\n0","\n1","\n2","\n3","\n4","\n5","\n6","\n7","\n8","\n9"
};
bool RJFormatReaderStart::start(size_t) {
	return true;
}
RJFormatReader::RJFormatReader(size_t f):
	_fields(f) {}
bool RJFormatReader::total(size_t c) {
	return c==_fields;
}
bool RJFormatReaderLevel::start(size_t l) {
	_level=l;
	return true;
}
size_t RJFormatReaderLevel::NextLevel() {
	return _level+1;
}
bool RJFormatReaderArrayElemStart::start(size_t) {
	return true;
}
int64_t RemoteAppError::asyncErr() {
	return build(RemoteAppRegion::System, RemoteAppRegionSystem::AsyncError);
}
int64_t RemoteAppError::exeption() {
	return build(RemoteAppRegion::System, RemoteAppRegionSystem::Exception);
}
int64_t RemoteAppError::build(int64_t reg, int64_t err) {
	return (reg<<8)+err;
}

