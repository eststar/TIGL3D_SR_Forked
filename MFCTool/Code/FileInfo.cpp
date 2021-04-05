#include "stdafx.h"
#include "FileInfo.h"
#include "Export_Function.h"

USING(Engine)

CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	_tchar szRelativePath[MAX_PATH] = L"";
	_tchar szCurrentPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurrentPath);//기본절대경로

	PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY, strFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);
	//얻어온 절대경로와 기본경로 받아서 상대경로로.
	return CString(szRelativePath);
}

void CFileInfo::PathExtraction(const wstring & wstrFilePath, list<Engine::PATHINFO*>& listPathInfo)
{
	CFileFind find;
	wstring wstrNextPath = wstrFilePath + L"\\*.*";
	BOOL bContinue = find.FindFile(wstrNextPath.c_str());
	while (bContinue)
	{
		bContinue = find.FindNextFile();
		if (find.IsDots()) 
			continue;//. 이나 .. 이면 컨티뉴
		else if (find.IsDirectory())
			PathExtraction(find.GetFilePath().GetString(), listPathInfo); //폴더면 다시
		else //폴더 아니고 첫번째 파일 경로 및 파일이름인 경우
		{
			if (find.IsSystem())
				continue;
			Engine::PATHINFO* pPathInfo = new Engine::PATHINFO;
			pPathInfo->iCount = 0;
			pPathInfo->pPath = L"";
			pPathInfo->pObjectKey = L"";
			pPathInfo->pStateKey = L"";

			TCHAR szPathBuf[MAX_PATH] = L"";
			lstrcpy(szPathBuf, find.GetFilePath().GetString()); //현재 경로 문자열.
			PathRemoveFileSpec(szPathBuf);

			pPathInfo->iCount = CFileInfo::FileCount(szPathBuf);
			wstring wstrTextureName = find.GetFileTitle(); 
			wstring wstrTextureExtension = find.GetFileName();
			wstrTextureExtension=PathFindExtension(wstrTextureExtension.c_str());

			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.length() - 1) + L"%d"+ wstrTextureExtension/*.png"*/; //숫자빼고 문자만.
			PathCombine(szPathBuf, szPathBuf, wstrTextureName.c_str());

			pPathInfo->pPath = CFileInfo::ConvertRelativePath(szPathBuf); //상대경로로.
			PathRemoveFileSpec(szPathBuf);
			pPathInfo->pStateKey = PathFindFileName(szPathBuf); // StateKey 
			PathRemoveFileSpec(szPathBuf);
			pPathInfo->pObjectKey = PathFindFileName(szPathBuf);

			listPathInfo.emplace_back(pPathInfo);
			bContinue = FALSE; //탈출조건
		}
	}
}

int CFileInfo::FileCount(const wstring & wstrFilePath) //같은 종류의 파일 몇개?
{
	wstring wstrFileFind = wstrFilePath + L"\\*.*";

	CFileFind find;
	BOOL bContinue = find.FindFile(wstrFileFind.c_str());

	int iCnt = 0;
	while (bContinue)
	{
		bContinue = find.FindNextFile();

		if (find.IsDots() || find.IsDirectory())
			continue;
		++iCnt;
	}
	return iCnt;
}