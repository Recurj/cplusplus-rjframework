#pragma once
#include <recurj/rjframework/modules/modbase.h>
namespace rjBase {
	class RJBASE_API RJHandle {
		friend class CRJHandleCreator;
	public:
		bool HandleRelease();
		inline bool HandleIsReady() {
			return RJHandle::HandleIsValid(m_hHandle);
		}
		operator HOS() const {
			return m_hHandle;
		}
		static bool HandleIsValid(const HOS);
	protected:
		RJHandle();
		virtual~RJHandle();
		bool HandleAttach(HOS);
		RJHandle& HandleOSError();
		void HandleClear();
	protected:
		virtual bool HandleDestroy();
		virtual void HandleAttached();
		_CLASS_COPY_AND_ASSIGN_DELETE(RJHandle)
	protected:
		HOS m_hHandle;
	};
	class CRJHandleCreator {
	public:
		CRJHandleCreator(RJHandle& h):
			m_clHandle(h) {
			m_bResult=false;
		}
		~CRJHandleCreator() {
			if (!m_bResult) m_clHandle.HandleRelease();
		}
		inline bool SetResult(bool b) {
			m_bResult=b;
			return b;
		}
		inline bool GetResult() const {
			return m_bResult;
		}
	protected:
		RJHandle& m_clHandle;
		bool m_bResult;
	};
}

