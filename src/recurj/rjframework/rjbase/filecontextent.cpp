#include "stdafx.h"
#include <recurj/rjframework/libs/base/file/filecontent.h>
#include <recurj/rjframework/libs/base/file/filereader.h>
#include <recurj/rjframework/libs/base/file/filewriter.h>
using namespace rjBase;
CRJFileContent::CRJFileContent() {}
CRJFileContent::~CRJFileContent() {}
bool CRJFileContent::Load(CRJFileName& fn) {
	CRJFileReader fr;
	if (fr.OpenEncoding(fn)) {
		size_t l=fr.GetSize();
		encode.SetEncoding(fr.GetEncode().GetEncoding());
		if (HasNeededSize(l)&&
			fr.Read(GetHeadForUpdate(), l)) {
			SetLength(l);
			fr.Close();
			return true;
		}
	}
	return false;
}
bool CRJFileContent::Save(CRJFileName& fn) {
	CRJFileWriter fw;
	if (fw.Create(fn)) {
		encode.Save(fw);
		return fw.Write(*this);
	}
	return false;
}

