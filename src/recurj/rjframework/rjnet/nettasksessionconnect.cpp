#include "stdafx.h"
#include <recurj/rjframework/libs/net/tasks/nettasksessionconnector.h>
#include <recurj/rjframework/libs/net/ipc/netsocketipcaccept.h>
#include <recurj/rjframework/libs/base/os/hprocess.h>
#include <recurj/rjframework/libs/base/datatools/bytewriter.h>
#include <recurj/rjframework/libs/net/messages/netmessagesessiondone.h>
#include <recurj/rjframework/libs/net/dataapi/netsessions.h>
using namespace rjNet;
CRJNetTaskSessionConnector::CRJNetTaskSessionConnector() {
	m_sVolume=m_sInUse=0;
}
CRJMessage* CRJNetTaskSessionConnector::OnTaskQueueMsg(CRJMessage& clMsg) {
	switch (clMsg.GetRegion()) {
	case eRJMessageRegions::Net: return onMsgRegionNet(clMsg);
	}
	return CRJNetTaskConnector::OnTaskQueueMsg(clMsg);
}
CRJMessage* CRJNetTaskSessionConnector::onMsgRegionNet(CRJMessage& clMsg) {
	switch (clMsg.GetCommand()) {
	case eRJMessagesRegionNet::SessionDone:OnSessionDone(((CRJMessageNetSessionDone&) clMsg).m_lId);
		return & clMsg;
		break;
	}
	return CRJNetTaskConnector::OnTaskQueueMsg(clMsg);
}
void CRJNetTaskSessionConnector::OnConnect() {
	CRJNetTaskConnector::OnConnect();
	SendPID(HUWLProcess::GetIdCurrentProcess());
	SendStatistic();
}
void CRJNetTaskSessionConnector::OnReceive() {
	CRJNetSocketIPCAccept a;
	if (a.Read(m_clSock)) {
		CRJSocket clSock(*this);
		if (a.Load(clSock)) {
			SendConfirm(a.GetId());
			if (StartSession(clSock, a.GetId(), a.GetParameters())) m_sInUse++;
			SendStatistic();
		}
	}
}
void CRJNetTaskSessionConnector::Ping() {
	SendPing();
}
void CRJNetTaskSessionConnector::OnSessionDone(size_t id) {
	SendDone(id);
	if (m_sInUse>0) {
		m_sInUse--;
		SendStatistic();
	}
}
void CRJNetTaskSessionConnector::SendConfirm(uint64_t id) {
	CRJStringByteDefault s;
	CRJByteWriterSessions clWriter(s);
	m_clSock.GetNetSend().SendData(clWriter.WriteHeader(SNetClientDataApiPlugins::cSession, SDataApiRegionSessionsRegions::cInfo, SDataApiRegionSessionsRegionInfoCmds::cConfirm).UInt64(id).SetLengthBaseFormat());
}
void CRJNetTaskSessionConnector::SendDone(uint64_t id) {
	CRJStringByteDefault s;
	CRJByteWriterSessions clWriter(s);
	m_clSock.GetNetSend().SendData(clWriter.WriteHeader(SNetClientDataApiPlugins::cSession, SDataApiRegionSessionsRegions::cInfo, SDataApiRegionSessionsRegionInfoCmds::cDone).UInt64(id).SetLengthBaseFormat());
}
void CRJNetTaskSessionConnector::SendPID(uint64_t pid) {
	CRJStringByteDefault s;
	CRJByteWriterSessions clWriter(s);
	m_clSock.GetNetSend().SendData(clWriter.WriteHeader(SNetClientDataApiPlugins::cSession, SDataApiRegionSessionsRegions::cInfo, SDataApiRegionSessionsRegionInfoCmds::cPid).UInt64(pid).SetLengthBaseFormat());
}
void CRJNetTaskSessionConnector::SendStatistic() {
	CRJStringByteDefault s;
	CRJByteWriterSessions clWriter(s);
	m_clSock.GetNetSend().SendData(clWriter.WriteHeader(SNetClientDataApiPlugins::cSession, SDataApiRegionSessionsRegions::cInfo, SDataApiRegionSessionsRegionInfoCmds::cStatistic).UInt64(m_sVolume).UInt64(m_sInUse).SetLengthBaseFormat());
}
void CRJNetTaskSessionConnector::SendPing() {
	CRJStringByteDefault s;
	CRJByteWriterSessions clWriter(s);
	m_clSock.GetNetSend().SendData(clWriter.WriteHeader(SNetClientDataApiPlugins::cSession, SDataApiRegionSessionsRegions::cInfo, SDataApiRegionSessionsRegionInfoCmds::cPing).SetLengthBaseFormat());
}

