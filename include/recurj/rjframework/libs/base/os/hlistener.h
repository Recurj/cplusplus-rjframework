#pragma once
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/datatools/result.h>
#include <recurj/rjframework/libs/base/os/hevent.h>
namespace rjBase {
#ifdef _rjOS_WIN
	struct RJBASE_API CRJEventListener {
		struct ListenerMapElem {
			RJHandle* pevent;
			CRJEventHandler::RJHandler handler;
		};
		CRJEventListener();
		void Bind(size_t, ListenerMapElem*, HOS*);
		void Wait(CRJEventHandler&, size_t);
		bool Insert(RJHandle&, CRJEventHandler::RJHandler);
		bool Remove(RJHandle&);
		bool RemoveByIndex(size_t, RJHandle&, size_t[]);
		void Rebuild();
		void Reset();
		inline bool HandleSetError(int64_t e) {
			m_clResult.SetError(DataResutRegionOS, e);
			return false;
		}
		inline bool HasSpace() const {
			return m_sCount<m_sMax;
		}
		inline size_t Count() const {
			return m_sCount;
		}
		ListenerMapElem* m_pElems;
		HOS* m_pEvents;
		size_t* m_pIndexes;
		CRJResult m_clResult;
		size_t m_sMax, m_sCount;
	};
	template<size_t N>
	struct RJListenerMapData {
		RJListenerMapData(CRJEventListener& listener) {
			listener.Bind(N, m_clElems, m_hEvents);
		}
		CRJEventListener::ListenerMapElem m_clElems[N];
		HOS m_hEvents[N];
	};
#endif 
}

