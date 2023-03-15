#pragma once
#include <recurj/rjframework/modules/modbase.h>
#include <atomic>
using namespace std;
namespace rjBase {
	const int64_t constUOSLatchClosed=0;
	const int64_t constUOSLatchOpened=1;
	extern template 
	struct std::atomic<int64_t>;
	using CSTDAtomicInt64=std::atomic<int64_t>;
	class RJBASE_API CRJAtomicInt64:public CSTDAtomicInt64 {
	public:
		CRJAtomicInt64()noexcept:
		CSTDAtomicInt64(0) {}
		CRJAtomicInt64(int64_t v):
			CSTDAtomicInt64(v) {}
		inline int64_t anGet() const {
			return load();
		}
		inline void anSet(int64_t v) {
			store(v);
		}
		inline int64_t anInc() {
			return fetch_add(1)+1;
		}
		inline int64_t anDec() {
			return fetch_sub(1)-1;
		}
		inline int64_t anAdd(int64_t v) {
			return fetch_add(v)+v;
		}
		inline int64_t anSub(int64_t v) {
			return fetch_sub(v)-v;
		}
		inline bool anTest(int64_t v) const {
			return load()==v;
		};
		inline int64_t anSetIf(int64_t c, int64_t v) {
			return (compare_exchange_strong(c, v))?
				v:c;
		}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJAtomicInt64)
	};
	class RJBASE_API CRJLatch:public CRJAtomicInt64 {
	public:
		CRJLatch()noexcept {}
		CRJLatch(bool b):
			CRJAtomicInt64(b) {}
		inline void latchClose() {
			anSet(constUOSLatchClosed);
		}
		inline void latchOpen() {
			anSet(constUOSLatchOpened);
		}
		inline int64_t latchIfClosedThenOpen() {
			return anSetIf(constUOSLatchClosed, constUOSLatchOpened);
		}
		inline int64_t latchIfOpenedThenClose() {
			return anSetIf(constUOSLatchOpened, constUOSLatchClosed);
		};
		bool latchOpenChecked() {
			return latchIfClosedThenOpen()==constUOSLatchOpened;
		}
		bool latchCloseChecked() {
			return latchIfOpenedThenClose()==constUOSLatchClosed;
		}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJLatch)
	};
}

