#pragma once
#include <recurj/rjframework/libs/base/datatools/bytereader.h>
#include <recurj/rjframework/libs/base/datatools/bytewriter.h>
#include <recurj/rjframework/modules/modnet.h>
namespace rjNet {
	const size_t cUNetSocketInPoolSessionsBufSize=128;
	const size_t cUNetSocketInPoolSessionsMaxCmdSize=32;
	struct SDataApiRegionSessionsRegions {
		static const uint16_t cInfo=0;
	};
	struct SDataApiRegionSessionsRegionInfoCmds {
		static const uint16_t cPid=0;
		static const uint16_t cConfirm=1;
		static const uint16_t cStatistic=2;
		static const uint16_t cPing=3;
		static const uint16_t cDone=4;
	};
	struct SDataApiHeaderNetSessions {
		uint16_t lModule;
		uint16_t lRegion;
		uint16_t lCmd;
		uint16_t lOptions;
	};
	struct SNetClientDataApiPlugins {
		static const uint16_t cBase=0;
		static const uint16_t cSession=1;
	};
	class RJNET_API CRJByteReaderSessions:public CRJByteReader {
	public:
		using CRJByteReader::CRJByteReader;
		CRJByteReader& ReadHeader(SDataApiHeaderNetSessions&);
	};
	class RJNET_API CRJByteWriterSessions:public CRJByteWriter {
	public:
		using CRJByteWriter::CRJByteWriter;
		CRJByteWriter& WriteHeader(const SDataApiHeaderNetSessions&);
		CRJByteWriter& WriteHeader(uint16_t, uint16_t, uint16_t, uint16_t lOpts=0);
	};
}

