#include "TetraTex.h"


USING(Engine)
CTetraTex::CTetraTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CTetraTex::CTetraTex(const CTetraTex & rhs)
	: CVIBuffer(rhs)
{
}


CTetraTex::~CTetraTex()
{
}

HRESULT CTetraTex::Ready_Buffer()
{
	m_dwFVF = FVF_COL;
	m_dwTriCnt = 6;
	m_dwVtxCnt = 5;
	m_dwVtxSize = sizeof(VTXTETRA);
	
	m_dwIdxSize = sizeof(INDEX16);
	m_dwIdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffers(), E_FAIL);

	VTXTETRA* pVerTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVerTex, NULL);

	pVerTex[0].vPosition = _vec3(1, -1, 1);
	//pVerTex[0].vTexUV = pVerTex[0].vPoxsition;
	pVerTex[0].dwColor = D3DXCOLOR(0.f, 1.f, 1.f, 1.f);

	pVerTex[1].vPosition = _vec3(1, -1, -1);
	//pVerTex[1].vTexUV = pVerTex[1].vPosition;
	pVerTex[1].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVerTex[2].vPosition = _vec3(-1, -1, 1);
	//pVerTex[2].vTexUV = pVerTex[2].vPosition;
	pVerTex[2].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVerTex[3].vPosition = _vec3(-1, -1, -1);
	//pVerTex[3].vTexUV = pVerTex[3].vPosition;
	pVerTex[3].dwColor = D3DXCOLOR(0.f, 1.f, 1.f, 1.f);

	pVerTex[4].vPosition = _vec3(0, 1, 0);
	//pVerTex[4].vTexUV = pVerTex[4].vPosition;
	pVerTex[4].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 2;
	pIndex[0]._2 = 1;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 1;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 1;
	pIndex[3]._2 = 3;

	pIndex[4]._0 = 4;
	pIndex[4]._1 = 3;
	pIndex[4]._2 = 2;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 2;
	pIndex[5]._2 = 0;

	m_pIB->Unlock();
	return S_OK;
}

CTetraTex * CTetraTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTetraTex*	pInstance = new CTetraTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Safe_Release(pInstance);

	return pInstance;
}

CResources * CTetraTex::Clone()
{
	return new CTetraTex(*this);
}

void CTetraTex::Free()
{
	CVIBuffer::Free();
}
