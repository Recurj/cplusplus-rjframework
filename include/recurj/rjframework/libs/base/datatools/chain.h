#pragma once
#include <recurj/rjframework/libs/base/datatools/result.h>
namespace rjBase {
	class RJBASE_API CRJChain {
	public:
		enum eUChainErrors:int64_t {
			Exception=1, NoMemory, Undefined, Decode, NoSpace, NoPointer, BadPos, BadFormat, BadLength,
			BadSize, WasAllocated, WasTruncated,
		};
		inline size_t GetLength() const {return m_sLength;};
		inline bool IsNonEmpty() const {return m_sLength>0;};
		inline bool IsEmpty() const {return m_sLength==0;};
	protected:
		CRJChain();
		virtual~CRJChain();
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJChain)
	protected:
		size_t m_sLength;
	};
	struct RJBASE_API CRJChainFinder {
	public:
		enum 
		class eDecision {
			eUDecisionNotFound=0, eUDecisionFound=1
		};
		bool FoundPos(size_t);
		bool NotFoundPos() const;
		size_t m_sFound;
		mutable eDecision m_lDecision;
	};
	struct RJBASE_API CRJChainFinderSet:public CRJChainFinder {
		size_t m_sIndex;
	};
}

