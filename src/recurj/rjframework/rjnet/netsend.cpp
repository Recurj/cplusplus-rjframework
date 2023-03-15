#include "stdafx.h"
#include <recurj/rjframework/libs/net/netsend.h>
#include <recurj/rjframework/libs/net/netsocket.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionnet.h>
using namespace rjNet;
RJ_ALLOCATOR(CRJNetSend::CRJNetSendElem, Allocator::Region::Net, AllocatorNet::Send) CRJNetSend::CRJNetSendElem::CRJNetSendElem():
	m_pNext(nullptr) {
	m_clStorage.SetStorage(m_bBlock,sizeof(m_bBlock));
}
CRJNetSend::CRJNetSendElem::~CRJNetSendElem() {}
CRJNetSend::CRJNetSend(CRJSocket& clSock):
	m_clSock(clSock),
m_stMetrics(SRJSocketProcessMetrics::s_stSendMetrics) {}
CRJNetSend::~CRJNetSend() {
	Reset();
}
bool CRJNetSend::SendResult(CRJStringByteFlex& block, int64_t l) {
	if (l<0) {
		auto lErr=CRJNet::GetNetLastError();
		if (lErr==EX_SOCKET_ERROR_BLOCK) return true;
		m_lError=lErr;
		return false;
	}
	block.ForwardSafed((size_t) l);
	m_sTotalSent+=(size_t) l;
	return true;
}
bool CRJNetSend::SendData(const CRJStringByte& s) {
	CRJStringByteFlex block(s);
	return send(block);
}
bool CRJNetSend::SendData(const uint8_t* p, size_t s) {
	CRJStringByteFlex block(p, s);
	return send(block);
}
bool CRJNetSend::SendReady() {
	m_sTotalSent=0;
	if (!HasData()) return true;
	return pop();
}
bool CRJNetSend::send(CRJStringByteFlex& block) {
	m_sTotalSent=0;
	if (HasData()) {
		if (m_sVolume>m_sThreshold) {
			Reset();
			return SetError(EX_SOCKET_ERROR_TOO_MANY_DEFFERED);
		}
		if (!pop()) return push(block);
	}
	return m_clSock.SocketSend(block,*this)?
		true:push(block);
}
bool CRJNetSend::push(CRJStringByteFlex& block) {
	size_t v;
	CRJNetSendElem* p;
	size_t s = block.GetLength();
	if (!m_pLast) {
		p=new CRJNetSendElem;
		if (!p) return SetError(EX_SOCKET_ERROR_SEND);
		m_pLast=m_pFirst=p;
	}
	v=m_pLast->GetStorage().GetCapacity();
	if (v>=s) {
		m_pLast->GetStorage().Cat(block);
		m_sVolume+=s;
		block.ForwardSafed(s);
	}
	else {
		size_t l;
		m_pLast->GetStorage().CatData(block.GetHeadForRead(), v);
		while (s>0) {
			p=new CRJNetSendElem;
			if (!p) return SetError(EX_SOCKET_ERROR_SEND);
			v=p->GetStorage().GetCapacity();
			l=(s>v)?v:s;
			p->GetStorage().CatData(block.GetHeadForRead(), l);
			m_sVolume+=l;
			block.ForwardSafed(l);
			s -= l;
			m_pLast=p;
		}
	}
	return true;
}
bool CRJNetSend::pop() {
	size_t s;
	CRJStringByteFlex block;
	while (m_pFirst) {
		s = block.Set(m_pFirst->GetStorage()).GetLength();
		if (!m_clSock.SocketSend(block,*this)) {
			if (block.IsNonEmpty()) {
				s -= block.GetLength();
				m_pFirst->Processed(s).Normalize();
				m_sVolume-=s;
			}
			return false;
		}
		m_sVolume-=s;
		freeFirst();
	}
	m_pLast=nullptr;
	return true;
}
void CRJNetSend::Reset() {
	CRJNetSendElem* p;
	while (m_pFirst) {
		p=m_pFirst->GetNext();
		delete m_pFirst;
		m_pFirst=p;
	}
	m_pLast=nullptr;
	m_sVolume=0;
}
void CRJNetSend::freeFirst() {
	try {
		CRJNetSendElem* p=m_pFirst;
		m_pFirst=p->GetNext();
		delete p;
	}
	catch(...) {}
}

