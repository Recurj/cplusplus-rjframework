#include "stdafx.h"
#include <recurj/rjframework/libs/www/json/parser.h>
using namespace rjWWW;
CRJStringByte CJSONParser::s_JSONSeparators(",{[");
CJSONParser::CJSONParser(const CRJStringByte& s):
	CRJStringByteFlex(s) {}
CJSONParser::~CJSONParser() {}
bool CJSONParser::IsVar(const CRJStringByte& s, IJSONVar& clFunc) {
	if (s.InBrackets('{','}')) {
		CJSONParser parser(s);
		parser.TruncLeft(1).TruncRight(1).Trim(' ');
		return parser.ParseVar(clFunc);
	}
	return false;
}
bool CJSONParser::IsArray(const CRJStringByte& s, IJSONArray& clFunc) {
	if (s.InBrackets('[',']')) {
		CJSONParser parser(s);
		parser.TruncLeft(1).TruncRight(1).Trim(' ');
		return parser.ParseArray(clFunc);
	}
	return false;
}
bool CJSONParser::ParseVar(IJSONVar& clFunc) {
	auto call=[& clFunc](CRJStringByteFlex& s)->bool {
		CRJChainFinder clFinder;
		if (s.FindPos(':', clFinder)) {
			CRJStringByteFlex var, val;
			return clFunc(var.Slice(s, 0, clFinder.m_sFound).Trim(' '), val.Slice(s, clFinder.m_sFound+1, s.GetLength()).Trim(' '));
		}
		return false;
	};
	CJSONFunctorParser<decltype(call)>cb(call);
	return Parse(cb);
}
bool CJSONParser::ParseArray(IJSONArray& clFunc) {
	size_t lIndex=0;
	auto call=[& lIndex,& clFunc](CRJStringByteFlex& s)->bool {
		lIndex++;
		return clFunc(lIndex, s);
	};
	CJSONFunctorParser<decltype(call)>cb(call);
	return Parse(cb);
}
bool CJSONParser::Parse(IJSONParser& clFunc) {
	size_t i, l, p, lCount;
	const uint8_t* pHead;
	uint8_t ch, chBack;
	CRJChainFinderSet clFinder;
	CRJStringByteFlex str;
	while (FindPosInSet(CJSONParser::s_JSONSeparators, clFinder)) {
		pHead=GetHeadForRead();
		ch=pHead[clFinder.m_sFound];
		l=clFinder.m_sFound+1;
		if (ch==',') {
			if (!clFunc(str.SetFromData(pHead, clFinder.m_sFound).Trim(' '))) return false;
			TruncLeft(l);
			continue;
		}
		lCount=1;
		chBack=(uint8_t)((ch=='{')?
			'}':']');
		pHead+=l;
		for (i=l; i<m_sLength; i++, pHead++) {
			if (* pHead==chBack) {
				lCount--;
				if (!lCount) {
					l=i+1;
					p=0;
					if (str.Slice(*this, l, m_sLength).FindPos(',', clFinder)) {
						l+=clFinder.m_sFound;
						p=1;
					}
					else if (!str.Trim(' ').IsEmpty()) return false;
					if (!clFunc(str.SetFromData(GetHeadForRead(), l))) return false;
					TruncLeft(l+p);
					goto nextSearch;
				}
			}
			else if (* pHead==ch) lCount++;
		}
		return false;
		nextSearch:;
	}
	if (Trim(' ').IsEmpty()) return true;
	return clFunc(*this);
}

