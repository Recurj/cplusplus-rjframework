#include "stdafx.h"
#include <recurj/rjframework/libs/base/strings/filename.h>
#include <recurj/rjframework/libs/base/application/application.h>
#include <recurj/rjframework/libs/base/strings/dtime.h>
#include <chrono>
using namespace rjBase;
CRJFileName::CRJFileName() {}
CRJFileName::CRJFileName(const fs::path& fp) {
	SetPath(fp);
}
CRJFileName::CRJFileName(const CRJStringUnicode& s) {
	Set(s);
}
CRJFileName::CRJFileName(const wchar_t* p) {
	SetLabel(p);
}
CRJFileName::CRJFileName(const CRJStringByte& s) {
	Set(s);
}
CRJFileName::CRJFileName(const char* p) {
	SetLabel(p);
}
CRJFileName::CRJFileName(const CRJFileName& fn) {
	Set(fn);
}
CRJFileName::~CRJFileName() {}
CRJFileName& CRJFileName::Unify() {
	CRJFilePath p(*this);
	return SetPath(p.Unify());
}
CRJFileName& CRJFileName::CatSeparator() {
	return (CRJFileName&) CatSingle(fs::path::preferred_separator);
}
CRJFileName& CRJFileName::SetPath(const fs::path& p) {
	const auto& cs=p.native();
	return (CRJFileName&) SetData(cs.c_str(), cs.length());
}
bool CRJFileName::IsPathAbsolute() const {
	fs::path clPath(GetHeadForRead(), GetTail());
	return clPath.is_absolute();
}
CRJFileName& CRJFileName::BuildPathAbsolute(const CRJFileName& root) {
	if (!IsPathAbsolute()) {
		CRJFileName fn(root);
		fn.Cat(*this);
		fs::path clPath(fn.GetHeadForRead(), fn.GetTail());
		SetPath(clPath.make_preferred().lexically_normal());
	}
	return *this;
}
CRJFileName& CRJFileName::BuildPath() {
	if (!IsEmpty()&&
		!IsLastThat(fs::path::preferred_separator)) CatSeparator();
	return *this;
}
CRJFileName& CRJFileName::BuildPath(const CRJStringUnicode& v) {
	Set(v);
	return BuildPath();
}
CRJFileName& CRJFileName::BuildPath(const CRJStringByte& v) {
	Set(v);
	return BuildPath();
}
CRJFileName& CRJFileName::GetAppExec() {
	CRJStringLong s;
#ifdef _rjOS_WIN
	HMODULE hModule=GetModuleHandleW(NULL);
	GetModuleFileNameW(hModule, s.GetHeadForUpdate(),(DWORD)(s.GetSize()-1));
	s.CalcCLength();
#else 
	s.Set(arg);
#endif 
	Set(s);
	return *this;
}
CRJFileName& CRJFileName::GetAppTemp() {
	SetPath(fs::temp_directory_path());
	return *this;
}
CRJFileName& CRJFileName::GetPath(CRJFileName& v) const {
	CRJFilePath fp(*this);
	return fp.GetPath(v);
}
CRJFileName& CRJFileName::GetNameAndExt(CRJFileName& v) const {
	CRJFilePath fp(*this);
	return fp.GetNameAndExt(v);
}
CRJFileName& CRJFileName::GetName(CRJFileName& v) const {
	CRJFilePath fp(*this);
	return fp.GetName(v);
}
CRJFileName& CRJFileName::GetExt(CRJFileName& v) const {
	CRJFilePath fp(*this);
	return fp.GetExt(v);
}
CRJFileName& CRJFileName::RelativeMe(CRJFileName& v) const {
	CRJFilePath fp(*this);
	return fp.RelativeMe(v);
}

bool CRJFileName::HasExtentionLowCase(const CRJString&ext) {
	CRJFileName fn;
	GetExt(fn);
	return fn.LowerCase()==ext;
}
bool CRJFileName::IsFileExist() const {
	std::error_code clError;
	fs::path clPath(GetHeadForRead(), GetTail());
	return fs::exists(clPath, clError);
}
bool CRJFileName::IsDirExist() const {
	std::error_code clError;
	fs::path clPath(GetHeadForRead(), GetTail());
	return fs::exists(clPath, clError)&&
		fs::is_directory(clPath);
}
bool CRJFileName::GetFileSize(size_t& s) const {
	std::error_code clError;
	fs::path clPath(GetHeadForRead(), GetTail());
	s=fs::file_size(clPath, clError);
	return !clError;
}
time_t CRJFileName::GetLastUpdated() const {
	std::error_code clError;
	fs::path clPath(GetHeadForRead(), GetTail());
	auto lwt=fs::last_write_time(clPath, clError);
	if (!clError) {
#ifdef _rjOS_FAMILY_UNIX
		return decltype(lwt)::clock::to_time_t(lwt);
		//	                 t = ((time_t)(lwt.time_since_epoch().Count()));
#endif 
#ifdef _rjOS_WIN
		//	                 t = std::chrono::duration_cast<std::chrono::seconds>(lwt.time_since_epoch()).Count();
		return ((time_t)(lwt.time_since_epoch().count()-EPOCH_BIAS)/ 10000000LL);
#endif 
	}
	return 0;
}
bool CRJFileName::CreateFilePath() const {
	std::error_code clError;
	fs::path clPath(GetHeadForRead(), GetTail());
	fs::path t=clPath.parent_path();
	fs::create_directories(t, clError);
	return !clError;
}
bool CRJFileName::CreateDir() const {
	if (IsDirExist()) return true;
	if (CreateFilePath()) {
		std::error_code clError;
		fs::path clPath(GetHeadForRead(), GetTail());
		return fs::create_directory(clPath, clError);
	}
	return false;
}
bool CRJFileName::Copy(const CRJFileName& clNew, fs::copy_options stOptions) const {
	std::error_code clError;
	fs::path clPath(GetHeadForRead(), GetTail());
	fs::path clPathNew(clNew.GetHeadForRead(), clNew.GetTail());
	return fs::copy_file(clPath, clPathNew, stOptions, clError);
}
bool CRJFileName::Move(const CRJFileName& clNew) const {
	std::error_code clError;
	fs::path clPath(GetHeadForRead(), GetTail());
	fs::path clPathNew(clNew.GetHeadForRead(), clNew.GetTail());
	fs::rename(clPath, clPathNew, clError);
	return !clError;
}
bool CRJFileName::RenameMe(const CRJFileName& clNew) const {
	std::error_code clError;
	fs::path clPath(GetHeadForRead(), GetTail());
	fs::path clPathNew(clNew.GetHeadForRead(), clNew.GetTail());
	fs::rename(clPath, clPathNew, clError);
	return !clError;
}
bool CRJFileName::RenameToMe(const CRJFileName& clOld) const {
	fs::path clPathOld(clOld.GetHeadForRead(), clOld.GetTail());
	return RenameToMe(clPathOld);
}
bool CRJFileName::RenameToMe(const fs::path& clPathOld) const {
	std::error_code clError;
	fs::path clPath(GetHeadForRead(), GetTail());
	fs::rename(clPathOld, clPath, clError);
	return !clError;
}
bool CRJFileName::RemoveSingle() const {
	std::error_code clError;
	fs::path clPath(GetHeadForRead(), GetTail());
	return fs::remove(clPath, clError);
}
bool CRJFileName::RemoveAll() const {
	std::error_code clError;
	fs::path clPath(GetHeadForRead(), GetTail());
	return fs::remove_all(clPath, clError);
}
bool CRJFileName::SetSameTime(const CRJFileName& clDst) {
	std::error_code clError;
	fs::path clPath(GetHeadForRead(), GetTail());
	fs::path clPathDst(clDst.GetHeadForRead(), clDst.GetTail());
	auto lwt=fs::last_write_time(clPath, clError);
	if (!clError) fs::last_write_time(clPathDst, lwt, clError);
	return !clError;
}
bool CRJFileName::GetLastUpdated(CRJStringDTime& dt) {
	time_t t=GetLastUpdated();
	if (t>0) {
		dt.SetDTAppl(t);
		return true;
	}
	dt.SetEmpty();
	return false;
}
bool CRJFileName::Backup() const {
	CRJFileName fnBak(*this);
	fnBak.CatLabel(L".bak");
	if (fnBak.IsFileExist()) fnBak.RemoveSingle();
	return RenameMe(fnBak);
}
CRJFileNameWithLastUpdate::CRJFileNameWithLastUpdate() {
	m_dtUpdate=0;
}
CRJFileNameWithLastUpdate::~CRJFileNameWithLastUpdate() {}
bool CRJFileNameWithLastUpdate::NeedReload(bool bLoaded,const CRJFileName& fn) {
	time_t t=fn.GetLastUpdated();
	if (!bLoaded||
		*this!=fn) {
		Set(fn);
		m_dtUpdate=t;
		return true;
	}
	if (t!=m_dtUpdate) {
		m_dtUpdate=t;
		return true;
	}
	return false;
}
CRJFilePath::CRJFilePath() {}
CRJFilePath::CRJFilePath(const CRJFileName& fn):
	m_clPath(fn.GetHeadForRead(), fn.GetTail()) {}
CRJFilePath::~CRJFilePath() {}
CRJFilePath& CRJFilePath::Unify() {
	m_clPath=fs::canonical(m_clPath);
	return *this;
}
CRJFileName& CRJFilePath::GetAppWorking(CRJFileName& fn) {
	fn.SetPath(fs::current_path());
	return fn;
}
bool CRJFilePath::SetAppWorking(CRJFileName& fn) {
	CRJFilePath path(fn);
	std::error_code clError;
	fs::current_path(path.m_clPath, clError);
	return !clError;
}
CRJFilePath& CRJFilePath::Set(const CRJFileName& fn) {
	m_clPath.assign(fn.GetHeadForRead(), fn.GetTail());
	return *this;
}
CRJFileName& CRJFilePath::RelativeMe(CRJFileName& fn) const {
	CRJFilePath fp(fn);
	return fn.SetPath(fs::proximate(fp.m_clPath, m_clPath));
}
size_t CRJFilePath::GetPathLength() const {
	return m_clPath.parent_path().native().length();
}
size_t CRJFilePath::GetNameLength() const {
	return m_clPath.stem().native().length();
}
size_t CRJFilePath::GetExtLength() const {
	return m_clPath.extension().native().length();
}
CRJFileName& CRJFilePath::GetPath(CRJFileName& s) const {
	const auto p=m_clPath.parent_path();
	const auto& cs=p.native();
	s.SetData(cs.c_str(), cs.length());
	return s.CatSeparator();
}
CRJFileName& CRJFilePath::GetNameAndExt(CRJFileName& s) const {
	const auto p=m_clPath.filename();
	const auto& cs=p.native();
	s.SetData(cs.c_str(), cs.length());
	return s;
}
CRJFileName& CRJFilePath::GetName(CRJFileName& s) const {
	const auto p=m_clPath.stem();
	const auto& cs=p.native();
	s.SetData(cs.c_str(), cs.length());
	return s;
}
CRJFileName& CRJFilePath::GetExt(CRJFileName& s) const {
	const auto p=m_clPath.extension();
	const auto& cs=p.native();
	s.SetData(cs.c_str(), cs.length());
	return s;
}
CRJFileNameTemp::CRJFileNameTemp() {
	GetAppTemp().BuildPath();
}
CRJFileNameTemp::~CRJFileNameTemp() {
	RemoveSingle();
}

