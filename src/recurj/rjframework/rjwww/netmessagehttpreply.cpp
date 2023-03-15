#include "stdafx.h"
#include <recurj/rjframework/libs/www/messages/reply.h>
using namespace rjWWW;
CMessageHTTPReply::CMessageHTTPReply(size_t lCmd,const SRJTaskTicket& stNetPost):
	CRJMessage(eRJMessageRegions::WWW, lCmd),
m_stNetPost(stNetPost) {}
CMessageHTTPReply::~CMessageHTTPReply() {}

