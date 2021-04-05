#ifndef Stage_h__
#define Stage_h__

#include "Define.h"
#include "Scene.h"

BEGIN(Engine)
class CResources;
class CComponent;
END

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();

public:
	HRESULT Ready_Scene()							override;
	_int Update_Scene(const _float& fTimeDelta)		override;
	void LateUpdate_Scene(const _float& fTimeDelta) override;
	void Render_Scene()								override;
public:
	HRESULT		Load_Terrain(LPDIRECT3DDEVICE9& rpGraphicDev, CLayer*&	pLayer, const _tchar* pPath);

private:
	vector<Engine::TERRAININFO*>		m_VecTerrainInfo;
	vector<Engine::CGameObject*>		m_VecTerrain;

private:
	HRESULT				Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev);
	HRESULT				Ready_Layer_Enviroment(LAYERID eLayerID);
	HRESULT				Ready_Layer_GameLogic(LAYERID eLayerID);
	HRESULT				Ready_Layer_UI(LAYERID eLayerID);

public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void				Free() override;

};


#endif // Stage_h__
