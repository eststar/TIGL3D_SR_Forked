#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Define.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;

END

class CDynamicCamera : public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera();

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
	HRESULT		Add_Component();
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(const _float& fTimeDelta);
	void		Mouse_Fix();

private:
	Engine::CTransform*		m_pTransformCom = nullptr;

	_float					m_fSpeed = 10.f;
	_bool					m_bClick = false;
	_bool					m_bMouseFix = true;

public:
	static CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										const _vec3* pEye, 
										const _vec3* pAt, 
										const _vec3* pUp, 
										const _float& fAspect, 
										const _float& fFovY, 
										const _float& fNear,
										const _float& fFar);

public:
	void						Free() override;

};


#endif // DynamicCamera_h__
