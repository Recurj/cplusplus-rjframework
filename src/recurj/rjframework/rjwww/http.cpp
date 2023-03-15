#include "stdafx.h"
#include <recurj/rjframework/libs/www/http/defs.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
using namespace rjWWW;
CRJStringByte CHTTP::s_clEmpty("");
CRJStringByte CHTTP::s_clMyProtocol("HTTP");
CRJStringByte CHTTP::s_clHeaderContentLength("content-length");
CRJStringByte CHTTP::s_clHeaderAcceptEncoding("accept-encoding");
CRJStringByte CHTTP::s_clHeaderAcceptLanguage("accept-language");
CRJStringByte CHTTP::s_clHeaderAcceptCharset("accept-charset");
CRJStringByte CHTTP::s_clHeaderConnection("connection");
CRJStringByte CHTTP::s_clHeaderLastModified("Last-Modified:");
CRJStringByte CHTTP::s_clHeaderUpgrade("upgrade");
CRJStringByte CHTTP::s_clHeaderWebsocketKey("sec-websocket-key");
CRJStringByte CHTTP::s_clHeaderWebsocketAccept("Sec-WebSocket-Accept: ");
CRJStringByte CHTTP::s_clHeaderCookie("cookie");
CRJStringByte CHTTP::s_clHeaderSetCookie("Set-Cookie: ");
CRJStringByte CHTTP::s_clHeaderSetCookieExpires(" expires = ");
CRJStringByte CHTTP::s_clHeaderContentEncodingGzip("gzip");
CRJStringByte CHTTP::s_clContentLanguage("Content-Language: ");
CRJStringByte CHTTP::s_clContentType("Content-Type: ");
CRJStringByte CHTTP::s_clConnectionClose("close");
CRJStringByte CHTTP::s_clConnectionKeepAlive("keep-alive");
CRJStringByte CHTTP::s_clConnectionUpgrade("upgrade");
CRJStringByte CHTTP::s_clUpgradeWebsocket("websocket");
CRJStringByte CHTTP::s_clContentMimeJSON("application/json");
CRJStringByte CHTTP::s_clContentMimeHTML("text/html");
CRJStringByte CHTTP::s_clContentFileJS("js");
CRJStringByte CHTTP::s_clContentFileCSS("css");
CRJStringByte CHTTP::s_clContentFileJSON("json");
CHTTP::funcRead CHTTP::s_TableOfFuncRead[SIZE_HTTP_SERVER_INPUT_MODE]= {
	& CHTTP::OnHTTPRecvFirst,& CHTTP::OnHTTPRecvHeader,& CHTTP::OnHTTPRecvBinary,& CHTTP::OnHTTPRecvError
};
CHTTP::CHTTP() {
	OnHTTPReset();
	m_bHTTPStoreAttributes=true;
}
CHTTP::~CHTTP(void) {}
void CHTTP::OnHTTPReset() {
	m_lHTTPLength=0;
	m_lHTTPInputMode=InputMode::INPUT_FIRST;
	m_clHTTPMessage.Restart();
	m_clStorage.SetVarRegion(DataResutRegionHttp);
}
bool CHTTP::OnHTTPRecvFailure() {
	OnHTTPReset();
	return false;
}
bool CHTTP::OnHTTPRecvDone() {
	return true;
}
bool CHTTP::OnHTTPRecv() {
	return (this->* s_TableOfFuncRead[m_lHTTPInputMode])();
}
bool CHTTP::OnHTTPRecvFirst() {
	CRJStringByteFlex s;
	CRJByteStorageReader clReader(*this);
	if (clReader.LineCRLF(s)) {
		if (!OnHTTPRecvFirstLine(s)) return OnHTTPRecvFailure();
		m_lHTTPInputMode=InputMode::INPUT_HEADER;
		return (this->* s_TableOfFuncRead[m_lHTTPInputMode])();
	}
	return true;
}
bool CHTTP::OnHTTPRecvHeader() {
	CRJStringByteFlex s;
	CRJByteStorageReader clReader(*this);
	while (clReader.LineCRLF(s)) {
		if (!s.GetLength()) return OnHTTPRecvHeaderTotal();
		else if (!OnHTTPRecvHeaderString(s)) return OnHTTPRecvFailure();
	}
	return true;
}
bool CHTTP::OnHTTPRecvBinary() {
	size_t lHas=GetNotProcessedSize();
	if (lHas>m_lHTTPLength) {
#ifdef _DEBUG
		CRJDataTools::DebugBreak();
#endif 
		lHas=m_lHTTPLength;
	}
	if (!OnHTTPRecvBinaryData(lHas)) return OnHTTPRecvFailure();
	Processed(lHas);
	m_lHTTPLength-=lHas;
	if (!m_lHTTPLength) return OnHTTPRecvBinaryTotal();
	return true;
}
bool CHTTP::OnHTTPRecvBinaryData(size_t) {
	return true;
}
bool CHTTP::OnHTTPRecvHeaderTotal() {
	if (OnHTTPRecvHeaderStringLastAttribute()) {
		if (m_lHTTPLength>0) {
			m_lHTTPInputMode=InputMode::INPUT_DATA;
			return (this->* s_TableOfFuncRead[m_lHTTPInputMode])();
		}
		else return OnHTTPRecvBinaryTotal();
	}
	return OnHTTPError(StreamError::BAD_CONTENT_LENGTH, Status::CLIENT_BAD_REQUEST);
}
bool CHTTP::OnHTTPRecvError() {
	return OnHTTPError(StreamError::BAD_READ_MODE, Status::SERVER_UNAVAILABLE);
}
bool CHTTP::OnHTTPRecvBinaryTotal() {
	m_lHTTPInputMode=InputMode::INPUT_FIRST;
	if (OnHTTPRecvTotal()) return OnHTTPRecvDone();
	return OnHTTPRecvFailure();
}
bool CHTTP::OnHTTPRecvTotal() {
	return true;
}
bool CHTTP::OnHTTPRecvHeaderString(CRJStringByteFlex& clInput) {
	CRJStringByteFlex clName;
	clInput.Trim(' ').WasFoundTopAndMovedHead(':', clName);
	if (clName.GetLength()>0) {
		if (!OnHTTPRecvHeaderStringLastAttribute()) return false;
		m_clHTTPLastAttrName.Set(clName.TrimRight(' '));
		m_clHTTPLastAttrValue.Set(clInput.TrimLeft(' '));
		return true;
	}
	else if (m_clHTTPLastAttrName.GetLength()>0) {
		m_clHTTPLastAttrValue.CatSingle(' ').Cat(clInput);
		return true;
	}
	return false;
}
bool CHTTP::OnHTTPRecvHeaderStringLastAttribute() {
	if ((m_clHTTPLastAttrName.GetLength()>0)&&
		(m_clHTTPLastAttrValue.GetLength()>0)) {
		m_clHTTPLastAttrName.LowerCase();
		return OnHTTPHeaderAttribute(m_clHTTPLastAttrName, m_clHTTPLastAttrValue);
	}
	return true;
}
bool CHTTP::OnHTTPHeaderAttribute(const CRJStringByte& a,const CRJStringByte& v) {
	if (m_bHTTPStoreAttributes) m_clHTTPMessage.Cat(a).CatSingle(':').Cat(v).CatSingle(0x0A);
	if (a==CHTTP::s_clHeaderContentLength) return CHTTP::OnHTTPRecvContentLength(v);
	return true;
}
bool CHTTP::OnHTTPRecvContentLength(const CRJStringByte& s) {
	size_t v;
	if (s.IsValidSize(v)) m_lHTTPLength=v;
	else return OnHTTPError(StreamError::BAD_BINARY_LENGTH, Status::SERVER_INTERNAL);
	return true;
}
bool CHTTP::OnHTTPRecvContentDesired(const CRJStringByte& clTemp,const CRJStringByte& clWanted) {
	auto call=[& clWanted](CRJStringByteFind& s)->bool {
		return s.Trim(' ')!=clWanted;
	};
	CRJStringByteFunctorSplitBool<decltype(call)>cb(call);
	return !clTemp.SplitResult(',', cb);
}
bool CHTTP::OnHTTPAnalyzeVersion() {
	CRJStringByteFlex clProto, str;
	str.Set(m_clHTTPMessage.m_clVersionRecv).WasFoundTopAndMovedHead('/', clProto);
	if (clProto==CHTTP::s_clMyProtocol) {
		str.WasFoundTopAndMovedHead('.', clProto);
		int64_t l1, l2;
		if (str.IsValidInt64(l1)&&
			clProto.IsValidInt64(l2)) {
			if ((l1>1)||
				((l1==1)&&
				(l2>=1))) {
				m_clHTTPMessage.m_lVersionMajor=l1;
				m_clHTTPMessage.m_lVersionMinor=l2;
				return true;
			}
		}
	}
	return OnHTTPError(StreamError::BAD_HEADER, Status::SERVER_NOT_IMPLEMENTED);
}
CHTTP::CDataTableWeighted::CDataTableWeighted(CHTTP::CDataWeighted* p, size_t l):
	m_pTable(p),
m_lFound(0),
m_lVolume(l) {}
size_t CHTTP::CDataTableWeighted::Parse(const CRJStringByte& src) {
	double d;
	CRJStringByteFlex p, clTemp(src);
	m_lFound=0;
	while (clTemp.WasFoundTopAndMovedHead(',', p)) {
		d=Part(p);
		Push(p.GetHeadForRead(), p.GetLength(), d);
	}
	d=Part(clTemp);
	Push(clTemp.GetHeadForRead(), clTemp.GetLength(), d);
	return m_lFound;
}
double CHTTP::CDataTableWeighted::Part(CRJStringByteFlex& part) {
	CRJStringByteFlex q, w;
	if (part.TrimRight(' ').WasFoundTopAndMovedTail(';', q)&&
		q.TrimLeft(' ').WasFoundTopAndMovedTail('=', w)) {
		double v;
		w.Trim(' ');
		q.TrimRight(' ');
		if ((q.GetLength()==1)&&
			(q[0]=='q')&&
			w.IsValidDouble(v)) return v;
	}
	return 1;
}
void CHTTP::CDataTableWeighted::Push(const uint8_t* pData, size_t lLen,double w) {
	for (size_t i=0; i<m_lFound; i++) {
		if (w>m_pTable[i].weight) {
			Shift(i);
			m_pTable[i].Set(pData, lLen, w);
			return ;
		}
	}
	if (m_lFound<m_lVolume) {
		m_pTable[m_lFound].Set(pData, lLen, w);
		m_lFound++;
	}
}
void CHTTP::CDataTableWeighted::Shift(size_t ind) {
	size_t p, c=m_lFound+1;
	if (c>=m_lVolume) c=m_lVolume-1;
	else m_lFound=c;
	while (c>ind) {
		p=c-1;
		m_pTable[c]=m_pTable[p];
		c=p;
	}
}

