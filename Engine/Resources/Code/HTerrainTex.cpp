#include "HTerrainTex.h"

USING(Engine)
Engine::CHTerrainTex::CHTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_hFile(nullptr)	
	, m_pPos(nullptr)
	, m_bClone(false)
{

}

Engine::CHTerrainTex::CHTerrainTex(const CHTerrainTex& rhs)
	: CVIBuffer(rhs)
	, m_hFile(rhs.m_hFile)
	, m_fH(rhs.m_fH)
	, m_iH(rhs.m_iH)
	, m_pPos(rhs.m_pPos)
	, m_bClone(true)
{

}

Engine::CHTerrainTex::~CHTerrainTex()
{

}

HRESULT Engine::CHTerrainTex::Ready_Buffers(const _ulong& dwCntX, const _ulong& dwCntZ,
	const _ulong& dwVtxItv)
{
	m_dwVtxSize		= sizeof(VTXTEX);
	m_dwVtxCnt		= dwCntX * dwCntZ;
	m_dwFVF			= FVF_TEX;
	m_dwTriCnt		= (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwIdxFmt		= D3DFMT_INDEX32;
	m_dwIdxSize		= sizeof(INDEX32);

	m_pPos = new _vec3[m_dwVtxCnt];

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffers(), E_FAIL);

	// 높이맵 파일로 부터 높이값을 추출
	m_hFile = CreateFile(L"../../Resource/Test/Texture/Terrain/Height.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	_ulong	dwByte = 0;

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	_ulong*		pPixel = new _ulong[m_iH.biHeight * m_iH.biWidth];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biHeight * m_iH.biWidth, &dwByte, NULL);
	
	CloseHandle(m_hFile);

	
	VTXTEX*		pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);
	
	_ulong	dwIndex = 0;

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVtxTex[dwIndex].vPosition = _vec3(_float(j * dwVtxItv),
												(pPixel[dwIndex] & 0x000000ff) / 20.f, 
												_float(i * dwVtxItv));

			m_pPos[dwIndex] = pVtxTex[dwIndex].vPosition;

			pVtxTex[dwIndex].vTexUV = _vec2((_float(j) / (dwCntX - 1)) * 20.f, (_float(i) / (dwCntZ - 1) )* 20.f);
		}
	}
	
	m_pVB->Unlock();

	Safe_Delete_Array(pPixel);


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

void Engine::CHTerrainTex::Render_Buffers()
{
	CVIBuffer::Render_Buffers();
}


Engine::CHTerrainTex* Engine::CHTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv/* = 1 */)
{
	CHTerrainTex*		pInstance = new CHTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffers(dwCntX, dwCntZ, dwVtxItv )))
	{
		MSG_BOX("TerrainTex Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;

}	

Engine::CResources* CHTerrainTex::Clone()
{
	return new CHTerrainTex(*this);
}

void Engine::CHTerrainTex::Free()
{
	if (false == m_bClone)
	{
		Safe_Delete_Array(m_pPos);
	}

	CVIBuffer::Free();
}

