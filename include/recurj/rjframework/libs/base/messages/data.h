#pragma once
#include <recurj/rjframework/libs/base/messages/message.h>
#include <recurj/rjframework/libs/base/datatools/value.h>
namespace rjBase {
	template<size_t D, size_t H=0>
	class CRJMessageData:public rjBase::CRJMessage {
	public:
		CRJMessageData(size_t c, funcFree lpFuncFree):
			rjBase::CRJMessage(c, lpFuncFree),
		m_lLength(0) {}
		CRJMessageData(size_t c,const CRJValue& v, funcFree lpFuncFree):
			rjBase::CRJMessage(c, lpFuncFree),
		m_lLength(0) {
			SetValue(v);
		}
		CRJMessageData():
			rjBase::CRJMessage(),
		m_lLength(0) {}
		virtual~CRJMessageData() {}
		CRJMessageData& SetValue(const CRJValue& v) {
			m_lLength=CRJValue::CopyDataBlock(m_bData+H, D, v.GetDataForRead(), v.GetLength());
			return *this;
		}
		CRJValue& GetValue(CRJValue& v) {
			v.m_pValuePointer=& m_bData[H];
			v.m_sValueSize=m_lLength;
			return v;
		}
		CRJValue CreateValue() {
			return CRJValue(& m_bData[H], m_lLength);
		}
		inline size_t GetLength() const {
			return m_lLength;
		}
		inline const uint8_t* GetHeadForRead() const {
			return & m_bData[H];
		}
		inline const uint8_t* GetHeadForReadPlain(size_t s=0) const {
			return & m_bData[s];
		}
		inline const char* GetHeadForReadC() const {
			return (char*)& m_bData[H];
		}
	protected:
		uint8_t m_bData[D+H];
		size_t m_lLength;
	};
}

