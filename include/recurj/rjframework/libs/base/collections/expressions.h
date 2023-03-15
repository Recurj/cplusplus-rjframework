#pragma once
#include <recurj/rjframework/libs/base/datatools/datatools.h>
namespace rjBase {
	class RJBASE_API CRJExpressionContext {
	public:
		CRJExpressionContext();
		CRJExpressionContext(const CRJExpressionContext&);
		virtual~CRJExpressionContext();
	};
	template<typename T>
	class CRJArgs {
	public:
		CRJArgs(T* p, size_t c):
			m_pFirst(p),
		m_lCount(c) {}
		CRJArgs& Reset(T* p, size_t c) {
			m_pFirst=p;
			m_lCount=c;
			return *this;
		}
		CRJArgs& Push(T& e) {
			e.SetNextPtr(m_pFirst);
			m_pFirst=& e;
			m_lCount++;
			return *this;
		}
		T* Pop() {
			T* r=m_pFirst;
			if (m_pFirst) m_pFirst=m_pFirst->GetNext();
			return r;
		}
		void Pop(size_t c) {
			while (c>0&&
				m_pFirst) {
				c--;
				m_pFirst=m_pFirst->GetNext();
			}
		}
		inline bool IsEmpty() const {
			return m_pFirst==nullptr;
		}
		inline size_t Count() const {
			return m_lCount;
		}
	protected:
		T* m_pFirst;
		size_t m_lCount;
	};
	template<typename T,typename TArgs,typename TResult=T>
	class CRJFunction {
	public:
		using funcFree=bool(*)(CRJFunction&);
		CRJFunction():
			m_clFuncFree(nullptr),
		m_pNext(nullptr),
		m_pArgs(nullptr),
		m_lArgsCount(0) {}
		CRJFunction(funcFree f):
			m_clFuncFree(f),
		m_pNext(nullptr),
		m_pArgs(nullptr),
		m_lArgsCount(0) {}
		virtual~CRJFunction() {
			FreeArgs(m_pArgs);
		}
		virtual bool Call(CRJExpressionContext&, TArgs&, TResult&)=0;
		void Push(T& a) {
			m_lArgsCount++;
			if (m_pArgs==nullptr) m_pArgs=& a;
			else {
				T* q=m_pArgs;
				while (q->HasNext()) {
					q=q->GetNext();
				}
				q->SetNextPtr(& a);
			}
		}
		inline void SetNext(CRJFunction& n) {
			m_pNext=& n;
		}
		inline bool HasNext() const {
			return m_pNext!=nullptr;
		}
		inline CRJFunction* GetNext() const {
			return m_pNext;
		}
		inline funcFree GetFreeFunc() const {
			return m_clFuncFree;
		}
		inline T* GetArgs() const {
			return m_pArgs;
		}
		inline size_t GetArgsCount() const {
			return m_lArgsCount;
		}
		static bool Free(CRJFunction& f) {
			if (f.m_clFuncFree==nullptr) return true;
			return f.m_clFuncFree(f);
		}
		static bool FreePtr(CRJFunction* p) {
			if (!p) return true;
			try {
				return Free(* p);
			}
			catch(...) {}
			return false;
		}
		static bool Exec(CRJExpressionContext& clContext, CRJFunction* p, TResult& v) {
			if (p) {
				TArgs args(p->GetArgs(), p->GetArgsCount());
				if (!p->Call(clContext, args, v)) return false;
				p=p->GetNext();
				while (p) {
					args.Reset(p->GetArgs(), p->GetArgsCount()).Push(v);
					if (!p->Call(clContext, args, v)) return false;
					p=p->GetNext();
				}
			}
			return true;
		}
	protected:
		void FreeArgs(T* p) {
			T* q;
			while (p) {
				q=p->GetNext();
				T::Free(* p);
				p=q;
			}
		};
	protected:
		funcFree m_clFuncFree;
		CRJFunction* m_pNext;
		T* m_pArgs;
		size_t m_lArgsCount;
	};
}

