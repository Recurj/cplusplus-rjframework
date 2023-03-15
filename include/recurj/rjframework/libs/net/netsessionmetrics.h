#pragma once
#include <recurj/rjframework/modules/modnet.h>
namespace rjNet {
	struct SUNetSessionMetrics {
		SUNetSessionMetrics() {
			lVolume=lInUse=0;
		}
		bool operator>(const SUNetSessionMetrics& s) {
			if (!lVolume) return false;
			else if (!s.lVolume) return true;
			if (s.lInUse==0) {
				if (lInUse==0) return lVolume>s.lVolume;
				else return false;
			}
			else if (lInUse==0) return true;
			return lInUse* s.lVolume<s.lInUse* lVolume;
		}
		size_t lVolume;
		size_t lInUse;
	};
}

