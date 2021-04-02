#ifndef Monster_h__
#define Monster_h__

#include "Define.h"
#include "Creature.h"
#include "MonsterBullet.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CCalculator;
class CLayer;

END
class CMonster : public Engine::CCreature
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster();

public:
	HRESULT Ready_Object()								override;
	_int Update_Object(const _float& fTimeDelta)		override;
	void LateUpdate_Object(const _float& fTimeDelta)	override;
	void Render_Object()								override;

private:
	HRESULT		Add_Component();
	void		SetUp_OnTerrain();
	void		Jumping(const _float& fTimeDelta);
	_int		Follow_Player(const _float& fTimeDelta);
	void		BillBoardYaw();

private:
	HRESULT		Create_Bullet();


private:
	Engine::CRcTex*			m_pBufferCom		 = nullptr;
	Engine::CTransform*		m_pTransformCom		 = nullptr;
	Engine::CTexture*		m_pTextureCom		 = nullptr;
	Engine::CRenderer*		m_pRendererCom		 = nullptr;
	Engine::CCalculator*	m_pCalculatorCom	 = nullptr;

private:
	_vec3	m_vUp;

	_float	m_fJumpSpeed;
	_float	m_fJumpAccel;
	_float	m_fTimeDeltaMax;

	_bool	m_bIsJump;

	_int	m_iTextureCount;
	_float	m_fAnimationTime;

	_float	m_fBulletCoolTime;
public:
	static CMonster*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void				Free() override;

};


#endif // Monster_h__
