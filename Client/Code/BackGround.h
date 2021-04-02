#ifndef BackGround_h__
#define BackGround_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;

END

class CBackGround : public Engine::CGameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackGround();

public:
	HRESULT Ready_Object()							override;
	_int	Update_Object(const _float& fTimeDelta) override;
	void	Render_Object()							override;
	
private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex*			m_pBufferCom	= nullptr;
	Engine::CTexture*		m_pTextureCom	= nullptr;
	Engine::CRenderer*		m_pRendererCom	= nullptr;

public:
	static CBackGround*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void					Free() override;

};


#endif // BackGround_h__
