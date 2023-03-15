#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/modules/modwww.h>
namespace rjWWW {
	const size_t CRJWWWWJSONPreAllocated=1024;
	class RJWWW_API CJSONBuilder:public CRJStringByteExpanded {
	public:
		CJSONBuilder();
		virtual~CJSONBuilder();
		CJSONBuilder& AddNull(const CRJStringByte&);
		CJSONBuilder& AddBool(const CRJStringByte&,bool);
		CJSONBuilder& AddInt64(const CRJStringByte&, int64_t);
		CJSONBuilder& AddSize(const CRJStringByte&, size_t);
		CJSONBuilder& AddStringVar(const CRJStringByte&,const CRJStringByte&);
		CJSONBuilder& AddStringSimple(const CRJStringByte&,const CRJStringByte&);
		CJSONBuilder& AddStringConstant(const CRJStringByte&,const CRJStringByte&);
		CJSONBuilder& OpenArray(const CRJStringByte&);
		CJSONBuilder& OpenArray();
		CJSONBuilder& OpenObject();
		CJSONBuilder& OpenObject(const CRJStringByte&);
		CJSONBuilder& OpenObjectWithoutBrackets(const CRJStringByte&);
		CJSONBuilder& AddArrayData(const CRJStringByte&, size_t, int64_t[]);
		CJSONBuilder& AddNullInArray();
		CJSONBuilder& AddBoolInArray(bool);
		CJSONBuilder& AddInt64InArray(int64_t);
		CJSONBuilder& AddSizeInArray(size_t);
		CJSONBuilder& AddStringVarInArray(const CRJStringByte&);
		CJSONBuilder& AddStringConstantInArray(const CRJStringByte&);
		CJSONBuilder& CloseArray();
		CJSONBuilder& CloseArrayEmpty();
		CJSONBuilder& CloseObject();
		CJSONBuilder& CloseObjectEmpty();
		CJSONBuilder& NextObject();
		static bool IsValueNull(const CRJStringByte&);
		static bool IsValueTrue(const CRJStringByte&);
		static bool IsValuefalse(const CRJStringByte&);
		bool FinalArraySafed();
		bool FinalObjectSafed();
		bool FinalArray();
		bool FinalObject();
		static CRJStringByte s_clJSONEscaped;
		static CRJStringByte s_clJSONTrue;
		static CRJStringByte s_clJSONFalse;
		static CRJStringByte s_clJSONNull;
		static CRJStringByte s_clJSONEmptyString;
	};
	class CJSONObject {
	public:
		CJSONObject(CJSONBuilder& clJSON, size_t s=1):
			m_clJSON(clJSON),
		m_lCount(s) {
			m_clJSON.OpenObject();
		}
		CJSONObject(const CRJStringByte& v, CJSONBuilder& clJSON, size_t s=1):
			m_clJSON(clJSON),
		m_lCount(s) {
			m_clJSON.OpenObject(v);
		}
		~CJSONObject() {
			if (m_lCount) m_clJSON.CloseObject();
			else m_clJSON.CloseObjectEmpty();
		}
		CJSONBuilder& m_clJSON;
		size_t m_lCount;
	};
	class CJSONArray {
	public:
		CJSONArray(CJSONBuilder& clJSON, size_t s=1):
			m_clJSON(clJSON),
		m_lCount(s) {
			m_clJSON.OpenArray();
		}
		CJSONArray(const CRJStringByte& v, CJSONBuilder& clJSON, size_t s=1):
			m_clJSON(clJSON),
		m_lCount(s) {
			m_clJSON.OpenArray(v);
		}
		~CJSONArray() {
			if (m_lCount) m_clJSON.CloseArray();
			else m_clJSON.CloseArrayEmpty();
		}
		CJSONBuilder& m_clJSON;
		size_t m_lCount;
	};
}

