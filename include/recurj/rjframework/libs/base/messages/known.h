#pragma once
#include <cstdint>
namespace rjBase {
	enum eRJMessageRegions:size_t {
		Base=0, Log, Net, WWW, Appl=1024
	};
	enum eRJMessagesRegionBase:size_t {
		Quit=0, Ping=1,
	};
	enum eRJMessagesRegionNet:size_t {
		Accepted=0, DataApi, SessionFind, SessionSend, SessionDone
	};
	enum eRJMessagesRegionWWW {
		Reply=0, Error, File, FileHead, JSON
	};
}

