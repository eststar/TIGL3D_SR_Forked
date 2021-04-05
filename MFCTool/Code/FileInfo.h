#ifndef FileInfo_h__
#define FileInfo_h__

#include "Engine_Define.h"
#include "Define.h"

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();
public:
	static CString ConvertRelativePath(CString strFullPath);
	static void PathExtraction(const wstring& wstrFilePath, list<Engine::PATHINFO*>& listPathInfo);
	static int	FileCount(const wstring& wstrFilePath);

};

#endif // FileInfo_h__