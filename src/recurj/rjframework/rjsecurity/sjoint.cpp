#include "stdafx.h"
#include <recurj/rjframework/libs/security/sjoint.h>
#include <recurj/rjframework/libs/base/datetime/dtimebinary.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/security/destools.h>
#include <recurj/rjframework/libs/security/opensslpublickey.h>
using namespace rjSecurity;
CSJointEndPointCipherEmpty CSJoint::CSJointEndPoint::s_CipherEmpty;
CSJointEndPointCipherDes* CSJointEndPointCipherDes::Create(const CRJStringByteFlex& st) {
	return new CSJointEndPointCipherDes(st);
}
void CSJointEndPointCipherDes::Free(CSJointEndPointCipherDes* p) {
	if (!p) return ;
	try {
		delete p;
	}
	catch(...) {}
}
CSJointEndPointCipherDes::CSJointEndPointCipherDes(const CRJStringByteFlex& st) {
	m_clKey.SetKey(st.GetHeadForRead(), st.GetLength());
}
CSJointEndPointCipherDes::~CSJointEndPointCipherDes() {}
bool CSJointEndPointCipherDes::Encrypt(CSJointConverter& stConverter) {
	stConverter.m_stDst.SetStorage((uint8_t *)stConverter.m_stSrc.GetHeadForRead(), stConverter.m_stSrc.GetLength());
	CRJSecurityDesTools::DoEncryptTripleDesBlockSimple(stConverter.m_stDst.GetHeadForUpdate(), stConverter.m_stSrc.GetLength(), m_clKey.GetKeyData());
	return true;
}
bool CSJointEndPointCipherDes::Decrypt(CSJointConverter& stConverter) {
	stConverter.m_stDst.SetStorage((uint8_t*)stConverter.m_stSrc.GetHeadForRead(), stConverter.m_stSrc.GetLength());
	CRJSecurityDesTools::DoDecryptTripleDesBlockSimple(stConverter.m_stDst.GetHeadForUpdate(), stConverter.m_stSrc.GetLength(), m_clKey.GetKeyData());
	return true;
}
CSJointEndPointCipherAes* CSJointEndPointCipherAes::Create(const CRJStringByteFlex& st) {
	return new CSJointEndPointCipherAes(st);
}
void CSJointEndPointCipherAes::Free(CSJointEndPointCipherAes* p) {
	if (!p) return ;
	try {
		delete p;
	}
	catch(...) {}
}
CSJointEndPointCipherAes::CSJointEndPointCipherAes(const CRJStringByteFlex& st) {
	auto p = st.GetHeadForRead();
	m_clKey.Load(p, p+32);
}
CSJointEndPointCipherAes::~CSJointEndPointCipherAes() {}
bool CSJointEndPointCipherAes::Encrypt(CSJointConverter& stConverter) {
	return m_clKey.EncryptInit()&&
		m_clKey.EncryptStep(stConverter.m_stSrc, stConverter.m_stDst);
}
bool CSJointEndPointCipherAes::Decrypt(CSJointConverter& stConverter) {
	return m_clKey.DecryptInit()&&
		m_clKey.DecryptStep(stConverter.m_stSrc, stConverter.m_stDst);
}
CSJoint::CSJointEndPoint::CSJointEndPoint() {
	m_pCipher=& CSJointEndPoint::s_CipherEmpty;
	Reset();
}
CSJoint::CSJointEndPoint::~CSJointEndPoint() {}
void CSJoint::CSJointEndPoint::Reset() {
	m_lSeq=0;
}
void CSJoint::CSJointEndPoint::SetCipher(CSJointManager& clManager, CSJointEndPointCipher* p) {
	FreeCipher(clManager);
	if (!p) m_pCipher=& CSJointEndPoint::s_CipherEmpty;
	else m_pCipher=p;
}
void CSJoint::CSJointEndPoint::FreeCipher(CSJointManager& clManager) {
	if (m_pCipher==& CSJointEndPoint::s_CipherEmpty) return ;
	try {
		clManager.FreeCipher(* m_pCipher);
	}
	catch(...) {}
	m_pCipher=& CSJointEndPoint::s_CipherEmpty;
}
CSJoint::CSJoint() {}
CSJoint::~CSJoint() {
	m_clMe.FreeCipher(*this);
	m_clRemote.FreeCipher(*this);
}
CSJointValidate::CSJointValidate() {
	Reset();
}
CSJointValidate::~CSJointValidate() {}
void CSJointValidate::Reset() {
	m_lValidateIndex=0;
	m_sValidateSize=0;
}
CSJointContext::CSJointContext(size_t h):
	m_lHeaderSize(h) {}
CSJointContext::~CSJointContext() {}
int64_t CSJointContext::GetKeyChangeDuration() {
	return 3600;
}
const CRJStringByteVarAssigned& CSJointContext::GetTempBuffer(CRJStringByteVarAssigned& st) const {
	st.SetStorage((uint8_t *)m_bTempBuffer,sizeof(m_bTempBuffer));
	return st;
}
CRJStringByteFlex& CSJointContext::GetCommandBuffer(CRJStringByteFlex& st) {
	st.SetFromData(m_bCommandBuffer, m_lCommandSize);
	return st;
}
SJointMsgHeader& CSJointContext::OnRecvCommHeader(CRJByteReader& reader) {
	return *((SJointMsgHeader*) reader.Skip(m_lHeaderSize).Head());
}
SJointMsgHeader& CSJointContext::GetHeader(const uint8_t* p) const {
	return *((SJointMsgHeader*)& p[m_lHeaderSize]);
}
CSJointContext& CSJointContext::PushCmd(time_t t, int16_t lCmd) {
	SJointMsgHeader& stHeader=OnSendCommHeader(t);
	stHeader.lCmd=lCmd;
	stHeader.lSeq=0;
	stHeader.lVersion=1;
	stHeader.lSubVersion=1;
	stHeader.lArgs=0;
	m_lCommandSize=m_lHeaderSize+SJointCommandHeaderLen;
	return *this;
}
CSJointContext& CSJointContext::PushArgInt64(int16_t lTag, int64_t lValue) {
	*((int16_t*)& m_bCommandBuffer[m_lCommandSize])=lTag;
	m_lCommandSize+=SJointArgFieldLen;
	*((int16_t*)& m_bCommandBuffer[m_lCommandSize])=SJointArgInt64Len;
	m_lCommandSize+=SJointArgFieldLen;
	*((int64_t*)& m_bCommandBuffer[m_lCommandSize])=lValue;
	m_lCommandSize+=SJointArgInt64Len;
	return WasAddedArg();
}
CSJointContext& CSJointContext::PushArgBytes(int16_t lTag,const CRJStringByteFlex& st) {
	size_t s = st.GetLength();
	*((int16_t*)& m_bCommandBuffer[m_lCommandSize])=lTag;
	m_lCommandSize+=SJointArgFieldLen;
	*((int16_t*)& m_bCommandBuffer[m_lCommandSize])=(int16_t) s;
	m_lCommandSize+=SJointArgFieldLen;
	if (s>0) __movsb(& m_bCommandBuffer[m_lCommandSize], st.GetHeadForRead(), (SIZE_T)s);
	m_lCommandSize+=s;
	return WasAddedArg();
}
CSJointContext& CSJointContext::WasAddedArg() {
	GetHeader(m_bCommandBuffer).lArgs++;
	return *this;
}
CSJoint& CSJoint::OpenSession() {
	m_clMe.Reset();
	m_clRemote.Reset();
	m_clValidate.Reset();
	return *this;
}
bool CSJoint::SendPrepare(CSJointContext& clContext, CSJointConverter& stConverter) {
	if (!m_clMe.GetCipher().Encrypt(stConverter)) return OnError(clContext, enumCSJointRecvResults::NOT_ENCRYPTED);
	return true;
}
bool CSJoint::SendData(CSJointContext& clContext,const CRJStringByteFlex& stData, size_t sMax, time_t t,bool bSync) {
	CRJStringByteFlex stSend, stWork;
	const uint8_t* pData = stData.GetHeadForRead();
	size_t w,s=stData.GetLength();
	while (s>0) {
		w=(s>sMax)?sMax:s;
		clContext.PushCmd(t, enumCSJointCommands::DATA).PushArgBytes(enumCSJointTags::DATA_ENCRYPTED, stWork.SetFromData(pData, w));
		if (!OnSendData(clContext, clContext.GetCommandBuffer(stSend))) return OnError(clContext, enumCSJointRecvResults::NOT_SENT);
		pData += w;
		s-=w;
	}
	return (bSync)?CheckKeyUpdate(clContext, t):true;
}
void CSJoint::SendMarker(CSJointContext& clContext,const CRJStringByteFlex& st) {
	clContext.GetHeader(st.GetHeadForRead()).lSeq=m_clMe.m_lSeq++;
}
bool CSJoint::SendEnv(CSJointContext& clContext,const CRJStringByte& clEnv) {
	CRJStringByteFlex stData, stSend;
	clContext.PushCmd(CRJDTimeTools::GetDTime(), enumCSJointCommands::SETENV).PushArgBytes(enumCSJointTags::ENV_NUMBER, stData.Set(clEnv));
	if (OnSendCommand(clContext, clContext.GetCommandBuffer(stSend))) return true;
	return OnError(clContext, enumCSJointRecvResults::NOT_SENT);
}
bool CSJoint::GenMyKey(CSJointContext& clContext, int64_t lAlg) {
	CRJSecurityPublicKey clPub;
	if (m_clRemote.m_clKeyFile.GetLength()==0) return OnError(clContext, enumCSJointRecvResults::NO_FILENAME_REMOTE);
	if (clPub.LoadFile(m_clRemote.m_clKeyFile,true)==CRJSecurityPublicKey::eRJPKIResults::OK) {
		CSJointConverter stConverter;
		CRJStringByteFlex stSend;
		uint8_t buffer[2* cSeptadaAES256Key];
		CRJSecurityBuffer clSSLBuf(clPub);
		time_t t=CRJDTimeTools::GetDTime();
		stConverter.SetSrc(buffer,sizeof(buffer));
		clContext.GetTempBuffer(stConverter.m_stDst);
		m_clMe.SetCipher(*this, GetMyCipher(t, lAlg, stConverter.m_stDst));
		if (clPub.Encrypt(stConverter.m_stSrc, stConverter.m_stDst, clSSLBuf)==CRJSecurityPublicKey::eRJPKIResults::OK) {
			clContext.PushCmd(t, enumCSJointCommands::SETKEY).PushArgInt64(enumCSJointTags::ALG_CODE, m_clMe.GetCipher().GetAlg()).PushArgBytes(enumCSJointTags::ALG_KEY, stConverter.GetSrcAsDst());
			if (OnSendCommand(clContext, clContext.GetCommandBuffer(stSend))) {
				m_lKeyChanged=t;
				return true;
			}
			return OnError(clContext, enumCSJointRecvResults::NOT_SENT);
		}
		else return OnError(clContext, enumCSJointRecvResults::NOT_PKI_CRYPT);
	}
#ifdef _DEBUG
	CRJDataTools::DebugBreak();
#endif 
	return OnError(clContext, enumCSJointRecvResults::NOT_LOADED_REMOTE);
}
bool CSJoint::Validate(CSJointContext& clContext) {
	size_t l;
	CSJointConverter stConverter;
	CRJStringByteFlex stSend;
	time_t tSend=CRJDTimeTools::GetDTime();
	srand((unsigned int) tSend);
	l= (size_t)rand();
	l &= 0x3F;
	l += 41;
	stConverter.m_stSrc.SetFromData(m_clValidate.m_bValidateBuffer,l);
	for (size_t i=0; i<l; i++) m_clValidate.m_bValidateBuffer[i]=(uint8_t)(rand()& 0xFF);
	m_clValidate.m_sValidateSize=l;
	m_clValidate.m_lValidateIndex++;
	clContext.GetTempBuffer(stConverter.m_stDst);
	if (!m_clMe.GetCipher().Encrypt(stConverter)) return OnError(clContext, enumCSJointRecvResults::NOT_ENCRYPTED);
	clContext.PushCmd(tSend, enumCSJointCommands::VALIDATE).PushArgInt64(enumCSJointTags::VALIDATE_INDEX, m_clValidate.m_lValidateIndex).PushArgInt64(enumCSJointTags::VALIDATE_STEP, 0).PushArgBytes(enumCSJointTags::DATA_ENCRYPTED, stConverter.GetSrcAsDst());
	if (OnSendCommand(clContext, clContext.GetCommandBuffer(stSend))) return true;
	return OnError(clContext, enumCSJointRecvResults::NOT_SENT);
}
bool CSJoint::Receive(CSJointContext& clContext, CSJointConverter& stConverter, time_t tRecv) {
	CRJByteReader reader(stConverter.m_stSrc);
	if (reader.GetTailSize()>clContext.HeaderSize()) {
		const SJointMsgHeader& stHeader=clContext.OnRecvCommHeader(reader);
		stConverter.m_stDst.SetEmpty();
		auto volume=reader.GetTailSize();
		if (volume>SJointCryptoBufferSize) return OnError(clContext, enumCSJointRecvResults::RECV_TOO_LONG);
		else if (volume<SJointCommandHeaderLen) return OnError(clContext, enumCSJointRecvResults::RECV_TOO_SHORT);
		if (stHeader.lSeq!=m_clRemote.m_lSeq) {
			if (OnBreakLine(clContext, stHeader.lSeq)) return OnError(clContext, enumCSJointRecvResults::SEQ_MISMATCH);
		}
		m_clRemote.m_lSeq++;
		switch (stHeader.lCmd) {
		case enumCSJointCommands::SETENV: return OnSetEnv(clContext, reader.Skip(SJointCommandHeaderLen), stHeader.lArgs);
			break;
		case enumCSJointCommands::SETKEY: return OnSetKey(clContext, reader.Skip(SJointCommandHeaderLen), stHeader.lArgs);
			break;
		case enumCSJointCommands::VALIDATE: return OnValidate(clContext, reader.Skip(SJointCommandHeaderLen), stHeader.lArgs, tRecv);
			break;
		case enumCSJointCommands::DATA: return OnData(clContext, reader.Skip(SJointCommandHeaderLen), stHeader.lArgs, stConverter, tRecv);
			break;
		}
		return OnError(clContext, enumCSJointRecvResults::BAD_CMD);
	}
	return OnError(clContext, enumCSJointRecvResults::BAD_HEADER);
}
bool CSJoint::AcceptRemoteKey(CSJointContext& clContext, int64_t lAlg,const CRJStringByte& st) {
	CSJointConverter stConverter;
	CRJSecurityPublicKey clPub;
	if (m_clMe.m_clKeyFile.GetLength()==0) return OnError(clContext, enumCSJointRecvResults::NO_FILENAME_LOCAL);
	if (clPub.LoadFile(m_clMe.m_clKeyFile,false)==CRJSecurityPublicKey::eRJPKIResults::OK) {
		CRJSecurityBuffer clSSLBuf(clPub);
		stConverter.m_stSrc.Set(st);
		clContext.GetTempBuffer(stConverter.m_stDst);
		if (clPub.Decrypt(stConverter.m_stSrc, stConverter.m_stDst, clSSLBuf)==CRJSecurityPublicKey::eRJPKIResults::OK) {
			m_clRemote.SetCipher(*this, GetRemoteCipher(lAlg, stConverter.GetSrcAsDst()));
			lAlg=GetMyKeyForRemoteKey(lAlg);
			if (lAlg!=m_clMe.GetCipher().GetAlg()) return GenMyKey(clContext, lAlg);
			return true;
		}
		else return OnError(clContext, enumCSJointRecvResults::NOT_PKI_DECRYPT);
	}
#ifdef _DEBUG
	CRJDataTools::DebugBreak();
#endif 
	return OnError(clContext, enumCSJointRecvResults::NOT_LOADED_NE);
}
bool CSJoint::OnSetEnv(CSJointContext& clContext, CRJByteReader& reader, int16_t lArgs) {
	CRJStringByteFlex stEnv;
	if (FindBytesArg(enumCSJointTags::ENV_NUMBER, reader, lArgs, stEnv)) return OnReceiveEnv(clContext, stEnv);
	return OnError(clContext, enumCSJointRecvResults::NO_ARG);
}
bool CSJoint::OnSetKey(CSJointContext& clContext, CRJByteReader& reader, int16_t lArgs) {
	int64_t lAlg;
	CRJStringByteFlex stKey;
	if (FindInt64Arg(enumCSJointTags::ALG_CODE, reader, lArgs, lAlg)&&
		FindBytesArg(enumCSJointTags::ALG_KEY, reader, lArgs, stKey)) return AcceptRemoteKey(clContext, lAlg, stKey);
	return OnError(clContext, enumCSJointRecvResults::NO_ARG);
}
bool CSJoint::OnValidate(CSJointContext& clContext, CRJByteReader& reader, int16_t lArgs, time_t tRecv) {
	int64_t lStep;
	if (FindInt64Arg(enumCSJointTags::VALIDATE_STEP, reader, lArgs, lStep)) {
		if (lStep==0) return OnValidateRequest(clContext, reader, lArgs, tRecv);
		else if (lStep==1) return OnValidateResponse(clContext, reader, lArgs, tRecv);
		return OnError(clContext, enumCSJointRecvResults::NO_VALIDATE_STEP);
	}
	return OnError(clContext, enumCSJointRecvResults::NO_ARG);
}
bool CSJoint::OnData(CSJointContext& clContext, CRJByteReader& reader, int16_t lArgs, CSJointConverter& stConverter, time_t tRecv) {
	if (FindBytesArg(enumCSJointTags::DATA_ENCRYPTED, reader, lArgs, stConverter.m_stSrc)) {
		if (!m_clRemote.GetCipher().Decrypt(stConverter)) return OnError(clContext, enumCSJointRecvResults::NOT_DECRYPTED);
		return CheckKeyUpdate(clContext, tRecv);
	}
	return OnError(clContext, enumCSJointRecvResults::NO_ARG);
}
bool CSJoint::OnValidateRequest(CSJointContext& clContext, CRJByteReader& reader, int16_t lArgs, time_t tRecv) {
	uint8_t buffer[256];
	int64_t lIndex;
	CSJointConverter stConverter;
	CRJStringByteFlex stSend;
	if (FindInt64Arg(enumCSJointTags::VALIDATE_INDEX, reader, lArgs, lIndex)&&
		FindBytesArg(enumCSJointTags::DATA_ENCRYPTED, reader, lArgs, stConverter.m_stSrc)) {
		stConverter.SetDstStorage(buffer,sizeof(buffer));
		if (!m_clRemote.GetCipher().Decrypt(stConverter)) return OnError(clContext, enumCSJointRecvResults::NOT_DECRYPTED);
		stConverter.GetSrcAsDst();
		clContext.GetTempBuffer(stConverter.m_stDst);
		if (!m_clMe.GetCipher().Encrypt(stConverter)) return OnError(clContext, enumCSJointRecvResults::NOT_ENCRYPTED);
		clContext.PushCmd(tRecv, enumCSJointCommands::VALIDATE).PushArgInt64(enumCSJointTags::VALIDATE_INDEX, lIndex).PushArgInt64(enumCSJointTags::VALIDATE_STEP, 1).PushArgBytes(enumCSJointTags::DATA_ENCRYPTED, stConverter.GetSrcAsDst());
		if (OnSendCommand(clContext, clContext.GetCommandBuffer(stSend))) return true;
		return OnError(clContext, enumCSJointRecvResults::NOT_SENT);
	}
	return OnError(clContext, enumCSJointRecvResults::NO_ARG);
}
bool CSJoint::OnValidateResponse(CSJointContext& clContext, CRJByteReader& reader, int16_t lArgs, time_t tRecv) {
	int64_t lIndex;
	CSJointConverter stConverter;
	clContext.GetTempBuffer(stConverter.m_stDst);
	if (FindInt64Arg(enumCSJointTags::VALIDATE_INDEX, reader, lArgs, lIndex)&&
		FindBytesArg(enumCSJointTags::DATA_ENCRYPTED, reader, lArgs, stConverter.m_stSrc)) {
		if (!m_clRemote.GetCipher().Decrypt(stConverter)) return OnError(clContext, enumCSJointRecvResults::NOT_DECRYPTED);
		if (lIndex==m_clValidate.m_lValidateIndex) {
			if ((m_clValidate.m_sValidateSize==stConverter.m_stDst.GetLength())&&
				(!memcmp(m_clValidate.m_bValidateBuffer, stConverter.m_stDst.GetHeadForUpdate(), m_clValidate.m_sValidateSize))) return CheckKeyUpdate(clContext, tRecv);
			else return OnError(clContext, enumCSJointRecvResults::NOT_VALIDATED);
		}
		else return OnError(clContext, enumCSJointRecvResults::NOT_SAME_VALIDATE_INDEX);
	}
	return OnError(clContext, enumCSJointRecvResults::NO_ARG);
}
bool CSJoint::CheckKeyUpdate(CSJointContext& clContext, time_t t) {
	if ((t-m_lKeyChanged)>clContext.GetKeyChangeDuration()) GenMyKey(clContext, m_clMe.GetCipher().GetAlg());
	return true;
}
bool CSJoint::OnError(CSJointContext&, int64_t) {
	return false;
}
bool CSJoint::OnReceiveEnv(CSJointContext&,const CRJStringByteFlex&) {
	return true;
}
CSJointEndPointCipher* CSJoint::GetRemoteCipher(int64_t lAlg,const CRJStringByteFlex& st) {
	try {
		if (lAlg==SJointEndPointCipherDes) {
			if (st.GetLength() == cUSDESKeyLong) {
				CSJointEndPointCipherDes* p=(CSJointEndPointCipherDes*) GetCipher(lAlg, st);
				if (p) return p;
			}
		}
		else if (lAlg==SJointEndPointCipherAes) {
			if (st.GetLength() ==(cSeptadaAES256Key+cSeptadaAES256IV)) {
				CSJointEndPointCipherAes* p=(CSJointEndPointCipherAes*) GetCipher(lAlg, st);
				if (p) return p;
			}
		}
	}
	catch(...) {}
	return & CSJointEndPoint::s_CipherEmpty;
}
CSJointEndPointCipher* CSJoint::GetMyCipher(time_t t, int64_t lAlg, CRJStringByteVarAssigned& st) {
	try {
		uint8_t* pData=st.GetHeadForUpdate();
		CRJStringByteFlex stRead;
		if (lAlg==SJointEndPointCipherDes) {
			CSJointEndPointCipherDes* p;
			srand((unsigned int) t);
			for (size_t i=0; i<cUSDESKeyLong; i++) pData[i]=(uint8_t)(rand()& 0xFF);
			p=(CSJointEndPointCipherDes*) GetCipher(lAlg, stRead.SetFromData(pData, cUSDESKeyLong));
			if (p) return p;
		}
		else if (lAlg==SJointEndPointCipherAes) {
			CSJointEndPointCipherAes* p;
			srand((unsigned int) t);
			for (size_t i=0; i< (cSeptadaAES256Key + cSeptadaAES256IV); i++) pData[i]=(uint8_t)(rand()& 0xFF);
			p=(CSJointEndPointCipherAes*) GetCipher(lAlg, stRead.SetFromData(pData, cSeptadaAES256Key + cSeptadaAES256IV));
			if (p) return p;
		}
	}
	catch(...) {}
	st.SetEmpty();
	return & CSJointEndPoint::s_CipherEmpty;
}
bool CSJoint::FindInt64Arg(int16_t lTag,const CRJByteReader& src, int16_t lCount, int64_t& lFound) {
	int16_t i, l, t;
	CRJByteReader reader(src.Head(), src.GetTailSize());
	for (i=0; i<lCount; i++) {
		if (reader.Int16(t)&&
			reader.Int16(l)) {
			if (l<=0) return false;
			if (lTag==t) {
				if (l==SJointArgInt64Len) return reader.Int64(lFound);
			}
			if (!reader.Skip(l)) return false;
		}
		else break;
	}
	return false;
}
bool CSJoint::FindBytesArg(int16_t lTag,const CRJByteReader& src, int16_t lCount, CRJStringByteFlex& stFound) {
	int16_t i, l, t;
	CRJByteReader reader(src.Head(), src.GetTailSize());
	for (i=0; i<lCount; i++) {
		if (reader.Int16(t)&&
			reader.Int16(l)) {
			if (l<=0) return false;
			if (lTag==t) return reader.Data(stFound, l);
			if (!reader.Skip(l)) return false;
		}
		else break;
	}
	return false;
}

