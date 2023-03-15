#pragma once
#include <cstdint>
namespace rjBase {
	struct AllocatorMessage {
		enum Elem:size_t {
			Cmd, Log, Params, NetSocket, NetSession, NetSessionDone, HttpReplyFile, HttpReplyJSON,
			ServerLong, ServerShort, Local, Outside,
			App=1024
		};
	};
}

