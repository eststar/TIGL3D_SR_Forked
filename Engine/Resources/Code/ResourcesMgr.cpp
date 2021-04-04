#include "ResourcesMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CResourcesMgr)
Engine::CResourcesMgr::CResourcesMgr()
	: m_pmapResources(nullptr)
	, m_dwContainerSize(0)
{

}

Engine::CResourcesMgr::~CResourcesMgr()
{
	Free();
}

Engine::CResources* CResourcesMgr::Clone(const _ushort& wContainerIdx, const _tchar* pResourceTag)
{
	if (nullptr == m_pmapResources)
		return nullptr;

	CResources*	pPrototype = Find_Resources(wContainerIdx, pResourceTag);
	NULL_CHECK_RETURN(pPrototype, nullptr);

	return pPrototype->Clone();
}

HRESULT Engine::CResourcesMgr::Reserve_ContainerSize(const _ushort& wSize)
{
	if (nullptr != m_pmapResources)
		return E_FAIL;
	
	m_dwContainerSize = wSize;
	
	m_pmapResources = new MAPRESOURCES[wSize];

	return S_OK;
}

HRESULT Engine::CResourcesMgr::Ready_Buffers(LPDIRECT3DDEVICE9& pGraphicDev, const _ushort& wContainerIdx, const _tchar* pBufferTag, BUFFERID eID, const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv)
{
	if (nullptr == m_pmapResources)
	{
		MSG_BOX("Resourcesmgr reserve Failed");
		return E_FAIL;
	}

	CResources*	 pResources = Find_Resources(wContainerIdx, pBufferTag);
	if (nullptr != pResources)
		return E_FAIL;

	switch (eID)
	{
	case BUFFER_TRICOL:
		pResources = CTriCol::Create(pGraphicDev);
		break;

	case BUFFER_RCCOL:
		pResources = CRcCol::Create(pGraphicDev);
		break;

	case BUFFER_RCTEX:
		pResources = CRcTex::Create(pGraphicDev);
		break;

	case BUFFER_TERRAINTEX:
		pResources = CTerrainTex::Create(pGraphicDev, dwCntX, dwCntZ, dwVtxItv);
		break;

	case BUFFER_HTERRAINTEX:
		pResources = CHTerrainTex::Create(pGraphicDev, dwCntX, dwCntZ, dwVtxItv);
		break;

	case BUFFER_CUBETEX:
		pResources = CCubeTex::Create(pGraphicDev);
		break;

	case BUFFER_TETRATEX:
		pResources = CTetraTex::Create(pGraphicDev);
		break;
	}

	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pmapResources[wContainerIdx].emplace(pBufferTag, pResources);

	return S_OK;
}

void Engine::CResourcesMgr::Render_Buffers(const _ushort& wContainerIdx, const _tchar* pBufferTag)
{
	CResources*	 pResources = Find_Resources(wContainerIdx, pBufferTag);
	if (nullptr == pResources)
		return;

	dynamic_cast<CVIBuffer*>(pResources)->Render_Buffers();	
}

HRESULT CResourcesMgr::Ready_Textures(LPDIRECT3DDEVICE9& pGraphicDev, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTUREID eID, const _tchar* pPath, const _uint& iCnt /*= 1*/)
{
	if (nullptr == m_pmapResources)
	{
		MSG_BOX("Resourcesmgr reserve Failed");
		return E_FAIL;
	}

	CResources*		pResources = Find_Resources(wContainerIdx, pTextureTag);

	if (nullptr != pResources)
		return E_FAIL;

	pResources = CTexture::Create(pGraphicDev, pPath, eID, iCnt);
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pmapResources[wContainerIdx].emplace(pTextureTag, pResources);

	return S_OK;
}

Engine::CResources* Engine::CResourcesMgr::Find_Resources(const _ushort& wContainerIdx, const _tchar* pResourceTag)
{
	MAPRESOURCES::iterator		iter = find_if(m_pmapResources[wContainerIdx].begin(),
												m_pmapResources[wContainerIdx].end(), 
												CTagFinder(pResourceTag));

	if (iter == m_pmapResources[wContainerIdx].end())
		return nullptr;

	return iter->second;
}

void Engine::CResourcesMgr::Free()
{
	for (_ulong i = 0; i < m_dwContainerSize; ++i)
	{
		for_each(m_pmapResources[i].begin(), m_pmapResources[i].end(), CDeleteMap());
		m_pmapResources[i].clear();
	}

	Safe_Delete_Array(m_pmapResources);
}

