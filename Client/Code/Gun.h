#pragma once
#include "GameObject.h"

BEGIN(Engine)
	class CRcTex;
	class CTexture;
	class CTransform;
	class CRenderer;
END
class CGun :
	public Engine::CGameObject
{
private:
	explicit		CGun(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			~CGun();

public: // 미정_0331
	enum STATE { IDLE, WALKING, SHOOTING, STATE_END };
public:
	HRESULT			Ready_Object()								override;
	_int			Update_Object(const _float& fTimeDelta)		override;
	void			LateUpdate_Object(const _float& fTimeDelta)	override;
	void			Render_Object()								override;

	void			Gun_On();
	void			Gun_Off();
private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

	Engine::CTransform*		m_pPlayerTransformCom = nullptr;
private:
	STATE		m_eState = IDLE;
	_float		m_fAngle;
	_float		m_fShootAngle;

	_float		m_fSpeed;
	_float		m_fTime;
	_float		m_fShootTime;
	_vec3		m_vDir;
	_bool		m_bIsOn;

public:
	void Set_State(STATE state); // 미정_0331 
public:
	static CGun*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void			Free() override;
};

