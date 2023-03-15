#pragma once
#include <recurj/rjframework/modules/modbase.h>
#include <shared_mutex>
using namespace std;
namespace rjBase {
	class RJBASE_API CRJSharedSize final {
	public:
		CRJSharedSize():
			m_sValue(0) {}
		CRJSharedSize(size_t s):
			m_sValue(s) {}
		size_t Get() const {
			std::shared_lock lock(mutex);
			return m_sValue;
		}
		void Set(size_t s) {
			std::unique_lock lock(mutex);
			m_sValue=s;
		}
		size_t Next() {
			std::unique_lock lock(mutex);
			return ++m_sValue;
		}
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJSharedSize)
	private:
		mutable std::shared_mutex mutex;
		size_t m_sValue;
	};
}

