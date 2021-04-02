#ifndef Logo_h__
#define Logo_h__

#include "Define.h"
#include "Scene.h"


class CLogo : public Engine::CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo();

public:
	HRESULT Ready_Scene()							override;
	_int Update_Scene(const _float& fTimeDelta)		override;
	void LateUpdate_Scene(const _float& fTimeDelta) override;
	void Render_Scene()								override;
	
private:
	HRESULT				Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax);
	HRESULT				Ready_Layer_Enviroment(LAYERID eLayerID);
	HRESULT				Ready_Layer_UI(LAYERID eLayerID);

private:

public:
	static CLogo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void				Free() override;

};


#endif // Logo_h__
