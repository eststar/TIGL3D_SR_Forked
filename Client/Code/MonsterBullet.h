#ifndef MonsterBullet_h__
#define MonsterBullet_h__

#include "Define.h"
#include "Creature.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CMonsterBullet : public Engine::CCreature
{
private:
	explicit CMonsterBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterBullet();

public:
	HRESULT Ready_Object(CTransform* pTrans);
	_int	Update_Object(const _float& fTimeDelta)		override;
	void	LateUpdate_Object(const _float& fTimeDelta) override;
	void	Render_Object()								override;

private:
	HRESULT		Add_Component();
	void		BillBoardYaw();

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

	Engine::CTransform*		m_pPlayerTransformCom = nullptr;
	Engine::CTransform*		m_pMonsterTransformCom = nullptr;

	_float	m_fTimeDeltaMax;
	_vec3	m_vPlayerPos;
	_vec3	m_vLook;
	_vec3	m_vDir;
	_vec3	m_vMonsterPos;

public:
	static CMonsterBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pTrans);

public:
	void						Free() override;
};

#endif // !MonsterBullet_h__
