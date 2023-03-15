#pragma once
#include <recurj/rjframework/libs/xml/xmlfilestore.h>
#include <recurj/rjframework/libs/xml/xmlerror.h>
#include <recurj/rjframework/libs/xml/xmltag.h>
#include <recurj/rjframework/libs/xml/xmlparserbase.h>
#include <functional>
namespace rjXml {
	class RJXML_API CRJXMLCollection:public CRJXMLParserObserver {
	public:
		CRJXMLCollection(bool bTruncContext=false);
		virtual~CRJXMLCollection();
		bool OnTagEntry(const CRJStringByte&) override;
		bool OnTagClose(const CRJStringByte&) override;
		bool OnTagContent(const CRJStringByte&, CRJStringByteVar&) override;
		bool OnTagAttribute(const CRJStringByte&,const CRJStringByte&, CRJStringByteVar&) override;
		virtual void OnRemoveRootTag();
		virtual void FreeAll();
		virtual bool Save2File(CRJXMLFileStore&,bool,const CRJStringByte&);
		CRJXMLTag* IsRootTagName(const CRJStringByte&) const;
		bool LoadFromFile(CRJFileName&);
		bool LoadFromFile(CRJFileName&,const CRJStringByte&, IXMLEnumerateTags&);
		bool LoadFromData(const CRJStringByte&);
		CRJStringByte& GetError(CRJStringByteVar& s) {
			return m_clError.GetErrorMsg(s.SetEmpty());
		}
		inline CRJXMLTag* GetRootTag() const {
			return m_pTagRoot;
		}
		static bool LoadFromFileAndWalk(const CRJStringByte&, CRJFileName&,const CRJStringByte&, IXMLEnumerateTags&);
		static bool LoadFromFileAndProcessRoot(const CRJStringByte&, CRJFileName&,const CRJStringByte&, IXMLEnumerateTags&);
		static bool LoadFromFileError(const CRJStringByte&, CRJFileName&,const CRJStringByte&);
		static const CRJStringByte& GetTextLoadings() {
			return s_loadings;
		}
	private:
		virtual bool Save2File(CRJXMLFileStore&, CRJXMLTag&);
	protected:
		static CRJStringByte s_loadings;
		static CRJStringByte s_fileError;
		static CRJStringByte s_rootTagAbsent;
		static CRJStringByte s_rootTagInvalid;
		static CRJStringByte s_contentTrunc;
		static CRJAllocatorRuntime m_clHeap;
		CRJXMLTag* m_pTagRoot,* m_pTagWork;
		bool m_bTruncContext;
	};
	class RJXML_API CRJXMLCollectionNotSingleRoot:public CRJXMLCollection {
	public:
		CRJXMLCollectionNotSingleRoot(bool bTruncContext=false):
			CRJXMLCollection(bTruncContext) {
			m_pTagWork=m_pTagRoot=& m_clTagRoot;
		}
		virtual~CRJXMLCollectionNotSingleRoot() {
			FreeAll();
		}
	protected:
		CRJXMLTagStatic m_clTagRoot;
	};
}

