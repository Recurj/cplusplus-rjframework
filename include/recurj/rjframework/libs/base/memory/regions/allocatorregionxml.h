#pragma once
#include <cstdint>
namespace rjBase {
	struct AllocatorXML {
		enum Elem:size_t {
			Tag, Content, Attribute
		};
	};
}

