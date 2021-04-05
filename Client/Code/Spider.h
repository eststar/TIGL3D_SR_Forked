#ifndef Spider_h__
#define Spider_h__

#include "Define.h"
#include "Creature.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CCalculator;
END

class CSpider final : public Engine::CCreature
{
private:
	explicit CSpider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpider();

public:
	HRESULT		Ready_Object()								override;
	_int		Update_Object(const _float& fTimeDelta)		override;
	void		LateUpdate_Object(const _float& fTimeDelta)	override;
	void		Render_Object()								override;

private:
	HRESULT		Add_Component();
	void		SetUp_OnTerrain();
	void		BillBoardYaw();
	void		Frame_Move(const _float& fTimeDelta);
	_int		Follow_Player(const _float& fTimeDelta);

private:
	FRAME					m_tFrame;
	_vec3					m_vPlayerPos;

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTransform*		m_pPlayerTransformCom = nullptr;

	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;

public:
	static CSpider*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void				Free() override;
};


#endif // !Spider_h__
