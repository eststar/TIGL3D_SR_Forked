#include "Component.h"

USING(Engine)

Engine::CComponent::CComponent()
{

}

Engine::CComponent::CComponent(const CComponent& rhs)
{

}

Engine::CComponent::~CComponent()
{
}

Engine::_int CComponent::Update_Component(const _float& fTimeDelta)
{
	return 0;
}

void Engine::CComponent::LateUpdate_Component(const _float & fTimeDelta)
{
}
