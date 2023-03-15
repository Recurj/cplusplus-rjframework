#pragma once
#include <recurj/rjframework/libs/base/strings/native.h>
#include <recurj/rjframework/libs/base/datatools/result.h>
namespace rjBase {
	class RJBASE_API RJLibrary:public CRJResult {
		using funcDecryptor=bool(*)(CRJStringByteVar&);
		enum eHLibraryErrors:int64_t {
			osLibraryNotLoaded=1, osLibraryInterfaceEmpty=2, osLibraryInterfaceNotFound=3,
		};
	public:
		RJLibrary();
		RJLibrary(RJLibrary&&
			);
		RJLibrary&operator=(RJLibrary&&
			);
		virtual~RJLibrary();
		virtual void Release();
		virtual bool Load(const CRJFileName&);
		void* GetInterface(const CRJStringByte&);
		inline bool IsReady() const {
			return m_hLibrary!=nullptr;
		}
		inline HLIBRARY GetHandle() const {
			return m_hLibrary;
		}
		inline void Attach(HLIBRARY h) {
			m_hLibrary=h;
		}
		inline void Detach() {
			m_hLibrary=nullptr;
		}
		static CRJFileName& AddLibExt(CRJFileName&);
		static bool ReFormat(CRJStringByteExpanded&);
		RJLibrary(const RJLibrary&)=delete;
		RJLibrary&operator=(const RJLibrary&)=delete;
	protected:
		void Swap(RJLibrary&&
			);
	public:
		static CRJStringUnicode s_libExt;
	private:
		HLIBRARY m_hLibrary;
	};
}

