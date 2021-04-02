#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Define.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;

END

class CStaticCamera : public Engine::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticCamera();

public:
	HRESULT		 Ready_Object(const _vec3* pEye,
							 const _vec3* pAt,
							 const _vec3* pUp,
							 const _float& fAspect,
							 const _float& fFovY,
							 const _float& fNear,
							 const _float& fFar);

	virtual _int Update_Object(const _float& fTimeDelta)	 override;
	virtual void LateUpdate_Object(const _float& fTimeDelta) override;


private:
	HRESULT		Add_Component();
	void		Target_Renewal();
	void		Key_Input(const _float& fTimeDelta);

private:
	Engine::CTransform*		m_pTransformCom = nullptr;

	_float					m_fSpeed = 10.f;
	_float					m_fDistance = 10.f;
	_float					m_fAngle = 0.f;

public:
	static CStaticCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3* pEye,
										const _vec3* pAt,
										const _vec3* pUp,
										const _float& fAspect,
										const _float& fFovY,
										const _float& fNear,
										const _float& fFar);

public:
	void Free() override;

};


#endif // StaticCamera_h__
