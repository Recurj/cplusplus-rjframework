#pragma once
#include <recurj/rjframework/modules/modbase.h>
#include <cstdint>
namespace rjBase {
	constexpr size_t DataResutRegionBase=0;
	constexpr size_t DataResutRegionChain=1;
	constexpr size_t DataResutRegionDataCollector=2;
	constexpr size_t DataResutRegionOS=3;
	constexpr size_t DataResutRegionLibrary=4;
	constexpr size_t DataResutRegionLang=5;
	constexpr size_t DataResutRegionDB=6;
	constexpr size_t DataResutRegionScript=7;
	constexpr size_t DataResutRegionXml=10;
	constexpr size_t DataResutRegionNet=11;
	constexpr size_t DataResutRegionHttp=12;
	constexpr size_t DataResutRegionPDF=13;
	constexpr size_t DataResutRegionXML=14;
	constexpr size_t DataResutRegionRun24=1024;
	constexpr size_t DataResutRegionReciprocity=1025;
	constexpr size_t DataResutRegionLangLexical=2048;
	constexpr size_t DataResutRegionLangSyntax=2049;
	constexpr size_t DataResutApplication=10* 1024;
	class CRJStringByteVar;
	class RJBASE_API CRJResult {
	public:
		CRJResult()noexcept;
		CRJResult(size_t)noexcept;
		CRJResult(size_t, int64_t)noexcept;
		virtual~CRJResult() {}
		virtual CRJStringByteVar& GetErrorMsg(CRJStringByteVar&) const;
		void Region(size_t region) {
			m_sRegion=region;
		}
		void Result(int64_t rc) {
			m_lResult=rc;
		}
		size_t GetRegion() const {
			return m_sRegion;
		}
		int64_t GetResult() const {
			return m_lResult;
		}
		void SetError(size_t region, int64_t rc) {
			m_sRegion=region;
			m_lResult=rc;
		}
		void ResetError() {
			m_lResult=0;
		}
		inline bool OK() const {
			return m_lResult==0;
		};
		inline bool HasError() const {
			return m_lResult!=0;
		};
	protected:
		size_t m_sRegion;
		int64_t m_lResult;
	};
	class RJBASE_API CRJResultWithSize:public CRJResult {
	public:
		CRJResultWithSize()noexcept;
		CRJResultWithSize(size_t, int64_t)noexcept;
		virtual~CRJResultWithSize() {}
		virtual CRJStringByteVar& GetErrorMsg(CRJStringByteVar&) const;
		void ResultAndValue(int64_t r, size_t v) {
			m_lResult=r;
			m_sValue=v;
		}
		void Value(size_t v) {
			m_sValue=v;
		}
		size_t GetValue() const {
			return m_sValue;
		}
	protected:
		size_t m_sValue;
	};
}

