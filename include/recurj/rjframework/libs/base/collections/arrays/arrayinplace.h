#pragma once
#include <local/rj/libs/base/datatools/datatools.h>
 template<typename T, size_t V>
class CRJArrayInPlace {
public:
#pragma warning(suppress: 26495)
public:
	CRJArrayInPlace()noexcept {
		m_sCount=0;
	}
	~CRJArrayInPlace() {}
	inline size_t Count() const {
		return m_sCount;
	}
	inline void Clear() {
		m_sCount=0;
	}
	using iterator=CRJRangeLoop<T>;
	iterator begin() const {
		return iterator(m_stData, 0);
	}
	iterator end() const {
		return iterator(m_stData, m_sCount);
	}
	bool SortedDel(const T& val) {
		size_t i, first=0, last=m_sCount;
		while (first<last) {
			i=(last-first)>>1;
			i+=first;
			if (m_stData[i]==val) {
				MoveLeft(i);
				m_sCount--;
				return true;
			}
			else if (m_stData[i]>val) last=i;
			else first=i+1;
		}
		return false;
	}
	bool SortedAdd(const T& val) {
		size_t i=0, first=0, last=m_sCount;
		while (first<last) {
			i=(last-first)>>1;
			i+=first;
			if (m_stData[i]==val) {
				return true;
			}
			else if (m_stData[i]>val) last=i;
			else first=i+1;
		}
		if (m_sCount<V) {
			MoveRight(i);
			m_stData[i]=val;
			m_sCount++;
			return true;
		}
		return false;
	}
protected:
	void MoveRight(size_t f) {
		size_t j, i=m_sCount;
		while (i>f) {
			j=i-1;
			m_stData[i]=m_stData[j];
			i=j;
		}
	}
	void MoveLeft(size_t i) {
		size_t j;
		while (i<m_sCount) {
			j=i+1;
			m_stData[i]=m_stData[j];
			i=j;
		}
	}
protected:
	T m_stData[V];
	size_t m_sCount;
};

