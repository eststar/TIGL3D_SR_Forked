#pragma once
#include "Define.h"
#include "GameObject.h"
BEGIN(Engine)

class CCubeTex;
class CTexture;
class CRenderer;
class CTransform;

END
class CSkyCube : public Engine::CGameObject
{
private:
	explicit CSkyCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkyCube();
public:
	HRESULT Ready_Object()						 override;
	_int Update_Object(const _float& fTimeDelta) override;
	void Render_Object()						 override;

private:
	HRESULT		Add_Component();
private:
	Engine::CCubeTex*		m_pBufferCom	= nullptr;
	Engine::CTexture*		m_pTextureCom	= nullptr;
	Engine::CRenderer*		m_pRendererCom	= nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;

public:
	static CSkyCube*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void					Free() override;
};

