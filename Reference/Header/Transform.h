#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform();
	virtual ~CTransform();


public:
	//Get

	/* 0405_다영 Get_Angle(), Get_Scale() 함수 추가 */
	const	_vec3*		Get_Angle();
	const	_vec3*		Get_Scale();

	const	_vec3*		Get_Info(INFO eType);
	void				Get_Info(INFO eType, _vec3* pInfo);
	const _matrix*		Get_WorldMatrix();


	//Set
	void				Set_Matrix(const _matrix* pMatWorld);
	void				Set_Matrix(const _matrix& rMatWorld);
	void				Set_Info(INFO eType, _vec3* pInfo);
	void				Set_Pos(const _vec3* pPos);
	void				Set_Scale(const _vec3* pScale);
	void				Set_Angle(const _vec3* pAngle);
	void				Set_Angle(ROTATION eType, const _float& fAngle);
	void				Set_Dir(_vec3* pDir);

	//Other
	void				Move_Pos(const _vec3* const pDir, const _float& fTimeDelta, const _float& fSpeed);
	void				Rotation(ROTATION eType, const _float& fAngle);
	void				Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	void				RotationAxis(_vec3* vAxis, _float fAngle); // 0401_미정
	void				RotationX(_float fAngle); // 0401_미정
	void				RotationY(_float fAngle); // 0401_미정
	void				RotationZ(_float fAngle); // 0401_미정

public:
	//General
	HRESULT				Ready_Transform();
	virtual				_int Update_Component(const _float& fTimeDelta);

private:
	const _matrix*		Compute_LookAtTarget(const _vec3* pTargetPos);

private:
	_vec3				m_vInfo[INFO_END];
	_vec3				m_vScale;
	_vec3				m_vAngle;
	_matrix				m_matWorld;

public:
	static	CTransform*		Create();
	virtual void			Free();
};

END
#endif // Transform_h__
