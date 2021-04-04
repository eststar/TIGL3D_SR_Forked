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
	GetCurrentDirectory(MAX_PATH, szCurrentPath);//�⺻������

	PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY, strFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);
	//���� �����ο� �⺻��� �޾Ƽ� ����η�.
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
			continue;//. �̳� .. �̸� ��Ƽ��
		else if (find.IsDirectory())
			PathExtraction(find.GetFilePath().GetString(), listPathInfo); //������ �ٽ�
		else //���� �ƴϰ� ù��° ���� ��� �� �����̸��� ���
		{
			if (find.IsSystem())
				continue;
			Engine::PATHINFO* pPathInfo = new Engine::PATHINFO;
			pPathInfo->iCount = 0;
			pPathInfo->pPath = L"";
			pPathInfo->pObjectKey = L"";
			pPathInfo->pStateKey = L"";

			TCHAR szPathBuf[MAX_PATH] = L"";
			lstrcpy(szPathBuf, find.GetFilePath().GetString()); //���� ��� ���ڿ�.
			PathRemoveFileSpec(szPathBuf);

			pPathInfo->iCount = CFileInfo::FileCount(szPathBuf);
			wstring wstrTextureName = find.GetFileTitle(); 
			wstring wstrTextureExtension = find.GetFileName();
			wstrTextureExtension=PathFindExtension(wstrTextureExtension.c_str());

			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.length() - 1) + L"%d"+ wstrTextureExtension/*.png"*/; //���ڻ��� ���ڸ�.
			PathCombine(szPathBuf, szPathBuf, wstrTextureName.c_str());

			pPathInfo->pPath = CFileInfo::ConvertRelativePath(szPathBuf); //����η�.
			PathRemoveFileSpec(szPathBuf);
			pPathInfo->pStateKey = PathFindFileName(szPathBuf); // StateKey 
			PathRemoveFileSpec(szPathBuf);
			pPathInfo->pObjectKey = PathFindFileName(szPathBuf);

			listPathInfo.emplace_back(pPathInfo);
			bContinue = FALSE; //Ż������
		}
	}
}

int CFileInfo::FileCount(const wstring & wstrFilePath) //���� ������ ���� �?
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