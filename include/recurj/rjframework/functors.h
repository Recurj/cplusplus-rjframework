#pragma once
#include <cstddef>
template<typename...Types>
class IRJFunctor {
public:
	virtual void operator()(Types...) = 0;
};
template<typename...Types>
class IRJFunctorBool {
public:
	virtual bool operator()(Types...) = 0;
};
template<typename TResult, typename...Types>
class IRJFunctorResult {
public:
	virtual TResult operator()(Types...) = 0;
};
template<typename IFunctor, typename TLambda, typename...Types>
class CRJFunctor :public IFunctor {
public:
	CRJFunctor(TLambda& f) :
		func(f) {}
	void operator()(Types...args) override {
		func(args...);
	}
	TLambda& func;
};
template<typename IFunctor, typename TLambda, typename...Types>
class CRJFunctorBool :public IFunctor {
public:
	CRJFunctorBool(TLambda& f) :
		func(f) {}
	bool operator()(Types...args) override {
		return func(args...);
	}
	TLambda& func;
};
template<typename TResult, typename IFunctor, typename TLambda, typename...Types>
class CRJFunctorResult :public IFunctor {
public:
	CRJFunctorResult(TLambda& f) :
		func(f) {}
	TResult operator()(Types...args) override {
		return func(args...);
	}
	TLambda& func;
};
