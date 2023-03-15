#pragma once
#include <recurj/rjframework/libs/base/strings/byte/string.h>
namespace rjBase {
	struct RemoteAppRegion {
		static const int64_t Undefined=1;
		static const int64_t System=2;
		static const int64_t Security=3;
		static const int64_t Storage=4;
		static const int64_t Format=5;
		static const int64_t Session=6;
		static const int64_t Property=7;
		static const int64_t Table=8;
		static const int64_t Server=101;
		static const int64_t Client=102;
		static const int64_t User=103;
		static const int64_t Network=104;
	};
	struct RemoteAppRegionSystem {
		static const int64_t Exception=1;
		static const int64_t AsyncError=2;
		static const int64_t Fatal=3;
		static const int64_t BadServer = 4;
	};
	struct RemoteAppRegionSecurity {
		static const int64_t InstallPublic=1;
		static const int64_t Generate=2;
		static const int64_t ServerPublic=3;
		static const int64_t WorkplacePrivate=4;
		static const int64_t WorkplacePublic=5;
	};
	struct RemoteAppRegionStorage {
		static const int64_t NoId=1;
		static const int64_t ReadFile=2;
		static const int64_t SaveFile=3;
		static const int64_t NoData=4;
	};
	struct RemoteAppRegionFormat {
		static const int64_t ParserHeader=1;
		static const int64_t ParserLineBadSeparator=2;
		static const int64_t ParserLineBadValue=3;
		static const int64_t ParserTotal=4;
		static const int64_t ParserStart = 5;
		static const int64_t Build = 6;
	};
	struct RemoteAppRegionSession {
		static const int64_t NoController=1;
		static const int64_t NoMethod=2;
		static const int64_t NoData=3;
	};
	struct RemoteAppRegionProperty {
		static const int64_t NoId=1;
		static const int64_t NoData=2;
		static const int64_t UnknownId=3;
	};
	struct RemoteAppRegionTable {
		static const int64_t BadId=1;
		static const int64_t BadFields=2;
		static const int64_t BadView=3;
		static const int64_t BadSQL=4;
		static const int64_t BadSection=5;
		static const int64_t ConfigNameEmpty=6;
		static const int64_t ConfigNameExist=7;
		static const int64_t ConfigNameNotExist=8;
		static const int64_t ConfigCreate=9;
		static const int64_t ConfigPropertyEmpty=10;
	};
	struct RemoteAppRegionServer {
		static const int64_t Fatal=1;
		static const int64_t HandbookNotFound=2;
	};
	struct RemoteAppRegionClient {
		static const int64_t Fatal = 1;
		static const int64_t NoApp=2;
		static const int64_t NoAppInit=3;
		static const int64_t NoRequest=4;
		static const int64_t NotImplemented = 5;
		static const int64_t appData=1024;
	};
	struct RemoteAppRegionUser {
		static const int64_t Denied=1;
		static const int64_t PasswordReset=2;
		static const int64_t NoId=3;//	      нет идентификатора
		static const int64_t NoPassword=4;//	      нет пароля
		static const int64_t Duplicated=5;//	      уже есть пользователь с таким именем
		static const int64_t NoSentEMail=6;
		static const int64_t CodeExpired=7;
		static const int64_t CodeInvalid=8;
		static const int64_t NoSaveNick=9;
		static const int64_t Expired=10;
		static const int64_t ApplicationNo=21;
		static const int64_t ApplicationBlocked=22;
	};
	struct RemoteAppRegionNetwork {
		static const int64_t Http=1;
		static const int64_t Connect=2;
	};
	class RJBASE_API RemoteAppError {
	public:
		static const int64_t No=0;
		static int64_t asyncErr();
		static int64_t exeption();
		static int64_t build(int64_t, int64_t);
	};
}

