#pragma once
#include <recurj/rjframework/libs/base/strings/native.h>
#include <recurj/rjframework/modules/modbase.h>
#include <filesystem>
namespace fs=std::filesystem;
namespace rjBase {
	class CRJStringDTime;
	class RJBASE_API CRJFileName:public CRJStringExpanded {
	public:
		CRJFileName();
		CRJFileName(const fs::path&);
		CRJFileName(const CRJStringUnicode&);
		CRJFileName(const CRJStringByte&);
		CRJFileName(const CRJFileName&);
		CRJFileName(const wchar_t*);
		CRJFileName(const char*);
		virtual~CRJFileName();
		CRJFileName& CatSeparator();
		CRJFileName& BuildPathAbsolute(const CRJFileName&);
		CRJFileName& BuildPath();
		CRJFileName& BuildPath(const CRJStringUnicode&);
		CRJFileName& BuildPath(const CRJStringByte&);
		CRJFileName& GetAppExec();
		CRJFileName& GetAppTemp();
		CRJFileName& GetPath(CRJFileName&) const;
		CRJFileName& GetNameAndExt(CRJFileName&) const;
		CRJFileName& GetName(CRJFileName&) const;
		CRJFileName& GetExt(CRJFileName&) const;
		CRJFileName& RelativeMe(CRJFileName&) const;
		CRJFileName& Unify();
		bool HasExtentionLowCase(const CRJString&);
		bool IsPathAbsolute() const;
		bool IsFileExist() const;
		bool IsDirExist() const;

		bool GetFileSize(size_t&) const;
		time_t GetLastUpdated() const;
		bool GetLastUpdated(CRJStringDTime&);
		bool CreateFilePath() const;
		bool CreateDir() const;
		bool Copy(const CRJFileName&, fs::copy_options) const;
		bool Move(const CRJFileName&) const;
		;
		bool RenameMe(const CRJFileName&) const;
		bool RenameToMe(const CRJFileName&) const;
		bool RenameToMe(const fs::path&) const;
		bool RemoveSingle() const;
		bool RemoveAll() const;
		bool Backup() const;
		bool SetSameTime(const CRJFileName&);
		CRJFileName& SetPath(const fs::path&);
	};
	class RJBASE_API CRJFileNameWithLastUpdate:public CRJFileName {
	public:
		CRJFileNameWithLastUpdate();
		virtual~CRJFileNameWithLastUpdate();
		bool NeedReload(bool,const CRJFileName&);
	protected:
		time_t m_dtUpdate;
	};
	class RJBASE_API CRJFilePath {
	public:
		CRJFilePath();
		CRJFilePath(const CRJFileName&);
		virtual~CRJFilePath();
		operator fs::path&() {
			return m_clPath;
		}
		CRJFilePath& Set(const CRJFileName&);
		CRJFilePath& Unify();
		size_t GetPathLength() const;
		size_t GetNameLength() const;
		size_t GetExtLength() const;
		CRJFileName& GetPath(CRJFileName&) const;
		CRJFileName& GetNameAndExt(CRJFileName&) const;
		CRJFileName& GetName(CRJFileName&) const;
		CRJFileName& GetExt(CRJFileName&) const;
		CRJFileName& RelativeMe(CRJFileName&) const;
		static bool SetAppWorking(CRJFileName&);
		static CRJFileName& GetAppWorking(CRJFileName&);
	private:
		fs::path m_clPath;
	};
	class RJBASE_API CRJFileNameTemp:public CRJFileName {
	public:
		CRJFileNameTemp();
		virtual~CRJFileNameTemp();
	};
}

