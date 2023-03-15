#include <local/rj/libs/base/application/application.h>
#include <local/rj/modules/defs/moddeftesttools.h>
using namespace std::chrono;
namespace rjTests {
	constexpr int32_t TestOK = 0;
	constexpr int32_t TestInProcess = 1;
	constexpr int32_t TestFailed = -1;
	constexpr int32_t TestNoMemory = -2;
	constexpr int32_t TestNoInsert = -3;
	constexpr int32_t TestNoDelete = -4;
	constexpr int32_t TestNoUpdate = -5;
	constexpr int32_t TestException = -6;

	constexpr size_t TestActionDone = 0;
	constexpr size_t TestActionInsert = 1;
	constexpr size_t TestActionDelete = 2;
	constexpr size_t TestActionCheck1 = 1024;

	class RJTESTTOOLS_API CRJApplicationTest:public CRJApplication {
	public:
		CRJApplicationTest(CRJLoggerWriter&);
		virtual~CRJApplicationTest();
		void Results();
		void Test(const CRJStringByte&, size_t);
		void Test(const CRJStringByte&, size_t,const CRJStringByte&);
		void Test(const CRJStringByte&, size_t, size_t);
		void OK();
		void Failed();
		void Error(const CRJStringByte&);
		void ErrorNotSame(size_t, size_t);
		void ErrorNotSameSize(size_t, size_t);
		void ErrorNotSame(const CRJStringByte&,const CRJStringByte&);
		void Warning(const CRJStringByte&);
		const CRJStringByte& GetTestName() const {
			return m_clName;
		}
	protected:
		size_t m_sTotals, m_sErrors, m_sWarnings;
		CRJStringByteExpanded m_clName, m_clDetails, m_clLog;
		milliseconds m_testStart;
	};
	class RJTESTTOOLS_API CRJTest {
	public:
		CRJTest(CRJApplicationTest&,const CRJStringByte&);
		void Next();
		void Next(const CRJStringByte&);
		void Next(size_t);
	public:
		CRJApplicationTest& m_clApp;
		CRJStringByteDefault m_clName;
		size_t m_sCount;
	};
}

