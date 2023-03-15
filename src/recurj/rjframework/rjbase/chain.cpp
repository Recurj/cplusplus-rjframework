#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/chain.h>
using namespace rjBase;
CRJChain::CRJChain():
	m_sLength(0) {}
CRJChain::~CRJChain() {}
bool CRJChainFinder::FoundPos(size_t v) {
	m_lDecision=eDecision::eUDecisionFound;
	m_sFound=v;
	return true;
}
bool CRJChainFinder::NotFoundPos() const {
	m_lDecision=eDecision::eUDecisionNotFound;
	return false;
}

