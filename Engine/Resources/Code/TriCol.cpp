#include "TriCol.h"

USING(Engine)
Engine::CTriCol::CTriCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTriCol::CTriCol(const CTriCol& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CTriCol::~CTriCol()
{

}

HRESULT Engine::CTriCol::Ready_Buffers()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt	= 3;
	m_dwFVF		= FVF_COL;
	m_dwTriCnt	= 1;

	m_dwIdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffers(), E_FAIL);

	VTXCOL*		pVtxCol = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);

	pVtxCol[0].vPosition	= _vec3(0.f, 1.f, 0.f);
	pVtxCol[0].dwColor		= D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	
	pVtxCol[1].vPosition = _vec3(1.f, -1.f, 0.f);
	pVtxCol[1].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	
	pVtxCol[2].vPosition = _vec3(-1.f, -1.f, 0.f);
	pVtxCol[2].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	m_pIB->Unlock();

	return S_OK;
}

void Engine::CTriCol::Render_Buffers()
{
	CVIBuffer::Render_Buffers();
}


Engine::CTriCol* Engine::CTriCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTriCol*		pInstance = new CTriCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffers()))
	{
		MSG_BOX("TriCol Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;

}	

Engine::CResources* CTriCol::Clone()
{
	return new CTriCol(*this);
}

void Engine::CTriCol::Free()
{
	CVIBuffer::Free();
}

