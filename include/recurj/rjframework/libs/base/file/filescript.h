#pragma once
#include <recurj/rjframework/libs/base/file/filewriter.h>
#include <recurj/rjframework/libs/base/datatools/result.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
namespace rjBase {
	constexpr size_t sErrorScriptCreate=1;
	constexpr size_t sErrorScriptOpen=2;
	constexpr size_t sErrorScriptLoad=3;
	constexpr size_t sErrorScriptNoCommand=4;
	constexpr size_t sErrorScriptVarNotDefined=5;
	constexpr size_t sErrorScriptVarInvalid=6;
	constexpr size_t sErrorScriptSubstitionTooLong=7;
	constexpr size_t sErrorScriptSave=8;
	constexpr size_t sErrorScriptAttributeInvalid=9;
	constexpr size_t sErrorScriptClose=10;
	class RJBASE_API CRJFileScript:public CRJFileWriter {
	public:
		using processFunc=bool(CRJFileScript::*)(CRJStringByteFlex&);
		CRJFileScript();
		virtual~CRJFileScript();
		bool RunText(CRJFileName&, CRJFileName&, CRJStringByteVar&);
		inline const CRJResult& GetResult() const {
			return m_clResult;
		}
		inline size_t GetLine() const {
			return m_sLine;
		}
	protected:
		virtual bool getVariable(const CRJStringByte&)=0;
		virtual bool closeScript();
		bool processText(CRJStringByteFlex&);
		bool run(CRJFileName&, CRJFileName&, CRJStringByteVar&, processFunc);
		bool substitute(const CRJStringByte&);
		bool setError(size_t);
	protected:
		CRJResult m_clResult;
		CRJStringByteFlex m_clSubstition;
		CRJStringByteExpanded m_clWork;
		size_t m_sLine;
	};
}

