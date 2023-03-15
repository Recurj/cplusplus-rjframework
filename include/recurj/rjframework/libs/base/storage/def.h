#pragma once
#include <recurj/rjframework/libs/base/memory/allocator.h>
#include <recurj/rjframework/libs/base/collections/btrees/index.h>
#define RJ_DATASTORE_TREE_KEY(_lIndex) (size_t)((_lIndex>>CRJDatastoreRegionShift)&0x00FFFFFFFF)
#define RJ_DATASTORE_TREE_INDEX(_lIndex) (size_t)(_lIndex&CRJDatastoreRegionMask)
namespace rjBase {
	class CRJDataStorage;
	constexpr size_t CRJSizeDatastoreRegion = 256;
	constexpr size_t CRJDatastoreRegionShift = 8;
	constexpr size_t CRJDatastoreRegionMask = 0x00FF;
}

