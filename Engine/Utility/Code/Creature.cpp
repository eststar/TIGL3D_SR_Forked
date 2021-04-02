#include "Camera.h"
#include "Creature.h"

USING(Engine)

Engine::CCreature::CCreature(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_iHP(0), m_iMaxHP(0), m_iAtt(0), m_iDef(0)
	, m_bIsDead(false)
{
}

Engine::CCreature::~CCreature()
{
}

HRESULT Engine::CCreature::Ready_Object()
{
	return S_OK;
}

Engine::_int Engine::CCreature::Update_Object(const _float & fTimeDelta)
{
	return _int();
}

void Engine::CCreature::LateUpdate_Object(const _float & fTimeDelta)
{
}

void Engine::CCreature::Free()
{
	CGameObject::Free();
}
