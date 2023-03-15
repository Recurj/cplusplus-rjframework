#pragma once
#include <recurj/rjframework/libs/base/datatools/datatools.h>
 template<typename T,typename Callable,typename...Args>void foreach(T elem, Callable op, Args...args) {
	const T::typeOfElem* p=elem.GetHeadForRead();
	for (size_t i=0; i<elem.GetLength(); i++, p++) {
		std::invoke(elem, op, args,* p);
	}
}
template<typename T,typename Callable,typename...Args>void foreachUpdate(T elem, Callable op, Args...args) {
	T::typeOfElem* p=elem.GetHeadForUpdate();
	for (size_t i=0; i<elem.GetLength(); i++, p++) {
		std::invoke(elem, op, args,* p);
	}
}

