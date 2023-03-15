#pragma once
#include <recurj/rjframework/libs/base/datatools/datatools.h>
using namespace rjBase;
template<typename T>
class CRJListItem {
public:
	CRJListItem(): m_pNext(nullptr), m_pPrev(nullptr) {}
	virtual ~CRJListItem() {}
	void ItemRemove() {
#ifdef _DEBUG
		if ((m_pNext) &&
			(m_pNext == m_pPrev)) CRJDataTools::DebugBreak();
		if (m_pNext) {
			if (m_pNext == this) CRJDataTools::DebugBreak();
			m_pNext->SetPrev(m_pPrev);
		}
		if (m_pPrev) {
			if (m_pPrev == this) CRJDataTools::DebugBreak();
			m_pPrev->SetNext(m_pNext);
		}
#else
		if (m_pNext) m_pNext->SetPrev(m_pPrev);
		if (m_pPrev) m_pPrev->SetNext(m_pNext);
#endif
		ItemReset();
	}
	inline void ItemReset() {
		m_pNext = m_pPrev = nullptr;
	}
	inline T* GetNext() const {
		return m_pNext;
	}
	inline T* GetPrev() const {
		return m_pPrev;
	}
	inline void SetNext(T* p) {
		m_pNext = p;
	}
	inline void SetPrev(T* p) {
		m_pPrev = p;
	}
	inline bool IsEmpty() const {
		return m_pNext == nullptr && m_pPrev == nullptr;
	}
	T* m_pNext, * m_pPrev;
};
template<typename T>
class CRJListBase {
public:
	CRJListBase()noexcept { Reset(); };
	virtual~CRJListBase() { Clear(); }
	T* GetTop() const { return m_pTop; }
	T* GetBot() const { return m_pBot; }
	void Top(T& clElem) {
		clElem.ItemReset();
		clElem.m_pPrev = nullptr;
		clElem.m_pNext = m_pTop;
		if (!m_sCount) {
			m_pTop = &clElem;
			m_pBot = &clElem;
		}
		else {
			m_pTop->m_pPrev = &clElem;
			m_pTop = &clElem;
		}
		m_sCount++;
	}
	void Top(CRJListBase& clList) {
		try {
			const size_t l = clList.Count();
			if (l > 0) {
				T* pFirst = clList.m_pTop;
				T* pLast = clList.m_pBot;
				clList.Reset();
				if (!m_pTop) {
					m_pTop = pFirst;
					m_pBot = pLast;
				}
				else {
					pLast->SetNext(m_pTop);
					m_pTop->SetPrev(pLast);
					m_pTop = pFirst;
				}
				m_sCount += l;
			}
		}
		catch (...) {}
	}
	void Bot(T& clElem) {
		clElem.ItemReset();
		clElem.m_pNext = nullptr;
		clElem.m_pPrev = m_pBot;
		if (!m_sCount) {
			m_pTop = &clElem;
			m_pBot = &clElem;
		}
		else {
			m_pBot->m_pNext = &clElem;
			m_pBot = &clElem;
		}
		m_sCount++;
	}
	void Bot(CRJListBase& clList) {
		try {
			size_t l = clList.Count();
			if (l > 0) {
				T* pFirst = clList.m_pTop;
				T* pLast = clList.m_pBot;
				m_sCount += l;
				clList.Reset();
				if (!m_pBot) {
					m_pTop = pFirst;
					m_pBot = pLast;
				}
				else {
					pFirst->SetPrev(m_pBot);
					m_pBot->SetNext(pFirst);
					m_pBot = pLast;
				}
			}
		}
		catch (...) {}
	}
	T& Pop(T& clElem) {
		if (m_sCount == 1) {
			Reset();
			clElem.ItemReset();
		}
		else {
			if (m_pTop == &clElem) {
				m_pTop = clElem.GetNext();
				m_pTop->SetPrev(nullptr);
				clElem.ItemReset();
			}
			else if (m_pBot == &clElem) {
				m_pBot = clElem.GetPrev();
				m_pBot->SetNext(nullptr);
				clElem.ItemReset();
			}
			else clElem.ItemRemove();
			m_sCount--;
		}
		return clElem;
	}
	T* PopTop() {
		T* rc = m_pTop;
		if (rc) {
			m_pTop = m_pTop->GetNext();
			m_sCount--;
			if (!m_sCount) m_pBot = nullptr;
			else m_pTop->SetPrev(nullptr);
			rc->ItemReset();
		}
		return rc;
	}
	T* PopBot() {
		T* rc = m_pBot;
		if (rc) {
			m_pBot = m_pBot->GetPrev();
			m_sCount--;
			if (!m_sCount) m_pTop = nullptr;
			else m_pBot->SetNext(nullptr);

			rc->ItemReset();
		}
		return rc;
	}
	T* PopAll() {
		T* rc = m_pTop;
		Reset();
		return rc;
	}
	void Clear() {
		try {
			T* p = PopAll();
			while (p) {
				freeElem(p);
				p = p->GetNext();
			}
		}
		catch (...) {}
	}
	void Remove(T& clElem) {
		freeElem(&Pop(clElem));
	}
	void Reset() {
		m_sCount = 0;
		m_pTop = m_pBot = nullptr;
	}
	inline size_t Count() const { return m_sCount; }
protected:
	virtual void freeElem(T*) {}
protected:
	size_t m_sCount;
	T* m_pTop;
	T* m_pBot;
};
template<typename T>
class CRJList : public CRJListBase<T> {
public:
	virtual ~CRJList() {}
	class CIterator {
	public:
		explicit CIterator(T* pTop) :m_pElem(pTop) {}
		CIterator(const CIterator& o) = default;
		CIterator& operator=(const CIterator& o) = default;
		~CIterator() = default;
		CIterator& operator++() { m_pElem = m_pElem->m_pNext; return *this; }
		CIterator& operator--() { m_pElem = m_pElem->m_pPrev; return *this; }
		T& operator*() const { return *m_pElem; }
		bool operator!=(const CIterator& other) const { return m_pElem != other.m_pElem; }
		bool operator==(const CIterator& other) const { return m_pElem == other.m_pElem; }
	private:
		T* m_pElem;
	};
	CIterator begin() const {
		return CIterator(this->m_pTop);
	}
	CIterator end() const {
		return CIterator(nullptr);
	}
};
template<typename T>
class CRJListAllocated : public CRJList<T> {
public:
	virtual ~CRJListAllocated() { this->Clear(); }
	bool TopAllocated(T* p) {
		if (p == nullptr) return notAllocated();
		this->Top(*p);
		return true;
	}
	bool BotAllocated(T * p) {
		if (p == nullptr) return notAllocated();
		this->Bot(*p);
		return true;
	}
protected:
	virtual bool notAllocated() {
		throw - 1;
	}
	 void freeElem(T*p) override {
		 if (p == nullptr) return;
		 try {
			 delete p;
		 }
		 catch (...) {}
	 }

};
template<typename TBase,typename T>void FreeListElem(TBase *q) {
	if (q == nullptr) return;
	try {
		T* p=(T*)q;
		delete p;
	}
	catch(...) {}
}