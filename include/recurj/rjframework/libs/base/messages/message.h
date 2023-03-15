#pragma once
#include <recurj/rjframework/libs/base/collections//lists/defs.h>
#include <recurj/rjframework/libs/base/datetime/dtimebinary.h>
#include <recurj/rjframework/libs/base/messages/known.h>
namespace rjBase {
	enum
		class EMessageserialize {
		Error, NotSupported, OK
	};
	class CRJLogger;
	class CRJByteWriter;
	class CRJTask;
	class RJBASE_API CRJMessage :public CRJListItem<CRJMessage> {
		friend class CRJListMessages;
		friend class CRJTaskQueue;
	public:
		static CRJMessage* Get(size_t, size_t);
		virtual~CRJMessage();
		virtual bool Log(CRJLogger&);
		virtual CRJMessage* Duplicate() const;
		virtual EMessageserialize Serialize(CRJByteWriter&);
		virtual bool Pack(CRJStringByteVar&);
		virtual bool Unpack(const CRJStringByte&);
		inline CRJMessage& GetItself() {return *this;}
		inline size_t GetRegion() const {return m_sMsgRegion;}
		inline size_t GetCommand() const {return m_sMsgCmd;}
		inline CRJMessage& SetRegion(size_t c) {m_sMsgRegion = c;return *this;}
		inline CRJMessage& SetCommand(size_t c) {m_sMsgCmd = c;	return *this;}
		inline CRJMessage& SetRegionAndCommand(size_t r, size_t c) {m_sMsgRegion = r;m_sMsgCmd = c;return *this;}
		static bool Send(CRJTask&, size_t, size_t);
		static bool SendUrgent(CRJTask&, size_t, size_t);
		static void Done(CRJMessage*);

	protected:
		CRJMessage();
		CRJMessage(size_t, size_t);
		CRJMessage(const CRJMessage&);
	private:
		static void freeMessage(CRJMessage*);
	protected:
		size_t m_sMsgRegion, m_sMsgCmd;
	};
}

