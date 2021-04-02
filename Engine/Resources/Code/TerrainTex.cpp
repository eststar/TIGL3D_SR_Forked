#include "TerrainTex.h"

USING(Engine)
Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_pPos(nullptr)
	, m_bClone(false)
{

}

Engine::CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
	: CVIBuffer(rhs)
	, m_pPos(rhs.m_pPos)
	, m_bClone(true)
{

}

Engine::CTerrainTex::~CTerrainTex()
{

}

HRESULT Engine::CTerrainTex::Ready_Buffers(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	m_dwVtxSize		= sizeof(VTXTEX);
	m_dwVtxCnt		= dwCntX * dwCntZ;
	m_dwFVF			= FVF_TEX;
	m_dwTriCnt		= (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwIdxFmt		= D3DFMT_INDEX32;
	m_dwIdxSize		= sizeof(INDEX32);

	m_pPos = new _vec3[m_dwVtxCnt];

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffers(), E_FAIL);

	
	VTXTEX*		pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);
	
	_ulong	dwIndex = 0;

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVtxTex[dwIndex].vPosition = _vec3(_float(j * dwVtxItv), 0.f, _float(i * dwVtxItv));

			m_pPos[dwIndex] = pVtxTex[dwIndex].vPosition;

			pVtxTex[dwIndex].vTexUV = _vec2((_float(j) / (dwCntX - 1)) * 20.f, (_float(i) / (dwCntZ - 1) )* 20.f);
		}
	}
	
	m_pVB->Unlock();


	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong dwTriCnt = 0;

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// 오른쪽 위
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;
			++dwTriCnt;

			// 왼쪽 아래
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;
			++dwTriCnt;
		}
	}
		
	m_pIB->Unlock();

	return S_OK;
}

void Engine::CTerrainTex::Render_Buffers()
{
	CVIBuffer::Render_Buffers();
}


Engine::CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv/* = 1 */)
{
	CTerrainTex*		pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffers(dwCntX, dwCntZ, dwVtxItv)))
	{
		MSG_BOX("TerrainTex Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;

}	

Engine::CResources* CTerrainTex::Clone()
{
	return new CTerrainTex(*this);
}

void Engine::CTerrainTex::Free()
{
	if (false == m_bClone)
	{
		Safe_Delete_Array(m_pPos);
	}

	CVIBuffer::Free();
}

