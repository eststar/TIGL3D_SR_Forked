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
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,  // ���ؽ� ������ ������(������ ũ�� * ������ ����)
												0,						    // ���� ��� ���, ���� 0�� ���� ���� ���(D3DUSAGE_DYNAMIC �������� : ��ƼŬ ���� �� ���)
												m_dwFVF,				    // ������ �Ӽ� �� ����
												D3DPOOL_MANAGED,		    // ���� ��� �� �����ϴ� �޸� Ǯ ���	
												&m_pVB,						// ������� �����ϱ� ���� �� ��ü
												NULL)))						// ���� ���� �� �ʿ��� �������(���� null�� ����)
		return E_FAIL;


	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,   // �ε��� ������ ������(�ε����� ũ�� * �ﰢ���� ����)
												0,						    // ���� ��� ���, ���� 0�� ���� ���� ���(D3DUSAGE_DYNAMIC �������� : ��ƼŬ ���� �� ���)
												m_dwIdxFmt,				    // �ε����� �Ӽ� �� ����
												D3DPOOL_MANAGED,		    // ���� ��� �� �����ϴ� �޸� Ǯ ���	
												&m_pIB,						// ������� �����ϱ� ���� �� ��ü
												NULL)))						// ���� ���� �� �ʿ��� �������(���� null�� ����)
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

