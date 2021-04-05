#pragma once
#include "Define.h"
#include "Creature.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CSpreadBullet :
	public Engine::CCreature
{
private:
	CSpreadBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	CSpreadBullet(LPDIRECT3DDEVICE9 pGraphicDev, const _float fAngle);
	~CSpreadBullet();

public:
	HRESULT Ready_Object()							 override;
	_int Update_Object(const _float& fTimeDelta)	 override;
	void LateUpdate_Object(const _float& fTimeDelta) override;
	void Render_Object()							 override;

	void Set_Pos(_vec3& vPos); // 미정_0330
	void Set_Angle(const _float& fAngle);
private:
	HRESULT		Add_Component();
	void		BillBoard(); // 미정_0330 빌보드 함수 추가

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CTransform*		m_pPlayerTransformCom = nullptr;

	_vec3					m_vLook;

	_int					m_iTextureCount;
	_float					m_fTime;
	_float					m_fAngle;
public:
	static CSpreadBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CSpreadBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float fAngle);

public:
	void				Free() override;
};

