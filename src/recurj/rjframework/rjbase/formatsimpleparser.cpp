#include "stdafx.h"
#include <recurj/rjframework/libs/base/formats/simple/parser.h>
#include <recurj/rjframework/libs/base/formats/simple/errors.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
using namespace rjBase;
int64_t RJFormatParser::Reply(const CRJStringByte& s, IRJFormatReader& receiver) {
	switch (s[0]) {
	case '+': return Fields(s, 0, receiver, 3);
	case '-': return onError(s);
	}
	return RemoteAppError::build(RemoteAppRegion::Format, RemoteAppRegionFormat::ParserHeader);
}
int64_t RJFormatParser::Request(const CRJStringByte& s, IRJFormatReader& receiver) {
	return Fields(s, 0, receiver, 0);
}
bool RJFormatParser::Lines(const CRJStringByte& s, size_t level, IRJFormatReader& receiver) {
	return receiver.total(RJFormatParser::Fields(s, level, receiver));
}
int64_t RJFormatParser::Fields(const CRJStringByte& orig, size_t level, IRJFormatReader& receiver, size_t start) {
	if (receiver.start(level) && start<orig.GetLength()) {
		CRJChainFinder finder;
		CRJStringByteFlex f, s(orig);
		size_t fields=0;
		const CRJStringByte& sep=RJFormat::Separators[level];
		s.TruncLeft(start);
		while (s.IsNonEmpty()) {
			if (!s.FindSubStr(sep, finder,0)) return RemoteAppError::build(RemoteAppRegion::Format, RemoteAppRegionFormat::ParserLineBadSeparator);
			fields++;
			if (!receiver.onRead(fields, f.SliceSafed(s, 0, finder.m_sFound))) return RemoteAppError::build(RemoteAppRegion::Format, RemoteAppRegionFormat::ParserLineBadValue);
			s.TruncLeft(finder.m_sFound+2);
		}
		return receiver.total(fields)?
			RemoteAppError::No:RemoteAppError::build(RemoteAppRegion::Format, RemoteAppRegionFormat::ParserTotal);
	}
	return RemoteAppError::build(RemoteAppRegion::Format, RemoteAppRegionFormat::ParserStart);
}
int64_t RJFormatParser::onError(const CRJStringByte& s) {
	if (s.GetLength()>3) {
		int64_t v;
		CRJStringByteFlex f;
		if (f.Slice(s, 3).IsValidInt64(v)) return v;
	}
	return -RemoteAppRegion::Undefined;
}

