#include "stdafx.h"
#include <recurj/rjframework/libs/www/messages/replyfile.h>
#include <recurj/rjframework/libs/base/tasks/task.h>
#include <recurj/rjframework/libs/base/memory/regions/allocatorregionmessage.h>
using namespace rjWWW;
RJ_ALLOCATOR(CMessageHTTPReplyFile, Allocator::Region::Message, AllocatorMessage::HttpReplyFile) CMessageHTTPReplyFile::CMessageHTTPReplyFile(size_t lCmd,const SRJTaskTicket& stNetPost,const CRJStringByte& clFileName):
	CMessageHTTPReply(lCmd, stNetPost) {
	m_clFileName.Set(clFileName).LowerCase();
}
CMessageHTTPReplyFile::~CMessageHTTPReplyFile() {}
CMessageHTTPReplyFile* CMessageHTTPReplyFile::Get(size_t lCmd,const SRJTaskTicket& stNetPost,const CRJStringByte& clFile) {
	if (!stNetPost.m_pTaskSource) return nullptr;
	return new CMessageHTTPReplyFile(lCmd, stNetPost, clFile);
}

