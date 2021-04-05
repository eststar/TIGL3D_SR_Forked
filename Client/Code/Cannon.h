#ifndef Cannon_h__
#define Cannon_h__

#include "Define.h"
#include "Creature.h"


BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CCalculator;

END

class CCannon final : public Engine::CCreature
{
private:
	explicit CCannon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCannon();

public:
	HRESULT Ready_Object()								override;
	_int Update_Object(const _float& fTimeDelta)		override;
	void LateUpdate_Object(const _float& fTimeDelta)	override;
	void Render_Object()								override;

private:
	HRESULT		Add_Component();
	void		SetUp_OnTerrain();
	void		BillBoardYaw();

private:
	HRESULT		Create_Bullet();

private:
	_float		m_fBulletCoolTime;

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTransform*		m_pPlayerTransformCom = nullptr;
	
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;

public:
	static CCannon*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void				Free() override;
};
#endif // !Cannon_h__
