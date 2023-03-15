#pragma once
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/datatools/bytereader.h>
#include <recurj/rjframework/libs/base/datatools/bytewriter.h>
#include <recurj/rjframework/libs/base/datatools/result.h>
#include <recurj/rjframework/libs/base/strings/byte/flex.h>
#define PushCRLF() Push(CRJStringByte::s_CRLF)
namespace rjBase {
	class CRJStringByte;
	class CRJStringByteVar;
	class RJBASE_API CRJByteStorage {
		friend class CRJByteStorageReader;
	public:
		CRJByteStorage(CRJStringByteFlex&);
		CRJByteStorage(uint8_t*, size_t);
		CRJByteStorage();
	public:
		virtual~CRJByteStorage();
		CRJByteStorage& ResetCollector();
		CRJByteStorage& Processed(size_t sCount=1);
		CRJByteStorage& Normalize();
		void SetBuffer(uint8_t* p, size_t);
		inline CRJStringByteVarAssigned& GetStorage() {
			return m_clStorage;
		}
		inline const uint8_t* GetNotProcessedHead() const {
			return m_clStorage.GetHeadForRead()+m_sProcessed;
		}
		inline size_t GetNotProcessedSize() const {
			return m_clStorage.GetLength()-m_sProcessed;
		}
		inline size_t GetProcessedSize() const {
			return m_sProcessed;
		}
	protected:
		CRJStringByteVarAssigned m_clStorage;
		size_t m_sProcessed;
	};
	template<size_t VOLUME>
	class CRJByteStorageAssigned:public CRJByteStorage {
	public:
		CRJByteStorageAssigned()noexcept {
			m_clStorage.SetStorage(m_uBuffer, VOLUME);
		}
	protected:
		uint8_t m_uBuffer[VOLUME];
	};
	class RJBASE_API CRJByteStorageClearner {
	public:
		CRJByteStorageClearner(CRJByteStorage& cl):
			m_clCollector(cl) {};
		~CRJByteStorageClearner() {
			m_clCollector.Normalize();
		}
		CRJByteStorage& m_clCollector;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJByteStorageClearner)
	};
	class RJBASE_API CRJByteStorageReader:public CRJByteReader {
	public:
		CRJByteStorageReader(CRJByteStorage& cl):
			CRJByteReader(cl.GetNotProcessedHead(), cl.GetNotProcessedSize()),
		m_clCollector(cl) {};
		~CRJByteStorageReader() {
			if (m_sOffset>0) m_clCollector.Processed(m_sOffset).Normalize();
		}
		CRJByteStorage& m_clCollector;
		_CLASS_COPY_AND_ASSIGN_DELETE(CRJByteStorageReader)
	};
}

