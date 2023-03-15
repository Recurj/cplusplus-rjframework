#pragma once
#include <recurj/rjframework/libs/base/strings/byte/allocated.h>
namespace rjBase {
	enum ENoMemory:int64_t {
		AppNoMemoryCustom=1024,
	};
	enum EErrorApplication:int64_t {
		AppErrNoMemory=1, AppErrNoApplication=1, AppErrOptionUnexpected, AppErrOptionAbsent,
		AppErrOptionBad, AppErrOptionBadDebug, AppErrConfigAbsent, AppErrConfigBad, AppErrCmdValuesCount,
		AppErrBadData, AppErrCustom=1024
	};
	enum EErrorOs:int64_t {
		AppErrHandleInvalid=101, AppErrHeapInvalid, AppErrSharedMemoryCreate=110, AppErrSharedMemoryRead,
		AppErrSharedMemoryWrite, AppErrMutexCreate=120, AppErrMutexOpen, AppErrMutexEnter,
		AppErrMutexRelease, AppErrQueueCreate=130, AppErrQueueWait, AppErrRegistryCreate=140,
		AppErrRegistryOpen, AppErrRegistryEnum, AppErrRegistryRead, AppErrRegistryWrite, AppErrRegistryDropValue,
		AppErrRegistryDropKey, AppErrDllLoadInteface=150, AppErrSendMsg=160, AppErrThread=160
	};
	enum EErrorFile:int64_t {
		AppErrFileCreatePath=201, AppErrFileCreate, AppErrFileOpen, AppErrFileRead, AppErrFileSave,
		AppErrFileBackup, AppErrFileLoad=211, AppErrFileParse, AppErrFileProcess, AppErrFileReadEncoding
	};
}

