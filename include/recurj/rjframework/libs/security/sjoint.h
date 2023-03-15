#pragma once
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
#include <recurj/rjframework/libs/base/datatools/bytereader.h>
#include <recurj/rjframework/libs/security/deskey.h>
#include <recurj/rjframework/libs/security/opensslcipher.h>
namespace rjSecurity {
	const int64_t SJointValidateBufferSize=128;
	const int64_t SJointCommandHeaderLen=8;
	const int64_t SJointArgFieldLen=2;
	const int64_t SJointArgInt64Len=8;
	const int64_t SJointArgValueOffset=4;
	const int64_t SJointCryptoBufferSize=8096;
	const int64_t SJointCommandBufferSize=SJointCryptoBufferSize+SJointCommandHeaderLen+SJointArgValueOffset;
	struct SJointMsgHeader {
		uint16_t lSeq;
		int8_t lVersion;
		int8_t lSubVersion;
		int16_t lCmd;
		int16_t lArgs;
	};
	struct SJointArgHeader {
		int16_t lTag;
		int16_t lLen;
	};
	enum enumCSJointRecvResults {
		UNKNOWN=-1, RECV_TOO_SHORT=1, RECV_TOO_LONG=2, SEQ_MISMATCH=3, BAD_CMD=4, NO_ARG=5,
		NOT_ACCEPTED_KEY=6, NOT_DECRYPTED=7, NOT_ENCRYPTED=8, NO_VALIDATE_STEP=9, NOT_SENT=10,
		NOT_PKI_CRYPT=11, NOT_LOADED_REMOTE=12, NOT_PKI_DECRYPT=13, NOT_LOADED_NE=14, NOT_VALIDATED=15,
		NOT_SAME_VALIDATE_INDEX=16, RECV_LEN_INVALID=17, NO_FILENAME_LOCAL=18, NO_FILENAME_REMOTE=19,
		BAD_HEADER=20
	};
	enum enumCSJointCommands {
		SETENV=0, SETKEY=1, VALIDATE=2, DATA=3
	};
	enum enumCSJointTags {
		ENV_NUMBER=0, ALG_CODE=1, ALG_KEY=2, VALIDATE_INDEX=3, VALIDATE_STEP=4, DATA_ENCRYPTED=5
	};
	struct RJSECURITY_API CSJointConverter {
		CRJStringByteFlex m_stSrc;
		CRJStringByteVarAssigned m_stDst;
		inline CSJointConverter& SetSrc(const uint8_t* p, size_t c) {
			m_stSrc.SetFromData(p, c);
			return *this;
		}
		inline CSJointConverter& SetDstStorage(uint8_t* p, size_t c) {
			m_stDst.SetStorage(p, c);
			return *this;
		}
		CRJStringByteFlex& GetSrcAsDst() {
			return m_stSrc.Set(m_stDst);
		}
	};
	class RJSECURITY_API CSJointEndPointCipher {
	public:
		virtual int64_t GetAlg()=0;
		virtual bool Encrypt(CSJointConverter&)=0;
		virtual bool Decrypt(CSJointConverter&)=0;
	};
	const int64_t SJointEndPointCipherEmpty=-1;
	const int64_t SJointEndPointCipherDes=0;
	const int64_t SJointEndPointCipherAes=1;
	class RJSECURITY_API CSJointEndPointCipherEmpty:public CSJointEndPointCipher {
	public:
		virtual int64_t GetAlg() {
			return SJointEndPointCipherEmpty;
		}
		virtual bool Encrypt(CSJointConverter&) {
			return false;
		};
		virtual bool Decrypt(CSJointConverter&) {
			return false;
		};
	};
	class RJSECURITY_API CSJointEndPointCipherDes:public CSJointEndPointCipher {
	public:
		static CSJointEndPointCipherDes* Create(const CRJStringByteFlex&);
		static void Free(CSJointEndPointCipherDes*);
		virtual~CSJointEndPointCipherDes();
		virtual int64_t GetAlg() {
			return SJointEndPointCipherDes;
		}
		virtual bool Encrypt(CSJointConverter&);
		virtual bool Decrypt(CSJointConverter&);
	protected:
		CSJointEndPointCipherDes(const CRJStringByteFlex&);
	protected:
		CRJDESKey m_clKey;
	};
	class RJSECURITY_API CSJointEndPointCipherAes:public CSJointEndPointCipher {
	public:
		static CSJointEndPointCipherAes* Create(const CRJStringByteFlex&);
		static void Free(CSJointEndPointCipherAes*);
		virtual~CSJointEndPointCipherAes();
		virtual int64_t GetAlg() {
			return SJointEndPointCipherAes;
		}
		virtual bool Encrypt(CSJointConverter&);
		virtual bool Decrypt(CSJointConverter&);
	protected:
		CSJointEndPointCipherAes(const CRJStringByteFlex&);
	protected:
		CRJSecurityCipherAES256 m_clKey;
	};
	class RJSECURITY_API CSJointValidate {
	public:
		CSJointValidate();
		virtual~CSJointValidate();
		void Reset();
		uint8_t m_bValidateBuffer[SJointValidateBufferSize];
		int64_t m_lValidateIndex;
		size_t m_sValidateSize;
	};
	class RJSECURITY_API CSJointContext {
	public:
		CSJointContext(size_t);
		virtual~CSJointContext();
		virtual int64_t GetKeyChangeDuration();
		virtual SJointMsgHeader& OnSendCommHeader(time_t)=0;
		SJointMsgHeader& OnRecvCommHeader(CRJByteReader&);
		CSJointContext& PushCmd(time_t, int16_t);
		CSJointContext& PushArgInt64(int16_t, int64_t);
		CSJointContext& PushArgBytes(int16_t,const CRJStringByteFlex&);
		CSJointContext& WasAddedArg();
		CRJStringByteFlex& GetCommandBuffer(CRJStringByteFlex&);
		const CRJStringByteVarAssigned& GetTempBuffer(CRJStringByteVarAssigned&) const;
		SJointMsgHeader& GetHeader(const uint8_t*) const;
		inline size_t HeaderSize() const {
			return m_lHeaderSize;
		}
	protected:
		uint8_t m_bCommandBuffer[SJointCommandBufferSize];
		uint8_t m_bTempBuffer[SJointCommandBufferSize];
		size_t m_lCommandSize;
	private:
		const size_t m_lHeaderSize;
	};
	class CSJointManager {
	public:
		virtual int64_t GetMyKeyForRemoteKey(int64_t)=0;
		virtual CSJointEndPointCipher* GetCipher(int64_t,const CRJStringByteFlex&)=0;
		virtual void FreeCipher(CSJointEndPointCipher&)=0;
	};
	class RJSECURITY_API CSJoint:public CSJointManager {
	public:
		CSJoint();
		virtual~CSJoint();
		virtual CSJoint& OpenSession();
		bool SendEnv(CSJointContext&,const CRJStringByte&);
		bool Receive(CSJointContext&, CSJointConverter&, time_t);
		bool SendPrepare(CSJointContext&, CSJointConverter&);
		bool SendData(CSJointContext&,const CRJStringByteFlex&, size_t, time_t,bool);
		void SendMarker(CSJointContext&,const CRJStringByteFlex&);
		bool Validate(CSJointContext&);
		bool GenMyKey(CSJointContext&, int64_t);
		void SetMePublic(const CRJStringByte& fn) {
			m_clMe.m_clKeyFile.Set(fn);
		}
		void SetRemotePublic(const CRJStringByte& fn) {
			m_clRemote.m_clKeyFile.Set(fn);
		}
		bool CheckKeyUpdate(CSJointContext&, time_t);
	protected:
		virtual bool OnError(CSJointContext&, int64_t);
		virtual bool OnReceiveEnv(CSJointContext&,const CRJStringByteFlex&);
		virtual bool OnBreakLine(CSJointContext&, uint16_t)=0;
		virtual bool OnSendData(CSJointContext&,const CRJStringByteFlex&)=0;
		virtual bool OnSendCommand(CSJointContext&,const CRJStringByteFlex&)=0;
		CSJointEndPointCipher* GetMyCipher(time_t, int64_t, CRJStringByteVarAssigned&);
		CSJointEndPointCipher* GetRemoteCipher(int64_t,const CRJStringByteFlex&);
		bool OnSetEnv(CSJointContext&, CRJByteReader&, int16_t);
		bool OnSetKey(CSJointContext&, CRJByteReader&, int16_t);
		bool OnValidate(CSJointContext&, CRJByteReader&, int16_t, time_t);
		bool OnData(CSJointContext&, CRJByteReader&, int16_t, CSJointConverter&, time_t);
		bool OnValidateRequest(CSJointContext&, CRJByteReader&, int16_t, time_t);
		bool OnValidateResponse(CSJointContext&, CRJByteReader&, int16_t, time_t);
		bool AcceptRemoteKey(CSJointContext&, int64_t,const CRJStringByte&);
		bool FindInt64Arg(int16_t,const CRJByteReader&, int16_t, int64_t&);
		bool FindBytesArg(int16_t,const CRJByteReader&, int16_t, CRJStringByteFlex&);
		class RJSECURITY_API CSJointEndPoint final {
		public:
			CSJointEndPoint();
			~CSJointEndPoint();
			void Reset();
			void SetCipher(CSJointManager&, CSJointEndPointCipher*);
			void FreeCipher(CSJointManager&);
			inline CSJointEndPointCipher& GetCipher() {
				return * m_pCipher;
			}
			static CSJointEndPointCipherEmpty s_CipherEmpty;
			CRJStringByteAllocated m_clKeyFile;
			uint16_t m_lSeq;
		private:
			CSJointEndPointCipher* m_pCipher;
		};
	protected:
		CSJointEndPoint m_clMe, m_clRemote;
		CSJointValidate m_clValidate;
		time_t m_lKeyChanged;
	};
}

