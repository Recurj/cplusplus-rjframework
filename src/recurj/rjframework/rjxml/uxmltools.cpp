#include "stdafx.h"
#include <recurj/rjframework/libs/xml/xmltools.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#include <recurj/rjframework/libs/xml/xmltag.h>
#include <recurj/rjframework/libs/xml/xmltagattribute.h>
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/base/os/hlibrary.h>
#include <recurj/rjframework/libs/base/application/application.h>
#include <recurj/rjframework/libs/base/logger/loggerapp.h>
using namespace rjXml;
const CRJStringByte CRJXMLTools::s_Header("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
const CRJStringByte CRJXMLTools::s_DocType("<!DOCTYPE ");
const CRJStringByte CRJXMLTools::s_CData("<![CDATA[");
const CRJStringByte CRJXMLTools::s_XMLConvertor("\"'><&");
const CRJStringByte CRJXMLTools::s_LoggerTagLog("Log");
const CRJStringByte CRJXMLTools::s_LoggerTagTask("Sections");
const CRJStringByte CRJXMLTools::s_LoggerAttrLevel("level");
const CRJStringByte CRJXMLTools::s_LoggerAttrPath("path");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrComm("comm");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrDB("db");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrSSL("ssl");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrTask("task");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrMsg("msg");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrLua("lua");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrConfig("config");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrInfo("info");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrMemory("memory");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrFile("file");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrPlugin("plugin");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrOS("os");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrDaemon("daemon");
const CRJStringByte CRJXMLTools::s_LoggerSectionAttrOthers("others");
const CRJStringByte CRJXMLTools::s_tagDB("DB");
const CRJStringByte CRJXMLTools::s_tagNet("Net");
const CRJStringByte CRJXMLTools::s_tagNetSend("Send");
const CRJStringByte CRJXMLTools::s_tagWebsocket("Websocket");
const CRJStringByte CRJXMLTools::s_attrName("name");
const CRJStringByte CRJXMLTools::s_attrDBDsn("dsn");
const CRJStringByte CRJXMLTools::s_attrDBSecure("secure");
const CRJStringByte CRJXMLTools::s_attrConfig("config");
const CRJStringByte CRJXMLTools::s_attrPause("pause");
const CRJStringByte CRJXMLTools::s_attrRetry("retry");
const CRJStringByte CRJXMLTools::s_attrTimeout("timeout");
const CRJStringByte CRJXMLTools::s_attrDBPacket("packet");
const CRJStringByte CRJXMLTools::s_attrDBTrxnIsolation("isolation");
const CRJStringByte CRJXMLTools::s_attrDB64Bits("64Bits");
const CRJStringByte CRJXMLTools::s_attrDBAutoCommit("autocommit");
const CRJStringByte CRJXMLTools::s_attrDBReadOnly("readonly");
const CRJStringByte CRJXMLTools::s_cmdIgnore("//");
const CRJStringByte CRJXMLTools::s_cmdInstruction("$!");
const CRJStringByte CRJXMLTools::s_cmdDoctype("$#");
const CRJStringByte CRJXMLTools::s_cmdCData("$%");
const CRJStringByte CRJXMLTools::s_cmdComment("$/");
const CRJStringByte CRJXMLTools::s_cmdEntry("Entry");
const CRJStringByte CRJXMLTools::s_cmdLeave("Leave");
const CRJStringByte CRJXMLTools::s_cmdAttribute("$:");
const CRJStringByte CRJXMLTools::s_cmdContent("$=");
#ifdef _rjOS_LINUX
 const CRJString CRJXMLTools::s_fileExt(".xml");
#endif 
#ifdef _rjOS_WIN
 const CRJString CRJXMLTools::s_fileExt(L".xml");
#endif 
class CRJXmlStringParser {
public:
	CRJXmlStringParser(CRJStringByteVar& s):
		str(s),
	sLen(s.GetLength()),
	h(s.GetHeadForUpdate()) {}
	CRJStringByteVar& Parse() {
		while (sLen>0) {
			if (* h=='&') {
				if (sLen>3&&
					h[3]==';') {
					if (CRJDataTools::AreSame(h+1,(uint8_t*)"lt", 2)) {
						replace('<', 3);
						continue;
					}
					if (CRJDataTools::AreSame(h+1,(uint8_t*)"gt", 2)) {
						replace('>', 3);
						continue;
					}
				}
				if (sLen>4&&
					h[4]==';') {
					if (CRJDataTools::AreSame(h+1,(uint8_t*)"amp", 3)) {
						replace('&', 4);
						continue;
					}
				}
				if (sLen>5&&
					h[5]==';') {
					if (CRJDataTools::AreSame(h+1,(uint8_t*)"apos", 4)) {
						replace('\'', 5);
						continue;
					}
					else if (CRJDataTools::AreSame(h+1,(uint8_t*)"quot", 4)) {
						replace('"', 5);
						continue;
					}
				}
			}
			h++;
			sLen--;
		}
		return str;
	}
protected:
	void replace(char ch, size_t c) {
		size_t r=c+1;
		* h=(uint8_t) ch;
		h++;
		if (sLen>r) {
			sLen-=r;
			CRJDataTools::MoveBytes(h, h+c, sLen);
		}
		else sLen=0;
		str.SetLengthSafed(str.GetLength()-c);
	}
	CRJStringByteVar& str;
	size_t sLen;
	uint8_t* h;
};
bool CRJXMLTools::Text2Xml(CRJStringByteVar& dst,const CRJStringByte& src) {
	CRJChainFinderSet clFound;
	CRJStringByteFlex clWork(src);
	while (clWork.FindPosInSet(CRJXMLTools::s_XMLConvertor, clFound)) {
		dst.CatDataCheckLength(clWork.GetHeadForRead(), clFound.m_sFound);
		switch (clWork.GetHeadForRead()[clFound.m_sFound]) {
		case 39:dst.CatLabel("&apos;", 6);
			break;
		case '"':dst.CatLabel("&quot;", 6);
			break;
		case '&':dst.CatLabel("&amp;", 5);
			break;
		case '<':dst.CatLabel("&lt;", 4);
			break;
		case '>':dst.CatLabel("&gt;", 4);
			break;
		}
		clWork.TruncLeft(clFound.m_sFound+1);
	}
	return dst.OK();
}
bool CRJXMLTools::Text2Xml(CRJStringByteVar& s) {
	CRJStringByteExpanded dst;
	if (Text2Xml(dst, s)) return s.Set(dst).OK();
	s.SetVarError(CRJChain::eUChainErrors::Decode);
	return false;
}
CRJStringByteVar& CRJXMLTools::Xml2Text(CRJStringByteVar& s) {
	CRJXmlStringParser parser(s);
	return parser.Parse();
	;
}
bool CRJXMLTools::OnTagLog(CRJXMLTag& clTag) {
	{
		CRJFileName fn;
		CRJStringByteDefault name;
		SRJLoggerLevels levels(cLoggerLevelDBError);
		size_t d=cLoggerLevelDBError;
		auto& clApp=CRJApplication::Current();
		if (OnTagLogAttributes(d, fn, clTag)&&
			OnTagLogTags(name, levels, clTag)) {
			if (name.IsEmpty()) clApp.AppSetLogLevels(levels);
			CRJApplication::Current().AppSetLog(d, fn);
			return true;
		}
		return false;
	}
}
//	     bool CRJXMLTools::OnTagLog(CRJXMLTag&clTag, ITaskLogInfo&manager) {
//	      CRJFileName fn;
//	      CRJStringByteDefault name;
//	      SRJLoggerLevels levels(cLoggerLevelDBError);
//	      size_t d=cLoggerLevelDBError;
//	      auto&clApp=CRJApplication::Current();
//	      if (OnTagLogAttributes(d, fn, clTag) && OnTagLogTags(name, levels, clTag)) {
//	       if (name.IsEmpty()) clApp.AppSetLogLevels(levels);
//	       else manager.TaskLogAdd(name, levels);
//	       CRJApplication::Current().AppSetLog(d, fn);
//	       return true;
//	      }
//	      return false;
//	     }
 bool CRJXMLTools::OnTagNetSend(CRJXMLTag& clTag, rjNet::SRJSocketProcessMetrics& stSendMetrics) {
	auto call=[& stSendMetrics](CRJXMLTagAttribute& clAttr)->bool {
		if (clAttr.GetName()==s_attrPause) stSendMetrics.dwWait=(uint32_t) clAttr.GetReturnValidInt64(rjNet::SRJSocketProcessMetrics::s_stSendMetrics.dwWait);
		else if (clAttr.GetName()==s_attrRetry) stSendMetrics.lRetry=clAttr.GetReturnValidInt64(rjNet::SRJSocketProcessMetrics::s_stSendMetrics.lRetry);
		return true;
	};
	CXMLEnumerateAttributes<decltype(call)>cb(call);
	return clTag.ForEachAttribute(cb);
}
bool CRJXMLTools::OnTagLogAttributes(size_t& d, CRJFileName& fn, CRJXMLTag& clTag) {
	auto call=[& d,& fn](CRJXMLTagAttribute& clAttr)->bool {
		if (clAttr.GetName()==CRJXMLTools::s_LoggerAttrLevel) d=clAttr.GetReturnValidInt64(cLoggerLevelTraceAll);
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerAttrPath) fn.Set(clAttr.GetValue());
		return true;
	};
	CXMLEnumerateAttributes<decltype(call)>cb(call);
	return clTag.ForEachAttribute(cb);
}
bool CRJXMLTools::OnTagLogTags(CRJStringByteVar& name, SRJLoggerLevels& levels, CRJXMLTag& clMain) {
	auto callE=[& name,& levels](CRJXMLTagAttribute& clAttr)->bool {
		if (clAttr.GetName()==CRJXMLTools::s_attrName) name.Set(clAttr.GetValue());
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrComm) levels.SetLevel(cLoggerPartComm, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrDB) levels.SetLevel(cLoggerPartDB, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrSSL) levels.SetLevel(cLoggerPartSecurity, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrTask) levels.SetLevel(cLoggerPartTask, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrMsg) levels.SetLevel(cLoggerPartMessage, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrLua) levels.SetLevel(cLoggerPartLua, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrConfig) levels.SetLevel(cLoggerPartConfig, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrInfo) levels.SetLevel(cLoggerPartInfo, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrMemory) levels.SetLevel(cLoggerPartMemory, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrFile) levels.SetLevel(cLoggerPartFile, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrPlugin) levels.SetLevel(cLoggerPartPlugin, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrOS) levels.SetLevel(cLoggerPartOS, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrDaemon) levels.SetLevel(cLoggerPartDaemon, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		else if (clAttr.GetName()==CRJXMLTools::s_LoggerSectionAttrOthers) levels.SetLevel(cLoggerPartOthers, clAttr.GetReturnValidInt64(cLoggerLevelDBError));
		return true;
	};
	CXMLEnumerateAttributes<decltype(callE)>cbE(callE);
	auto call=[& cbE](CRJXMLTag& clTag)->bool {
		return clTag.IsThatName(CRJXMLTools::s_LoggerTagTask)?
			clTag.ForEachAttribute(cbE):
			true;
	};
	CXMLEnumerateTags<decltype(call)>cb(call);
	name.SetEmpty();
	return clMain.ForEachTag(cb);
}
bool CRJXMLTools::OnTagDB(rjDb::IRJDBConnectionInfo& app, CRJXMLTag& clTag) {
	CRJStringByteExpanded clName, clSetup, clDsn;
	rjDb::SDBConnectionOptions stOpts;
	bool bSecure=false;
	auto call=[& bSecure,& clName,& clDsn,& clSetup,& stOpts](CRJXMLTagAttribute& clAttr)->bool {
		if (clAttr.GetName()==CRJXMLTools::s_attrName) {
			return clName.Set(clAttr.GetValue()).OK();
		}
		else if (clAttr.GetName()==CRJXMLTools::s_attrDBDsn) {
			return clDsn.Set(clAttr.GetValue()).OK();
		}
		else if (clAttr.GetName()==CRJXMLTools::s_attrConfig) {
			return clSetup.Set(clAttr.GetValue()).OK();
		}
		else if (clAttr.GetName()==CRJXMLTools::s_attrDBSecure) {
			bSecure=clAttr.GetReturnValidBool(true);
			return true;
		}
		else if (clAttr.GetName()==CRJXMLTools::s_attrPause) {
			stOpts.sDBConnectPause=clAttr.GetReturnValidSize(stOpts.sDBConnectPause);
			return true;
		}
		else if (clAttr.GetName()==CRJXMLTools::s_attrRetry) {
			stOpts.sDBConnectRetry=clAttr.GetReturnValidSize(stOpts.sDBConnectRetry);
			return true;
		}
		else if (clAttr.GetName()==CRJXMLTools::s_attrTimeout) {
			stOpts.sDBTimeout=clAttr.GetReturnValidSize(stOpts.sDBTimeout);
			return true;
		}
		else if (clAttr.GetName()==CRJXMLTools::s_attrDBPacket) {
			stOpts.sDBPacketSize=clAttr.GetReturnValidSize(stOpts.sDBPacketSize);
			return true;
		}
		else if (clAttr.GetName()==CRJXMLTools::s_attrDBTrxnIsolation) {
			stOpts.sDBTrxnIsolation=clAttr.GetReturnValidSize(stOpts.sDBTrxnIsolation);
			return true;
		}
		else if (clAttr.GetName()==CRJXMLTools::s_attrDB64Bits) {
			stOpts.bSupport64BitsFields=clAttr.GetReturnValidBool(true);
			return true;
		}
		else if (clAttr.GetName()==CRJXMLTools::s_attrDBAutoCommit) {
			stOpts.bAutoCommit=clAttr.GetReturnValidBool(stOpts.bAutoCommit);
			return true;
		}
		else if (clAttr.GetName()==CRJXMLTools::s_attrDBReadOnly) {
			stOpts.bReadOnly=clAttr.GetReturnValidBool(stOpts.bReadOnly);
			return true;
		}
		return true;
	};
	CXMLEnumerateAttributes<decltype(call)>cb(call);
	if (clTag.ForEachAttribute(cb)) {
		if (clDsn.GetLength()>0) {
			if (bSecure&&
				!RJLibrary::ReFormat(clDsn)) CRJLoggerApp::ErrorText("Could not decrypt DB DSN", clName, cLoggerPartConfig);
			else if (app.DBConnectionInfoAdd(clName, clDsn, clSetup, stOpts)) return true;
		}
	}
	CRJLoggerApp::ErrorMessage("Could not process tag \"DB\"", cLoggerPartConfig, cLoggerLevelImportant);
	return false;
}
bool CRJXMLTools::ErrorTagUnknown(const CRJXMLTag& tag) {
	CRJLoggerThread::ErrorText("Unknown tag", tag.GetName(), cLoggerPartConfig);
	return false;
}
bool CRJXMLTools::ErrorTagContent(const CRJXMLTag& tag) {
	CRJLoggerThread::ErrorText("Invalid tag content", tag.GetName(), cLoggerPartConfig);
	return false;
}
bool CRJXMLTools::ErrorAttributeUnknown(const CRJXMLTag& tag,const CRJXMLTagAttribute& attr) {
	CRJStringByteExpanded s;
	CRJLoggerThread::ErrorText(tag.GetName(), s.SetLabel("Unknown attribute ").Cat(attr.GetName()), cLoggerPartConfig);
	return false;
}
bool CRJXMLTools::ErrorAttributeInvalid(const CRJXMLTag& tag,const CRJXMLTagAttribute& attr) {
	CRJStringByteExpanded s;
	CRJLoggerThread::ErrorText(tag.GetName(), s.SetLabel("Invalid attribute ").Cat(attr.GetName()).CatLabel(" Value=").Cat(attr.GetValue()), cLoggerPartConfig);
	return false;
}
bool CRJXMLTools::ErrorAttributeNotFound(const CRJXMLTag& tag,const CRJStringByte& name) {
	CRJStringByteExpanded s;
	CRJLoggerThread::ErrorText(tag.GetName(), s.SetLabel("missing attribute ").Cat(name), cLoggerPartConfig);
	return false;
}

