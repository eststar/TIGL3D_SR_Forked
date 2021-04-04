#include "Renderer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

Engine::CRenderer::CRenderer()
	: m_pGraphicDev(nullptr)
{
}

Engine::CRenderer::~CRenderer()
{
	Free();
}

void Engine::CRenderer::Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject)
{
	if (RENDER_END <= eGroup)
		return;

	m_RenderGroup[eGroup].push_back(pGameObject);
	pGameObject->AddRef();
}

void Engine::CRenderer::Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Render_Priority(pGraphicDev);
	Render_Nonalpha(pGraphicDev);
	Render_Alpha(pGraphicDev);
	Render_UI(pGraphicDev);
}

void Engine::CRenderer::Clear_RenderGroup()
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void Engine::CRenderer::Free()
{
	Clear_RenderGroup();
}


void CRenderer::Render_Priority(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
	{
		iter->Render_Object();
		Safe_Release(iter);
	}
	m_RenderGroup[RENDER_PRIORITY].clear();
}

void CRenderer::Render_Nonalpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
	{
		iter->Render_Object();
		Safe_Release(iter);
	}
	m_RenderGroup[RENDER_NONALPHA].clear();
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	//���ĺ��� ON
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//�����׽�Ʈ ON
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
	{
		iter->Render_Object();
		Safe_Release(iter);
	}
	m_RenderGroup[RENDER_ALPHA].clear();


	//�����׽�Ʈ OFF
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//���ĺ��� OFF
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	_matrix matView, matProj, matDefault;
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixIdentity(&matDefault);
	//���ٰ��� ���ֱ� ���� �佺���̽� ��ȯ�̶� ������ȯ�� �׵���·� ����
	pGraphicDev->SetTransform(D3DTS_VIEW, &matDefault);

	//���� �׵��̸� �ȵǰ� ��Ⱦ��� ���ؼ� ������ ��
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matDefault);

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	for (auto& iter : m_RenderGroup[RENDER_UI])
	{
		iter->Render_Object();
		Safe_Release(iter);
	}
	m_RenderGroup[RENDER_UI].clear();

	//�ٽ� ������ ��ķ� ��������
	pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	//�����׽�Ʈ OFF
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//���ĺ��� OFF
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}