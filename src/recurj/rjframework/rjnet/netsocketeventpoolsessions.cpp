#include "stdafx.h"
#include <recurj/rjframework/libs/net/netsocketeventsessionpooled.h>
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
#include <recurj/rjframework/libs/net/ipc/netsocketipctransmit.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionnet.h>
#include <recurj/rjframework/libs/net/tasks/nettaskpool.h>
using namespace rjNet;
RJ_ALLOCATOR(CRJNetSocketEventSessionPooled::CWaitElem, Allocator::Region::Net, AllocatorNet::EventSessionPooled) 
CRJNetSocketEventSessionPooled::CRJNetSocketEventSessionPooled(IRJSocketHolder&holder):
	CRJSocketTCPPool(holder),m_clNetSend(SocketGetMe()) {
	m_pid=0;
	m_lSeq=0;
}
CRJNetSocketEventSessionPooled::~CRJNetSocketEventSessionPooled() {}
void CRJNetSocketEventSessionPooled::PoolRelease() {
	m_clWaitList.Clear();
	CRJSocketTCPPool::PoolRelease();
}
void CRJNetSocketEventSessionPooled::OnNetEventRead() {
	CRJStringByteVar& storage=m_clStorage.GetStorage();
#ifdef _DEBUG
	size_t w=storage.GetLength();
#endif 
	if (SocketTCPReadExisting(storage)) {
#ifdef _DEBUG
		auto call=[this,& storage, w](CRJLogger& clLogger)->void {
			CRJStringByteDefault sInfo;
			SocketGetConnectionInfo(sInfo);
			clLogger.GetWriter().LogPublic(clLogger.Header(CRJDTimeTools::GetDTime()).Cat(sInfo).CatLabel(" Recv world ").CatSize(storage.GetLength()-w));
		};
		CRJLoggerFunctor<decltype(call)>cb(call);
		CRJLoggerThread::Check(cLoggerPartComm, cLoggerLevelTrace, cb);
#endif 
		RecvData();
	}
#ifdef _DEBUG
	else CRJDataTools::DebugBreak();
#endif 
}
void CRJNetSocketEventSessionPooled::OnNetEventSend() {
	m_clNetSend.SendReady();
}
bool CRJNetSocketEventSessionPooled::PoolStart() {
	m_clNetSend.Reset();
	return CRJSocketTCPPool::PoolStart();
}
bool CRJNetSocketEventSessionPooled::Transmit(CRJSocket& clSock,const CRJStringByte& s) {
	CRJNetSocketIPCTransmit clOut;
	if (clOut.Load(clSock, m_pid)) {
		m_lSeq++;
		CRJNetSocketEventSessionPooled::CWaitElem* pElem=CRJNetSocketEventSessionPooled::CWaitElem::Get(clSock, m_lSeq, s);
		if (pElem) {
			if (clOut.Send(SocketGetMe(), m_lSeq, s)) {
				m_clWaitList.Bot(* pElem);
				return true;
			}
			else delete pElem;
		}
	}
	return false;
}
void CRJNetSocketEventSessionPooled::RecvData() {
	size_t lLen;
	size_t l=m_clStorage.GetNotProcessedSize();
	const uint8_t* bHead=m_clStorage.GetNotProcessedHead();
	while (l>2) {
		lLen=CRJByteConvertor::BigEndianSet16(*(uint16_t*) bHead);
		if (lLen>cUNetSocketInPoolSessionsMaxCmdSize||
			lLen<sizeof(SDataApiHeaderNetSessions)) {
			m_clStorage.ResetCollector();
			return ;
		}
		else if (lLen+2>l)break;
		OnRecv(bHead+2, lLen);
		l=m_clStorage.Processed(lLen+2).GetNotProcessedSize();
		bHead=m_clStorage.GetNotProcessedHead();
	}
	m_clStorage.Normalize();
}
void CRJNetSocketEventSessionPooled::OnRecv(const uint8_t* pData, size_t lLen) {
	SDataApiHeaderNetSessions stHeader;
	CRJByteReaderSessions clReader((uint8_t*) pData, lLen);
	if (clReader.ReadHeader(stHeader)) {
		switch (stHeader.lModule) {
		case SNetClientDataApiPlugins::cSession:
			switch (stHeader.lRegion) {
			case SDataApiRegionSessionsRegions::cInfo:
				switch (stHeader.lCmd) {
				case SDataApiRegionSessionsRegionInfoCmds::cPid:clReader.UInt64(m_pid);
					break;
				case SDataApiRegionSessionsRegionInfoCmds::cConfirm:OnConfirm(clReader);
					break;
				case SDataApiRegionSessionsRegionInfoCmds::cDone:OnDone(clReader);
					break;
				case SDataApiRegionSessionsRegionInfoCmds::cStatistic:OnStatistic(clReader);
					break;
				};
			}
		}
	}
}
void CRJNetSocketEventSessionPooled::OnDone(CRJByteReader& clReader) {
	uint64_t t;
	if (clReader.UInt64(t)) {
		CRJNetSocketEventSessionPooled::CWaitElem* q,* p=m_clWaitList.GetTop();
		while (p) {
			q=p->GetNext();
			if (p->m_lId==t) {
				m_clWaitList.Remove(* p);
				return ;
			}
			p=q;
		}
	}
}
void CRJNetSocketEventSessionPooled::OnConfirm(CRJByteReader&) {}
void CRJNetSocketEventSessionPooled::OnStatistic(CRJByteReader& clReader) {
	uint64_t t1, t2;
	if (clReader.UInt64(t1).UInt64(t2)) {
		m_stMetric.lVolume=t1;
		m_stMetric.lInUse=t2;
	}
}

