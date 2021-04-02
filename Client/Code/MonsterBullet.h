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
	HRESULT Ready_Object()								override;
	_int	Update_Object(const _float& fTimeDelta)		override;
	void	LateUpdate_Object(const _float& fTimeDelta) override;
	void	Render_Object()								override;

private:
	HRESULT		Add_Component();
	//_int		Follow_Player(const _float& fTimeDelta);
	void		BillBoardYaw();

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

	Engine::CTransform*		m_pPlayerTransformCom = nullptr;

	_float	m_fTimeDeltaMax;
	_vec3	m_vPlayerPos;
	_vec3	m_vLook;

public:
	static CMonsterBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void						Free() override;
};

#endif // !MonsterBullet_h__
