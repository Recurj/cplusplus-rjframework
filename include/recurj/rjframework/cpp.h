#pragma once
#define _CLASS_COPY_AND_ASSIGN_DELETE(__type) private: \
 __type(const __type &) = delete; \
 __type(__type&&)= delete; \
 __type &operator=(const __type &)= delete; \
 __type &operator=(__type &&)= delete;\


