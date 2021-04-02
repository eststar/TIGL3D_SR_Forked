#pragma once
#include "Define.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;
class CCalculator;

END
class CFirstCamera : public Engine::CCamera
{
private:
	explicit CFirstCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFirstCamera();

public:
	HRESULT Ready_Object(const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fAspect,
		const _float& fFovY,
		const _float& fNear,
		const _float& fFar);

	virtual _int Update_Object(const _float& fTimeDelta) override;

private:
	HRESULT		Add_Component(void);
	void		Mouse_Fix();
	void		Mouse_Move(const _float& fTimeDelta);
	void		Camera_Move();

private:
	_float					m_fSpeed = 10.f;
	_float					m_fDistance = 10.f;
	_float					m_fAngle = 0.f;

public:
	static CFirstCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3* pEye,
										const _vec3* pAt,
										const _vec3* pUp,
										const _float& fAspect,
										const _float& fFovY,
										const _float& fNear,
										const _float& fFar);

public:
	void Free(void) override;

};

