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
class CResources;
class CComponent;
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
public:
	void	Set_Component(COMPONENTID eCOMID, const _tchar* strCOMTag, CComponent* pCom);
	void	Set_Resource(COMPONENTID eCOMID, const _tchar* strCOMTag, CResources* pResource);

private:
	HRESULT		Add_Component();

private:
	Engine::CTerrainTex*	m_pBufferCom	 = nullptr;
	Engine::CTexture*		m_pTextureCom	 = nullptr;
	Engine::CRenderer*		m_pRendererCom	 = nullptr;
	Engine::CTransform*		m_pTransformCom	 = nullptr;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static HRESULT		Load_Terrain(LPDIRECT3DDEVICE9& pGraphicDev, const _tchar* pPath, vector<Engine::TERRAININFO*>& rVecTerrainInfo, vector<CGameObject*>& rVecTerrain);

public:
	void					Free() override;

};


#endif // Terrain_h__
