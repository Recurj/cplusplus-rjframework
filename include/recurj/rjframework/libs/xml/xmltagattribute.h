#pragma once
#include <recurj/rjframework/libs/base/collections/lists/defs.h>
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/xml/xmldefs.h>
using namespace rjBase;
namespace rjXml {
	class CRJXMLFileStore;
	class RJXML_API CRJXMLTagAttribute:public CRJListItem<CRJXMLTagAttribute> {
	public:
		static CRJXMLTagAttribute* CreateAttribute(const CRJStringByte&,const CRJStringByte&);
		virtual~CRJXMLTagAttribute();
		void GetReturn(CRJStringByteFlex&);
		bool GetReturnNum(int64_t&);
		bool GetReturnSize(size_t&);
		bool GetReturnDouble(double&);
		bool GetReturnBool(bool&);
		int64_t GetReturnValidInt64(int64_t);
		int32_t GetReturnValidInt32(int32_t);
		size_t GetReturnValidSize(size_t);
		double GetReturnValidDouble(double);
		bool GetReturnValidBool(bool);
		void operator delete(void*);
		inline bool UpdateValue(const CRJStringByte& s) {
			return m_clValue.Set(s);
		}
		inline const CRJStringByte& GetName() const {
			return m_clName;
		};
		inline const CRJStringByte& GetValue() const {
			return m_clValue;
		};
		inline bool IsThatName(const CRJStringByte& clName) const {
			return clName==m_clName;
		}
	protected:
		void*operator new(size_t);
		CRJXMLTagAttribute(const CRJStringByte&,const CRJStringByte&);
		CRJStringByteValue m_clName;
		CRJStringByteExpanded m_clValue;
	protected:
		static CRJAllocatorRuntime m_clHeap;
	};
	class CRJXMLTagAttributes : public CRJList<CRJXMLTagAttribute> {
	protected:
		void freeElem(CRJXMLTagAttribute* p) override;
	};
}

