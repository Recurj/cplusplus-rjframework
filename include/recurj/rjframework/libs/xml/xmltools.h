#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
#include <recurj/rjframework/libs/base/logger/logger.h>
#include <recurj/rjframework/libs/db/dbconnectioninfo.h>
#include <recurj/rjframework/libs/net/netbase.h>
#include <recurj/rjframework/libs/xml/xmldefs.h>
using namespace rjBase;
namespace rjXml {
	class CRJXMLTag;
	class CRJXMLTagAttribute;
	class RJXML_API CRJXMLTools {
	public:
		static bool Text2Xml(CRJStringByteVar&);
		static bool Text2Xml(CRJStringByteVar&,const CRJStringByte&);
		static CRJStringByteVar& Xml2Text(CRJStringByteVar&);
		static bool OnTagLog(CRJXMLTag&);
		//	       static bool OnTagLog(CRJXMLTag&, ITaskLogInfo&);
		static bool OnTagLogAttributes(size_t&, CRJFileName&, CRJXMLTag&);
		static bool OnTagLogTags(CRJStringByteVar&, SRJLoggerLevels&, CRJXMLTag&);
		static bool OnTagDB(rjDb::IRJDBConnectionInfo&, CRJXMLTag&);
		static bool OnTagNetSend(CRJXMLTag& clTag, rjNet::SRJSocketProcessMetrics&);
		static bool ErrorTagUnknown(const CRJXMLTag&);
		static bool ErrorTagContent(const CRJXMLTag&);
		static bool ErrorAttributeUnknown(const CRJXMLTag&,const CRJXMLTagAttribute&);
		static bool ErrorAttributeInvalid(const CRJXMLTag&,const CRJXMLTagAttribute&);
		static bool ErrorAttributeNotFound(const CRJXMLTag&,const CRJStringByte&);
		static const CRJStringByte s_Header;
		static const CRJStringByte s_DocType;
		static const CRJStringByte s_CData;
		static const CRJStringByte s_XMLConvertor;
		static const CRJStringByte s_LoggerTagLog;
		static const CRJStringByte s_LoggerTagTask;
		static const CRJStringByte s_LoggerAttrLevel;
		static const CRJStringByte s_LoggerAttrPath;
		static const CRJStringByte s_LoggerSectionAttrComm;
		static const CRJStringByte s_LoggerSectionAttrDB;
		static const CRJStringByte s_LoggerSectionAttrSSL;
		static const CRJStringByte s_LoggerSectionAttrTask;
		static const CRJStringByte s_LoggerSectionAttrMsg;
		static const CRJStringByte s_LoggerSectionAttrLua;
		static const CRJStringByte s_LoggerSectionAttrConfig;
		static const CRJStringByte s_LoggerSectionAttrInfo;
		static const CRJStringByte s_LoggerSectionAttrMemory;
		static const CRJStringByte s_LoggerSectionAttrFile;
		static const CRJStringByte s_LoggerSectionAttrPlugin;
		static const CRJStringByte s_LoggerSectionAttrOS;
		static const CRJStringByte s_LoggerSectionAttrDaemon;
		static const CRJStringByte s_LoggerSectionAttrOthers;
		static const CRJStringByte s_tagDB;
		static const CRJStringByte s_tagNet;
		static const CRJStringByte s_tagNetSend;
		static const CRJStringByte s_tagWebsocket;
		static const CRJStringByte s_attrName;
		static const CRJStringByte s_attrDBDsn;
		static const CRJStringByte s_attrDBSecure;
		static const CRJStringByte s_attrConfig;
		static const CRJStringByte s_attrPause;
		static const CRJStringByte s_attrRetry;
		static const CRJStringByte s_attrTimeout;
		static const CRJStringByte s_attrDBPacket;
		static const CRJStringByte s_attrDBTrxnIsolation;
		static const CRJStringByte s_attrDB64Bits;
		static const CRJStringByte s_attrDBAutoCommit;
		static const CRJStringByte s_attrDBReadOnly;
		static const CRJString s_fileExt;
		static const CRJStringByte s_cmdIgnore;
		static const CRJStringByte s_cmdInstruction;
		static const CRJStringByte s_cmdDoctype;
		static const CRJStringByte s_cmdCData;
		static const CRJStringByte s_cmdComment;
		static const CRJStringByte s_cmdEntry;
		static const CRJStringByte s_cmdLeave;
		static const CRJStringByte s_cmdAttribute;
		static const CRJStringByte s_cmdContent;
	};
}

