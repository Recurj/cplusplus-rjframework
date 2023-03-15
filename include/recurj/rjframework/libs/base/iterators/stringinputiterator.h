#pragma once
#include <assert.h>
#include <recurj/rjframework/modules/modbase.h>
using namespace std;
template<typename T>
class CUStringInputIterator {
public:
	typedef CUStringInputIterator self_type;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	using iterator_category=std::input_iterator_tag;
	typedef T reference;
	typedef const T* pointer;
public:
	explicit CUStringInputIterator(pointer pData, size_t lSize, size_t const lIndex=0):
		m_pData(pData),
	m_lSize(lSize),
	m_lIndex(lIndex) {}
	CUStringInputIterator(const CUStringInputIterator& o)=default;
	CUStringInputIterator&operator=(const CUStringInputIterator& o)=default;
	~CUStringInputIterator()=default;
	CUStringInputIterator&operator++() {
		if (m_lIndex>=m_lSize)throw std::out_of_range("Iterator cannot be incremented past the end of range.");
		++m_lIndex;
		return *this;
	}
	CUStringInputIterator operator++(int) {
		CUStringInputIterator tmp=*this;
		++*this;
		return tmp;
	}
	bool operator==(const CUStringInputIterator& other) const {
		return compatible(other)&&
			m_lIndex==other.m_lIndex;
	}
	bool operator!=(const CUStringInputIterator& other) const {
		return !(*this==other);
	}
	T operator*() const {
		if (m_pData==nullptr)throw std::bad_function_call();
		return *(m_pData+m_lIndex);
	}
	T operator->() const {
		if (m_pData==nullptr)throw std::bad_function_call();
		return *(m_pData+m_lIndex);
	}
private:
	bool compatible(const CUStringInputIterator& other) const {
		return m_pData==other.m_pData;
	}
private:
	pointer m_pData=nullptr;
	size_t m_lSize, m_lIndex=0;
};

