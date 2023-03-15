#include "stdafx.h"
#include <recurj/rjframework/libs/base/file/filewalker.h>
using namespace rjBase;
CRJFileWalker::CRJFileWalker() {}
CRJFileWalker::~CRJFileWalker() {}
bool CRJFileWalker::OnDirectory(const fs::directory_entry&) {
	return true;
}
bool CRJFileWalker::OnFile(const fs::directory_entry&) {
	return true;
}
bool CRJFileWalker::OnLink(const fs::directory_entry& entry) {
	return OnFile(entry);
}
bool CRJFileWalker::ScanLocal(const CRJFileName& fn) {
	fs::path clPath(fn.GetHeadForRead(), fn.GetTail());
	if (fs::exists(clPath)&&
		fs::is_directory(clPath)) {
		for (auto const& entry:fs::directory_iterator(clPath)) {
			if (fs::is_directory(entry.status())) {
				if (!OnDirectory(entry)) return false;
			}
			else if (fs::is_symlink(entry.status())) {
				if (!OnLink(entry)) return false;
			}
			else if (fs::is_regular_file(entry.status())) {
				if (!OnFile(entry)) return false;
			}
		}
		return true;
	}
	return false;
}
bool CRJFileWalker::ScanTotal(const CRJFileName& fn) {
	fs::path clPath(fn.GetHeadForRead(), fn.GetTail());
	if (fs::exists(clPath)) {
		if (fs::is_directory(clPath)) {
			for (auto const& entry:fs::recursive_directory_iterator(clPath)) {
				if (fs::is_directory(entry.status())) {
					if (!OnDirectory(entry)) return false;
				}
				else if (fs::is_symlink(entry.status())) {
					if (!OnLink(entry)) return false;
				}
				else if (fs::is_regular_file(entry.status())) {
					if (!OnFile(entry)) return false;
				}
			}
		}
		else {
			fs::directory_entry entry(clPath);
			return OnFile(entry);
		}
		return true;
	}
	return false;
}
bool CRJFileWalker::ScanCurrent(const CRJFileName& fn) {
	fs::path clPath(fn.GetHeadForRead(), fn.GetTail());
	if (fs::exists(clPath)) {
		if (fs::is_directory(clPath)) {
			for (auto const& entry:fs::recursive_directory_iterator(clPath)) {
				if (fs::is_directory(entry.status()))continue;
				if (fs::is_symlink(entry.status())) {
					if (!OnLink(entry)) return false;
				}
				else if (fs::is_regular_file(entry.status())) {
					if (!OnFile(entry)) return false;
				}
			}
		}
		return true;
	}
	return false;
}

