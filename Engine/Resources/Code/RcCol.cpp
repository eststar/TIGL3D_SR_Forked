#include "RcCol.h"

USING(Engine)
Engine::CRcCol::CRcCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CRcCol::CRcCol(const CRcCol& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CRcCol::~CRcCol()
{

}

HRESULT Engine::CRcCol::Ready_Buffers()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 4;
	m_dwFVF = FVF_COL;
	m_dwTriCnt = 2;

	m_dwIdxFmt  = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffers(), E_FAIL);

	VTXCOL*		pVtxCol = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);
		
	pVtxCol[0].vPosition = _vec3(-1.f, 1.f, 0.f);
	pVtxCol[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVtxCol[1].vPosition = _vec3(1.f, 1.f, 0.f);
	pVtxCol[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVtxCol[2].vPosition = _vec3(1.f, -1.f, 0.f);
	pVtxCol[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVtxCol[3].vPosition = _vec3(-1.f, -1.f, 0.f);
	pVtxCol[3].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 위
	pIndex[0]._0 = 0; 
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// 왼쪽 아래
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;
	
	m_pIB->Unlock();

	return S_OK;
}

void Engine::CRcCol::Render_Buffers()
{
	CVIBuffer::Render_Buffers();
}


Engine::CRcCol* Engine::CRcCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcCol*		pInstance = new CRcCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffers()))
	{
		MSG_BOX("RcCol Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;

}	

Engine::CResources* CRcCol::Clone()
{
	return new CRcCol(*this);
}

void Engine::CRcCol::Free()
{
	CVIBuffer::Free();
}

