#ifndef Player_h__
#define Player_h__

#include "Define.h"
#include "Creature.h"
#include "Bullet.h"
#include "Gun.h"
#include "BigBullet.h"
#include "Ray.h"
#include "IceBullet.h"

BEGIN(Engine)

class CRcTex; // 미정_0330 추가
class CCubeTex;
class CTexture;
class CTransform;
class CRenderer;
class CCalculator;
class CLayer;

END
class CPlayer : public Engine::CCreature
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

	enum GUNSKILL { FASTBULLET, BIGBULLET, RAY, FIRE, SHOTGUN, BARRIER, BOMB, ICE, SKILLEND }; // 미정_0401
public:
	HRESULT Ready_Object()								override;
	_int	Update_Object(const _float& fTimeDelta)		override;
	void	LateUpdate_Object(const _float& fTimeDelta) override;
	void	Render_Object()								override;


private:
	HRESULT		Add_Component();
	void		Key_Input(const _float& fTimeDelta);
	void		SetUp_OnTerrain();
	void		Mouse_Fix();

private:
	HRESULT				CreateBullet();
	HRESULT				Create_Bullet(); // 미정_0330 인자지움
	HRESULT				Create_BigBullet();
	HRESULT				Create_Ray();
	HRESULT				Create_IceBullet();

private:
	Engine::CCubeTex*		m_pBufferCom		 = nullptr;
	Engine::CTexture*		m_pTextureCom		 = nullptr;
	Engine::CTransform*		m_pTransformCom		 = nullptr;
	Engine::CRenderer*		m_pRendererCom		 = nullptr;
	Engine::CCalculator*	m_pCalculatorCom	 = nullptr;
	Engine::CGameObject*	m_pGun				 = nullptr; // 미정_0331 총
	
	_vec3					m_vLook;
	_bool					m_bIsJump;
	_float					m_fTime;
	_bool					m_bHasGun;	// 미정_0331

	_bool					m_bSkillOn;	// 미정_0401 
	GUNSKILL				m_eGunSkill;
public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void				Free() override;

};


#endif // Player_h__
