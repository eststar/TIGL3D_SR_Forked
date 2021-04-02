#ifndef Terrain_h__
#define Terrain_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Engine)

class CTerrainTex;
class CHTerrainTex;
class CTexture;
class CRenderer;
class CTransform;

END
class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain();

public:
	HRESULT Ready_Object()							 override;
	_int Update_Object(const _float& fTimeDelta)	 override;
	void LateUpdate_Object(const _float& fTimeDelta) override;
	void Render_Object()							 override;

private:
	HRESULT		Add_Component();

private:
	Engine::CTerrainTex*	m_pBufferCom	 = nullptr;
	Engine::CTexture*		m_pTextureCom	 = nullptr;
	Engine::CRenderer*		m_pRendererCom	 = nullptr;
	Engine::CTransform*		m_pTransformCom	 = nullptr;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void					Free() override;

};


#endif // Terrain_h__
