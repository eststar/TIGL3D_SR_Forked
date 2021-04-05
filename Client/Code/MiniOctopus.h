#ifndef MiniOctopus_h__
#define MiniOctopus_h__

#include "Define.h"
#include "Creature.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CCalculator;

END
class CMiniOctopus : public Engine::CCreature
{
private:
	explicit CMiniOctopus(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniOctopus();

public:
	HRESULT Ready_Object()								override;
	_int Update_Object(const _float& fTimeDelta)		override;
	void LateUpdate_Object(const _float& fTimeDelta)	override;
	void Render_Object()								override;

private:
	HRESULT		Add_Component();
	void		Frame_Move(const _float& fTimeDelta);
	void		SetUp_OnTerrain();
	void		Jumping(const _float& fTimeDelta);
	_int		Follow_Player(const _float& fTimeDelta);
	void		BillBoardYaw();

private:
	Engine::CRcTex*			m_pBufferCom		 = nullptr;
	Engine::CTransform*		m_pTransformCom		 = nullptr;
	Engine::CTexture*		m_pTextureCom		 = nullptr;
	Engine::CRenderer*		m_pRendererCom		 = nullptr;
	Engine::CCalculator*	m_pCalculatorCom	 = nullptr;

private:
	FRAME	m_tFrame;

	_vec3	m_vUp;

	_float	m_fJumpSpeed;
	_float	m_fJumpAccel;
	_float	m_fTimeDeltaMax;

	_bool	m_bIsJump;

public:
	static CMiniOctopus*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void				Free() override;

};


#endif // CMiniOctopus_h__
