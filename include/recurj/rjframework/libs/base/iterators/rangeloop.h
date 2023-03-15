#pragma once
#include <assert.h>
#include <recurj/rjframework/modules/modbase.h>
using namespace std;
template<typename T>
class CRJRangeLoop {
public:
	using pointer=const T*;
public:
	explicit CRJRangeLoop(pointer pData, size_t sIndex):
		m_pData(pData),
	m_sIndex(sIndex) {}
	CRJRangeLoop(const CRJRangeLoop& o)=default;
	CRJRangeLoop&operator=(const CRJRangeLoop& o)=default;
	~CRJRangeLoop()=default;
	CRJRangeLoop&operator++() {
		++m_sIndex;
		return *this;
	}
	const T&operator*() const {
		return m_pData[m_sIndex];
	}
	bool operator!=(const CRJRangeLoop& other) const {
		return m_sIndex!=other.m_sIndex;
	}
	bool operator==(const CRJRangeLoop& other) const {
		return m_sIndex==other.m_sIndex;
	}
private:
	pointer m_pData;
	size_t m_sIndex;
};
template<typename T>
class CRJRangeLoopMutable {
public:
	using pointer=T*;
public:
	explicit CRJRangeLoopMutable(pointer pData, size_t sIndex):
		m_pData(pData),
	m_sIndex(sIndex) {}
	CRJRangeLoopMutable(const CRJRangeLoopMutable& o)=default;
	CRJRangeLoopMutable&operator=(const CRJRangeLoopMutable& o)=default;
	~CRJRangeLoopMutable()=default;
	CRJRangeLoopMutable&operator++() {
		++m_sIndex;
		return *this;
	}
	T&operator*() const {
		return m_pData[m_sIndex];
	}
	bool operator!=(const CRJRangeLoopMutable& other) const {
		return m_sIndex!=other.m_sIndex;
	}
private:
	pointer m_pData;
	size_t m_sSize, m_sIndex;
};

