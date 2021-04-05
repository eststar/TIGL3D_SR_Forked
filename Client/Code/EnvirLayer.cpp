#include "stdafx.h"
#include "EnvirLayer.h"

#include "Export_Function.h"
#include "Terrain.h"
//#include "GameObject.h"
#include "CollisionMgr.h"

USING(Engine)

CEnvirLayer::CEnvirLayer()
{
}

CEnvirLayer::~CEnvirLayer()
{
}

HRESULT CEnvirLayer::Ready_Layer(_int iVecSize)
{
	CLayer::Ready_Layer(iVecSize);

	return S_OK;
}

_int CEnvirLayer::Update_Layer(const _float & fTimeDelta)
{
	CLayer::Update_Layer(fTimeDelta);

	return 0;
}

void CEnvirLayer::LateUpdate_Layer(const _float & fTimeDelta)
{
	CLayer::LateUpdate_Layer(fTimeDelta);
}

HRESULT CEnvirLayer::Load_Terrain(LPDIRECT3DDEVICE9& pGraphicDev, const _tchar * pPath)
{
	HANDLE hFile = CreateFile(pPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	_ulong dwByte;
	TERRAININFO* pTempTile = nullptr;
	vector<TERRAININFO*> vecTempTile;

	//���� �б�
	while (true)
	{
		pTempTile = new TERRAININFO;
		ZeroMemory(pTempTile, sizeof(TERRAININFO));

		ReadFile(hFile, pTempTile, sizeof(TERRAININFO), &dwByte, nullptr);
		if (dwByte == 0)
		{
			Safe_Delete(pTempTile);
			break;
		}
		vecTempTile.emplace_back(pTempTile);
	}
	CloseHandle(hFile);
	if (vecTempTile.empty())
	{
		ERR_MSG(L"Load Failed");
		return E_FAIL;
	}

	//���� Info ���� ����
	for (auto& pInfo : m_VecTerrainInfo)
		Safe_Delete(pInfo);
	m_VecTerrainInfo.clear();
	m_VecTerrainInfo.shrink_to_fit();
	//�ε��� Info���� �߰�
	m_VecTerrainInfo = vecTempTile;

	//���� terrain �����
	if (!m_VecTerrain.empty())
	{
		for (_uint i = 0; i < m_VecTerrain.size(); i++)
			Safe_Release(m_VecTerrain[i]);
		m_VecTerrain.clear();
		m_VecTerrain.shrink_to_fit();
	}
	m_VecTerrain.reserve(m_VecTerrainInfo.size());


	//�ε��� INFO�� ���� CTerrain ������ ���Ϳ� ����
	for (_uint i = 0; i < m_VecTerrainInfo.size(); i++)
	{
		//���� ��ü ����
		CGameObject*		pObject = CTerrain::Create(pGraphicDev);
		NULL_CHECK_RETURN(pObject, E_FAIL);

		CTerrain*				pTerrain = dynamic_cast<CTerrain*>(pObject);

		// ���� COM���� ������ ����
		//Ʈ������

		Engine::CComponent*		pComponent = nullptr;
		CTransform* pTransform = Engine::CTransform::Create();
		pComponent = pTransform;
		NULL_CHECK_RETURN(pComponent, E_FAIL);

		pTransform->Set_Pos(&(m_VecTerrainInfo[i]->vPos));
		pTransform->Set_Scale(&(m_VecTerrainInfo[i]->vSize));
		pTransform->Set_Angle(&(m_VecTerrainInfo[i]->vRotation));

		//CTerrain�� ��� ������ ������Ʈ���� ���⼭ ������Ʈ �ص� object�� ���� �������̳ʿ� ���� ������Ʈ���� �ȹٲ�.
		//�׷��� Cterrain�� ������Ʈ�� ������ ������Ʈ�� ����� �������̳ʿ� �ٽ� ����
		pTerrain->Set_Component(COM_DYNAMIC, L"Com_Transform", pTransform);

		//terraintex
		_tchar szBuf[MAX_PATH] = L"Buffer_TerrainTex";
		_tchar szVtxCNX[MAX_PATH] = L"";
		_tchar szVtxCNZ[MAX_PATH] = L"";
		_tchar szVtxItv[MAX_PATH] = L"";

		_itow_s(_int(m_VecTerrainInfo[i]->dwVtxCNX), szVtxCNX, 10);
		_itow_s(_int(m_VecTerrainInfo[i]->dwVtxCNZ), szVtxCNX, 10);
		_itow_s(_int(m_VecTerrainInfo[i]->dwVtxItv), szVtxItv, 10);

		lstrcat(szBuf, L"_");
		lstrcat(szBuf, szVtxCNX);
		lstrcat(szBuf, L"_");
		lstrcat(szBuf, szVtxCNZ);
		lstrcat(szBuf, L"_");
		lstrcat(szBuf, szVtxItv);

		FAILED_CHECK_RETURN(Engine::ADD_Buffer(pGraphicDev, RESOURCE_STATIC, szBuf, Engine::BUFFER_TERRAINTEX
			, m_VecTerrainInfo[i]->dwVtxCNX + 1, m_VecTerrainInfo[i]->dwVtxCNZ + 1, m_VecTerrainInfo[i]->dwVtxItv), E_FAIL);

		//���ҽ� ���� ����
		CResources* pResource = Engine::Clone(RESOURCE_STATIC, szBuf);
		NULL_CHECK_RETURN(pResource, E_FAIL);

		pTerrain->Set_Resource(COM_STATIC, L"Com_Buffer", pResource);

		NULL_CHECK_RETURN(pObject, E_FAIL);

		m_VecTerrain.push_back(pObject);
		FAILED_CHECK_RETURN(Add_GameObject(ENVIR_TERRAIN, pObject), E_FAIL);

	}

	return S_OK;
}


CEnvirLayer * CEnvirLayer::Create(_int iVecSize)
{
	CEnvirLayer*		pInstance = new CEnvirLayer;

	if (FAILED(pInstance->Ready_Layer(iVecSize)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEnvirLayer::Free()
{
	CLayer::Free();

	for (_uint i = 0; i < m_VecTerrain.size(); i++)
	{
		Safe_Release(m_VecTerrain[i]);
	}
	m_VecTerrain.clear();
	m_VecTerrain.shrink_to_fit();

	for (_uint i = 0; i < m_VecTerrainInfo.size(); i++)
	{
		Safe_Delete(m_VecTerrainInfo[i]);
	}
	m_VecTerrainInfo.clear();
	m_VecTerrainInfo.shrink_to_fit();

}
