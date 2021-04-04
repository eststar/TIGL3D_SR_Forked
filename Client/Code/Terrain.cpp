#include "stdafx.h"
#include "Terrain.h"

#include "Export_Function.h"

USING(Engine)

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTerrain::~CTerrain()
{

}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CTerrain::Load_Terrain(LPDIRECT3DDEVICE9& pGraphicDev, const _tchar * pPath, vector<Engine::TERRAININFO*>& rVecTerrainInfo, vector<CGameObject*>& rVecTerrain)
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
	for (auto& pInfo : rVecTerrainInfo)
		Safe_Delete(pInfo);
	rVecTerrainInfo.clear();
	rVecTerrainInfo.shrink_to_fit();
	//�ε��� Info���� �߰�
	rVecTerrainInfo = vecTempTile;

	//���� terrain �����
	if (!rVecTerrain.empty())
	{
		for (_uint i = 0; i < rVecTerrain.size(); i++)
			Safe_Release(rVecTerrain[i]);
		rVecTerrain.clear();
		rVecTerrain.shrink_to_fit();
	}
	rVecTerrain.reserve(rVecTerrainInfo.size());


	//�ε��� INFO�� ���� CTerrain ������ ���Ϳ� ����
	for (_uint i = 0; i < rVecTerrainInfo.size(); i++)
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

		pTransform->Set_Pos(&(rVecTerrainInfo[i]->vPos));
		pTransform->Set_Scale(&(rVecTerrainInfo[i]->vSize));
		pTransform->Set_Angle(&(rVecTerrainInfo[i]->vRotation));

		//CTerrain�� ��� ������ ������Ʈ���� ���⼭ ������Ʈ �ص� object�� ���� �������̳ʿ� ���� ������Ʈ���� �ȹٲ�.
		//�׷��� Cterrain�� ������Ʈ�� ������ ������Ʈ�� ����� �������̳ʿ� �ٽ� ����
		pTerrain->Set_Component(COM_DYNAMIC, L"Com_Transform", pTransform);

		//terraintex
		_tchar szBuf[MAX_PATH] = L"Buffer_TerrainTex";
		_tchar szVtxCNX[MAX_PATH] = L"";
		_tchar szVtxCNZ[MAX_PATH] = L"";
		_tchar szVtxItv[MAX_PATH] = L"";

		_itow_s(_int(rVecTerrainInfo[i]->dwVtxCNX), szVtxCNX, 10);
		_itow_s(_int(rVecTerrainInfo[i]->dwVtxCNZ), szVtxCNX, 10);
		_itow_s(_int(rVecTerrainInfo[i]->dwVtxItv), szVtxItv, 10);

		lstrcat(szBuf, L"_");
		lstrcat(szBuf, szVtxCNX);
		lstrcat(szBuf, L"_");
		lstrcat(szBuf, szVtxCNZ);
		lstrcat(szBuf, L"_");
		lstrcat(szBuf, szVtxItv);

		FAILED_CHECK_RETURN(Engine::ADD_Buffer(pGraphicDev, RESOURCE_STATIC, szBuf, Engine::BUFFER_TERRAINTEX
			, rVecTerrainInfo[i]->dwVtxCNX + 1, rVecTerrainInfo[i]->dwVtxCNZ + 1, rVecTerrainInfo[i]->dwVtxItv), E_FAIL);

		//���ҽ� ���� ����
		CResources* pResource = Engine::Clone(RESOURCE_STATIC, szBuf);
		NULL_CHECK_RETURN(pResource, E_FAIL);

		pTerrain->Set_Resource(COM_STATIC, L"Com_Buffer", pResource);

		rVecTerrain.push_back(pObject);
	}

	return S_OK;
}

void CTerrain::Free()
{
	CGameObject::Free();
}

HRESULT CTerrain::Ready_Object()
{
	m_pObjTag = L"Terrain";

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

Engine::_int CTerrain::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(RENDER_PRIORITY, this);

	return OBJ_NOEVENT;
}

void CTerrain::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);
}

void CTerrain::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffers();		

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CTerrain::Set_Component(COMPONENTID eCOMID, const _tchar* strCOMTag, CComponent* pCom)
{
	auto iter = find_if(m_mapComponent[eCOMID].begin(), m_mapComponent[eCOMID].end(), CTagFinder(strCOMTag));

	if (iter == m_mapComponent[eCOMID].end())
		return;
	Safe_Release(iter->second);
	m_mapComponent[eCOMID].erase(iter);

	if (strCOMTag == L"Com_Transform")
	{
		Safe_Release(m_pTransformCom);
		m_pTransformCom = dynamic_cast<CTransform*>(pCom);
		m_mapComponent[eCOMID].emplace(strCOMTag, pCom);
	}
}

void CTerrain::Set_Resource(COMPONENTID eCOMID, const _tchar* strCOMTag, CResources* pResource)
{
	//������ �̸��� Ŭ�� ���۰� ����ִ°�?
	auto iter = find_if(m_mapComponent[eCOMID].begin(), m_mapComponent[eCOMID].end(), CTagFinder(strCOMTag));
	//������ ������ �����
	if (iter != m_mapComponent[eCOMID].end())
	{
		Safe_Release(iter->second);
		m_mapComponent[eCOMID].erase(iter);
	}

	CComponent* pCOM = nullptr;

	//Ŭ���� ���ҽ��� �� Ŭ������ ��ĳ, ��ĳ�Ѱ� ������Ʈ �����Ϳ� �ֱ�
	if (strCOMTag == L"Com_Texture")
	{
		Safe_Release(m_pTextureCom);
		m_pTextureCom = dynamic_cast<CTexture*>(pResource);
		pCOM = m_pTextureCom;
	}
	else if (strCOMTag == L"Com_Buffer")
	{
		Safe_Release(m_pBufferCom);
		m_pBufferCom = dynamic_cast<CTerrainTex*>(pResource);
		pCOM = m_pBufferCom;
	}
	NULL_CHECK(pCOM);
	m_mapComponent[eCOMID].emplace(strCOMTag, pCOM);
}


HRESULT CTerrain::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_TerrainTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	//pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_TerrainY"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}
