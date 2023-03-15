#pragma once
#include <recurj/rjframework/libs/base/strings/byte/var.h>
#include <recurj/rjframework/modules/moddb.h>
using namespace rjBase;
namespace rjDb {
	class RJDB_API CRJDBMonthMarker:public CRJStringByteVarBuffer<8> {
	public:
		CRJDBMonthMarker();
		void Reset();
		void Set(int64_t, int64_t);
		bool CheckDTime(const uint8_t*);
		bool CheckLabel(const uint8_t*);
	};
}

