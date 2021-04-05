#ifndef WalkCannon_h__
#define WalkCannon_h__

#include "Define.h"
#include "Creature.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CCalculator;
END

class CWalkCannon : public Engine::CCreature
{
private:
	explicit CWalkCannon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWalkCannon();

public:
	HRESULT		Ready_Object()								override;
	_int		Update_Object(const _float& fTimeDelta)		override;
	void		LateUpdate_Object(const _float& fTimeDelta)	override;
	void		Render_Object()								override;

private:
	HRESULT		Add_Component();
	void		SetUp_OnTerrain();
	void		BillBoardYaw();
	void		Frame_Move(const _float& fTimeDelta);
	_int		Follow_Player(const _float& fTimeDelta);

private:
	HRESULT		Create_Bullet();

private:
	FRAME					m_tFrame;
	_float					m_fBulletCoolTime;
	_vec3					m_vPlayerPos;
	_vec3					m_vMonsterPos;

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTransform*		m_pPlayerTransformCom = nullptr;
	Engine::CTransform*		m_pMonsterTransformCom = nullptr;

	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;

public:
	static CWalkCannon*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void				Free() override;
};
#endif // !WalkCannon_h__
