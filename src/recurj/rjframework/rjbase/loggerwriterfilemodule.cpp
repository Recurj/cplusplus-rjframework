#include "stdafx.h"
#include <recurj/rjframework/libs/base/logger/writers/filemodule.h>
#include <recurj/rjframework/libs/base/strings/encoding.h>
#include <recurj/rjframework/libs/base/application/application.h>
using namespace rjBase;
CRJLogWriterFileModule::CRJLogWriterFileModule():
	m_sLimit(cLoggerFileLimitDefault) {}
CRJLogWriterFileModule::~CRJLogWriterFileModule() {}
bool CRJLogWriterFileModule::LogPrivate(const CRJStringByte& s) {
	bool rc=CRJLogWriterFile::LogPrivate(s);
	checkLimit();
	return rc;
}
void CRJLogWriterFileModule::checkLimit() {
	filepos_t l;
	if (m_clFile.GetLength(l)&&
		(size_t) l>m_sLimit.Get()) {
		Restart();
	}
}

