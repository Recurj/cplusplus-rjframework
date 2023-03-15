#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/strings/unicode/var.h>
#include <recurj/rjframework/libs/base/file/filewriter.h>
#include <recurj/rjframework/libs/xml/xmldefs.h>
using namespace rjBase;
namespace rjXml {
	const size_t XML_FILE_STORE_DEFUALT_BUFFER=4* 1024;
	const size_t XML_FILE_STORE_TAB_ARRAY=32;
	class RJXML_API CRJXMLFileStore {
	public:
		CRJXMLFileStore(CRJFileWriter&,bool r=false, size_t lBuf=XML_FILE_STORE_DEFUALT_BUFFER);
		virtual~CRJXMLFileStore();
		CRJXMLFileStore& Open();
		CRJXMLFileStore& Open(const CRJStringByte&);
		CRJXMLFileStore& Close();
		CRJXMLFileStore& TagOpen(const CRJStringByte&);
		CRJXMLFileStore& TagClose(const CRJStringByte&,bool);
		CRJXMLFileStore& TagClose();
		CRJXMLFileStore& TagContinue();
		CRJXMLFileStore& Content(const CRJStringByte&);
		CRJXMLFileStore& Attribute(const CRJStringByte&);
		CRJXMLFileStore& Attribute(const CRJStringByte&,const CRJStringByte&);
		CRJXMLFileStore& AttributeInt(const CRJStringByte&, int64_t);
		CRJXMLFileStore& AttributeSize(const CRJStringByte&, size_t);
		CRJXMLFileStore& AttributeBool(const CRJStringByte&,bool);
		CRJXMLFileStore& ContentInt(int64_t);
		CRJXMLFileStore& ContentBool(bool);
		CRJXMLFileStore& External(const CRJStringByte&);
		CRJXMLFileStore& Desciption(const CRJStringByte&);
		CRJXMLFileStore& DocType(const CRJStringByte&);
		CRJXMLFileStore& CData(const CRJStringByte&);
		CRJXMLFileStore& Comments(const CRJStringByte&);
		inline void SetReadableMode(bool b) {
			m_bReadable=b;
		}
		operator bool() const {
			return m_bOK;
		}
	protected:
		bool FlushBuf();
		bool Tabs(size_t);
		bool Save(const uint8_t*, size_t);
		bool Save(const CRJStringByte&);
		CRJXMLFileStore& SetResult(bool rc) {
			m_bOK=m_bOK&&
				rc;
			return *this;
		}
		bool SetError() {
			m_bOK=false;
			return false;
		}
	protected:
		static const uint8_t s_Tabs[XML_FILE_STORE_TAB_ARRAY];
		static const char s_TagChars[4];
		static const char s_AttrChars[4];
		CRJFileWriter& m_clFile;
		CRJStringByteExpanded m_clBuffer;
		int64_t m_lTabs;
		bool m_bReadable, m_bOK, b_bTagOpened;
	};
}

