#include "GameObject.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pObjTag(nullptr)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::CGameObject(const CGameObject& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pObjTag(rhs.m_pObjTag)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::~CGameObject()
{

}

Engine::CComponent* CGameObject::Get_Component(const _tchar* pComponentTag, COMPONENTID eType)
{
	CComponent*		pComponent = Find_Component(pComponentTag, eType);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

HRESULT Engine::CGameObject::Ready_Object()
{
	return S_OK;
}

Engine::_int Engine::CGameObject::Update_Object(const _float& fTimeDelta)
{
	_int iExit = 0;

	for (auto& iter : m_mapComponent[COM_DYNAMIC])
		iExit = iter.second->Update_Component(fTimeDelta);

	return iExit;
}

void CGameObject::LateUpdate_Object(const _float & fTimeDelta)
{
	for (auto& iter : m_mapComponent[COM_DYNAMIC])
		iter.second->LateUpdate_Component(fTimeDelta);
}

void Engine::CGameObject::Render_Object()
{

}

Engine::CComponent* CGameObject::Find_Component(const _tchar* pComponentTag, COMPONENTID eType)
{
	auto	iter = find_if(m_mapComponent[eType].begin(), m_mapComponent[eType].end(), CTagFinder(pComponentTag));

	if (iter == m_mapComponent[eType].end())
		return nullptr;

	return iter->second;
}

void Engine::CGameObject::Free()
{
	Safe_Release(m_pGraphicDev);

	for (_uint i = 0; i < COM_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}
}

