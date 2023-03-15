#pragma once
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/xml/xmltagattribute.h>
#include <recurj/rjframework/libs/xml/xmltagcontent.h>
#include <recurj/rjframework/functors.h>
namespace rjXml {
	using IXMLEnumerateElements=IRJFunctorBool<CRJXMLElement&>;
	using IXMLEnumerateTags=IRJFunctorBool<CRJXMLTag&>;
	using IXMLEnumerateContents=IRJFunctorBool<CRJXMLTagContent&>;
	using IXMLEnumerateAttributes=IRJFunctorBool<CRJXMLTagAttribute&>;
	template<typename TFunc>
	using CXMLEnumerateElements=CRJFunctorBool<IXMLEnumerateElements, TFunc, CRJXMLElement&>;
	template<typename TFunc>
	using CXMLEnumerateTags=CRJFunctorBool<IXMLEnumerateTags, TFunc, CRJXMLTag&>;
	template<typename TFunc>
	using CXMLEnumerateContents=CRJFunctorBool<IXMLEnumerateContents, TFunc, CRJXMLTagContent&>;
	template<typename TFunc>
	using CXMLEnumerateAttributes=CRJFunctorBool<IXMLEnumerateAttributes, TFunc, CRJXMLTagAttribute&>;
	class CRJXMLFileStore;
//	using CRJXMLTagAttributes=CRJListBase<CRJXMLTagAttribute>;
	class RJXML_API CRJXMLTag:public CRJXMLElement {
	public:
		static CRJXMLTag* CreateTag(CRJXMLTag*,const CRJStringByte&);
		virtual~CRJXMLTag();
		void AddElement(CRJXMLElement&);
		CRJXMLTag* AddTag(const CRJStringByte&);
		bool AddContent(const CRJStringByte&);
		bool UpdateContent(const CRJStringByte&);
		CRJStringByteVar& GetContent(CRJStringByteVar&) const;
		CRJStringByteVar& GetContentByte(CRJStringByteVar&,bool bErr=true) const;
		CRJStringUnicodeVar& GetContentUnicode(CRJStringUnicodeVar&,bool bErr=true) const;
		CRJFileName& GetContentFileName(CRJFileName&,bool,bool bErr=true) const;
		bool AddAttribute(const CRJStringByte&,const CRJStringByte&);
		bool ForEachElement(IXMLEnumerateElements&) const;
		bool ForEachTag(IXMLEnumerateTags&) const;
		bool ForEachContent(IXMLEnumerateContents&) const;
		bool HasContext() const;
		size_t HowManyTags() const;
		bool ForEachAttribute(IXMLEnumerateAttributes&) const;
		CRJXMLTagAttribute* GetAttribute(const CRJStringByte&);
		bool GetAttributeValue(const CRJStringByte&, CRJStringByteFlex&) const;
		bool GetAttributeValueNum(const CRJStringByte&, int64_t&,bool&) const;
		bool GetAttributeValueSize(const CRJStringByte&, size_t&,bool&) const;
		bool GetAttributeValueDouble(const CRJStringByte&,double&,bool&) const;
		bool GetAttributeValueBool(const CRJStringByte&,bool&,bool&) const;
		CRJStringByteFlex& GetAttributeValueValid(const CRJStringByte&, CRJStringByteFlex&,const CRJStringByte&);
		bool LogOutAttributes(const CRJStringByte&,const CRJStringByte&);
		CRJXMLTag* GetFirstTag() const;
		CRJXMLTag* GetNextTag(CRJXMLElement&) const;
		CRJXMLTagAttribute* GetNextAttribute(CRJXMLTagAttribute&) const;
		inline CRJXMLTagAttribute* GetFirstAttribute() const {
			return m_clAttributes.GetTop();
		}
		inline CRJXMLTag* GetParent() const {
			return m_pParent;
		}
		inline const CRJStringByte& GetName() const {
			return m_clName;
		}
		inline bool IsThatName(const CRJStringByte& clName) {
			return m_clName==clName;
		}
		inline const CRJXMLTagAttributes& GetAttributes() const {
			return m_clAttributes;
		}
		void*operator new(size_t);
		void operator delete(void*);
	protected:
		CRJXMLTag(CRJXMLTag*,const CRJStringByte&);
		CRJXMLTag();
		void FreeAttributes();
		void FreeElements();
		bool FindAttribute(const CRJStringByte&, IXMLEnumerateAttributes&) const;
		void LogOutAttribute(CRJLogger&,const CRJStringByte&,const CRJStringByte&);
	protected:
		static CRJStringByte s_tagErrorHeader;
		static CRJStringByte s_tagErrorDetails;
		CRJXMLTag* m_pParent;
		CRJStringByteAllocated m_clName;
		CRJXMLListElements m_clElements;
		CRJXMLTagAttributes m_clAttributes;
	protected:
		static CRJAllocatorRuntime m_clHeap;
	};
	class RJXML_API CRJXMLTagStatic:public CRJXMLTag {
	public:
		CRJXMLTagStatic(){}
		virtual~CRJXMLTagStatic() {}
		void operator delete(void*) {}
	};
}

