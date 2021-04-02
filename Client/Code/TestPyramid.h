#ifndef TestPyramid_h__
#define TestPyramid_h__

#include "Define.h"
#include "Creature.h"

BEGIN(Engine)

class CRcCol;
class CRcTex;
class CTetraTex;

class CTexture;

class CTransform;
class CRenderer;

END

class CTestPyramid : public Engine::CCreature
{
private:
	explicit CTestPyramid(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestPyramid();

public:
	HRESULT Ready_Object()								override;
	_int	Update_Object(const _float& fTimeDelta)		override;
	void	LateUpdate_Object(const _float& fTimeDelta) override;
	void	Render_Object()								override;
	
private:
	HRESULT		Add_Component();

private:
	//Engine::CRcCol*			m_pBufferCom = nullptr;
	//Engine::CRcTex*			m_pBufferCom	= nullptr;
	Engine::CTetraTex*			m_pBufferCom = nullptr;

	Engine::CTexture*		m_pTextureCom	= nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom	= nullptr;


public:
	static CTestPyramid*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void					Free() override;

};


#endif // TestPyramid_h__
