#pragma once
#include <recurj/rjframework/libs/base/datatools/datatools.h>
 template<typename T>
class CRJCompare {
public:
	using funcComp=bool(CRJCompare<T>::*)(const T&) const;
	CRJCompare(const T& p, size_t lAction):
		m_Model(p),
	m_lAction(lAction& 7) {}
	bool Compare(const T& p) {
		return (this->* s_funcTable[m_lAction])(p);
	}
	bool funcEQ(const T& v) const {
		return v==m_Model;
	}
	bool funcNotEQ(const T& v) const {
		return v!=m_Model;
	}
	bool funcMore(const T& v) const {
		return v>m_Model;
	}
	bool funcLess(const T& v) const {
		return v<m_Model;
	}
	bool funcMoreOrEQ(const T& v) const {
		return v>=m_Model;
	}
	bool funcLessOrEQ(const T& v) const {
		return v<=m_Model;
	}
protected:
	static funcComp s_funcTable[8];
	T m_Model;
	size_t m_lAction;
};
template<typename T>
using funcCompare=bool(CRJCompare<T>::*)(const T&) const;
template<typename T>funcCompare<T>CRJCompare<T>::s_funcTable[8]= {
	& CRJCompare::funcEQ,& CRJCompare::funcNotEQ,& CRJCompare::funcMore,& CRJCompare::funcLess,
	& CRJCompare::funcMoreOrEQ,& CRJCompare::funcLessOrEQ,& CRJCompare::funcEQ,& CRJCompare::funcEQ
};
namespace rjBase {
	RJBASE_TEMPLATE template 
	class RJBASE_API CRJCompare<size_t>;
	RJBASE_TEMPLATE template 
	class RJBASE_API CRJCompare<int64_t>;
	using CRJCompareSize=RJBASE_API CRJCompare<size_t>;
	using CRJCompareInt64=RJBASE_API CRJCompare<int64_t>;
}

