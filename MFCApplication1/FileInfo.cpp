#include "pch.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(const CString & wstrFileFullPath)
{
	TCHAR szCurDir[MAX_PATH]{}; 
	GetCurrentDirectory(MAX_PATH, szCurDir); 
	TCHAR szRelativePath[MAX_PATH]{}; 
	// 같은 드라이브에서는 상관 없음. 다만 다른 드라이브면 안됨. 
	PathRelativePathTo(szRelativePath, szCurDir, FILE_ATTRIBUTE_DIRECTORY, wstrFileFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);
	return CString(szRelativePath);
}
