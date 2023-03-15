#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/xml/xmlelement.h>
using namespace rjBase;
namespace rjXml {
	class RJXML_API CRJXMLTagContent:public CRJXMLElement {
	public:
		static CRJXMLTagContent* CreateTagContent(const CRJStringByte&);
		virtual~CRJXMLTagContent();
		inline const CRJStringByte& GetValue() const {
			return m_clValue;
		};
		void*operator new(size_t);
		void operator delete(void*);
	protected:
		CRJXMLTagContent(const CRJStringByte&);
	protected:
		CRJStringByteValue m_clValue;
	protected:
		static CRJAllocatorRuntime m_clHeap;
	};
}

