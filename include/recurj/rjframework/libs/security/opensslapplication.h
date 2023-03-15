#pragma once
#include <mutex>
#include <recurj/rjframework/libs/base/application/application.h>
#include <recurj/rjframework/libs/base/memory/allocatorany.h>
#include <recurj/rjframework/libs/security/security.h>
namespace rjSecurity {
	class RJSECURITY_API CRJSecurityApplication {
	public:
		CRJSecurityApplication();
		virtual~CRJSecurityApplication();
		bool IsReady() const {
			return m_bReady;
		}
	private:
		bool PrepareOpenSSLApplication();
		void CloseOpenSSLApplication();
	public:
		static std::mutex** m_pTableOfCS;
		static size_t m_lCount;
		static int m_lSSLAppDataIndex;
		static CRJAllocatorRuntime m_clHeap;
		bool m_bReady;
	};
}

