#pragma once
#include <cstdint>
namespace rjBase {
	struct AllocatorNet {
		enum Elem:size_t {
			Send, EventSessionPooled, TaskPool
		};
	};
}

