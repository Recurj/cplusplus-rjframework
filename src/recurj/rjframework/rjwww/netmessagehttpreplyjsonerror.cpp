#include "stdafx.h"
#include <recurj/rjframework/libs/www/messages/replyjsonerror.h>
#include <recurj/rjframework/libs/base/tasks/task.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionmessage.h>
using namespace rjWWW;
RJ_ALLOCATOR(CMessageHTTPReplyJSONError, Allocator::Region::Message, AllocatorMessage::HttpReplyJSON) CMessageHTTPReplyJSONError* CMessageHTTPReplyJSONError::Get(int64_t lErr,const SRJTaskTicket& stPost,const CRJStringByte& clLang) {
	return new CMessageHTTPReplyJSONError(lErr, stPost, clLang);
}
CMessageHTTPReplyJSONError::CMessageHTTPReplyJSONError(int64_t lErr,const SRJTaskTicket& stPost,const CRJStringByte& clLang):
	CMessageHTTPReply(eRJMessagesRegionWWW::Error, stPost),
m_clLang(clLang),
m_clReply(lErr) {}
CMessageHTTPReplyJSONError::~CMessageHTTPReplyJSONError() {}
void CMessageHTTPReplyJSONError::JSONError(int64_t lErr,const SRJTaskTicket& stNetPost,const CRJStringByte& clLang) {
	if (!stNetPost.m_pTaskSource) return ;
	TaskMsgGetAndSend<CMessageHTTPReplyJSONError, int64_t,const SRJTaskTicket&,const CRJStringByte&>(* stNetPost.m_pTaskSource, lErr, stNetPost, clLang);
}

