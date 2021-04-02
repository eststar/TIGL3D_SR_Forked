#include "VIBuffer.h"

USING(Engine)

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CResources(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwFVF(rhs.m_dwFVF)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwIdxSize(rhs.m_dwIdxSize)	
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

Engine::CVIBuffer::~CVIBuffer()
{

}

HRESULT CVIBuffer::Ready_Buffers()
{
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,  // 버텍스 버퍼의 사이즈(정점의 크기 * 정점의 개수)
												0,						    // 정점 사용 방식, 숫자 0인 정적 버퍼 사용(D3DUSAGE_DYNAMIC 동적버퍼 : 파티클 생성 시 사용)
												m_dwFVF,				    // 정점의 속성 값 지정
												D3DPOOL_MANAGED,		    // 정적 사용 시 선택하는 메모리 풀 방식	
												&m_pVB,						// 결과물을 저장하기 위한 컴 객체
												NULL)))						// 버퍼 만들 떄 필요한 예약상태(거의 null로 지정)
		return E_FAIL;


	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,   // 인덱스 버퍼의 사이즈(인덱스의 크기 * 삼각형의 개수)
												0,						    // 정점 사용 방식, 숫자 0인 정적 버퍼 사용(D3DUSAGE_DYNAMIC 동적버퍼 : 파티클 생성 시 사용)
												m_dwIdxFmt,				    // 인덱스의 속성 값 지정
												D3DPOOL_MANAGED,		    // 정적 사용 시 선택하는 메모리 풀 방식	
												&m_pIB,						// 결과물을 저장하기 위한 컴 객체
												NULL)))						// 버퍼 만들 떄 필요한 예약상태(거의 null로 지정)
												return E_FAIL;


	return S_OK;
}

void CVIBuffer::Render_Buffers()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	m_pGraphicDev->SetFVF(m_dwFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	
}

void Engine::CVIBuffer::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CResources::Free();
}

