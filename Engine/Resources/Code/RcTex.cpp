#include "RcTex.h"

USING(Engine)
Engine::CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CRcTex::CRcTex(const CRcTex& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CRcTex::~CRcTex()
{

}

HRESULT Engine::CRcTex::Ready_Buffers()
{
	m_dwVtxSize		= sizeof(VTXTEX);
	m_dwVtxCnt		= 4;
	m_dwFVF			= FVF_TEX;
	m_dwTriCnt		= 2;
	m_dwIdxFmt		= D3DFMT_INDEX16;
	m_dwIdxSize		= sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffers(), E_FAIL);

	VTXTEX*		pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);
		
	pVtxTex[0].vPosition	= _vec3(-1.f, 1.f, 0.f);
	pVtxTex[0].vTexUV		= _vec2(0.f, 0.f);

	pVtxTex[1].vPosition	= _vec3(1.f, 1.f, 0.f);
	pVtxTex[1].vTexUV		= _vec2(1.f, 0.f);

	pVtxTex[2].vPosition	= _vec3(1.f, -1.f, 0.f);
	pVtxTex[2].vTexUV		= _vec2(1.f, 1.f);

	pVtxTex[3].vPosition	= _vec3(-1.f, -1.f, 0.f);
	pVtxTex[3].vTexUV		= _vec2(0.f, 1.f);

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

void Engine::CRcTex::Render_Buffers()
{
	CVIBuffer::Render_Buffers();
}


Engine::CRcTex* Engine::CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTex*		pInstance = new CRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffers()))
	{
		MSG_BOX("RcTex Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}	

Engine::CResources* CRcTex::Clone()
{
	return new CRcTex(*this);
}

void Engine::CRcTex::Free()
{
	CVIBuffer::Free();
}

