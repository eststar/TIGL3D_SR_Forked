#ifndef OctopusBoss_h__
#define OctopusBoss_h__

#include "Define.h"
#include "Creature.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CCalculator;

END

class COctopusBoss : public Engine::CCreature
{
private:
	explicit COctopusBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COctopusBoss();

public:
	HRESULT Ready_Object()								override;
	_int	Update_Object(const _float& fTimeDelta)		override;
	void	LateUpdate_Object(const _float& fTimeDelta) override;
	void	Render_Object()								override;

private:
	HRESULT	Add_Component();
	void	Frame_Move(const _float& fTimeDelta);
	void	Jumping(const _float& fTimeDelta);
	_int	Follow_Player(const _float& fTimeDelta);
	void	BillBoardYaw();
	void	SetUp_OnTerrain();

private:
	HRESULT Create_Monster();

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTransform*		m_pPlayerTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

	Engine::CCalculator*	m_pCalculatorCom = nullptr;

	

private:
	FRAME					m_tFrame;

	_vec3					m_vUp;
	_vec3					m_vPlayerPos;

	_float					m_fJumpSpeed;
	_float					m_fJumpAccel;
	_float					m_fTimeDeltaMax;
	_int					m_iJumpCnt;
	_int					m_iJumpCntMax;
	_bool					m_bIsJump;

	wstring					wstrPatternKey;

	_float					m_fPatternTime;

public:
	static COctopusBoss*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void						Free() override;
};



#endif // !OctopusBoss_h__
