#pragma once
#include "Define.h"
#include "Creature.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CFireBullet :
	public Engine::CCreature
{
private:
	CFireBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	~CFireBullet();

public:
	HRESULT Ready_Object()							 override;
	_int Update_Object(const _float& fTimeDelta)	 override;
	void LateUpdate_Object(const _float& fTimeDelta) override;
	void Render_Object()							 override;

	void Set_Pos(_vec3& vPos); // ����_0330
private:
	HRESULT		Add_Component();
	void		BillBoard(); // ����_0330 ������ �Լ� �߰�

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CTransform*		m_pPlayerTransformCom = nullptr;

	_vec3					m_vLook;

	_int					m_iTextureCount;
	_float					m_fTime;
public:
	static CFireBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void				Free() override;
};

