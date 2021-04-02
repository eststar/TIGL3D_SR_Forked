#include "Transform.h"
#include <iostream>
using namespace std;
USING(Engine)

Engine::CTransform::CTransform()
	: m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
{
	D3DXMatrixIdentity(&m_matWorld);
}

Engine::CTransform::~CTransform()
{
}

//Get
const	Engine::_vec3* CTransform::Get_Info(INFO eType)
{
	return &m_vInfo[eType];
}

void CTransform::Get_Info(INFO eType, _vec3* pInfo)
{
	memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
}

const Engine::_matrix* CTransform::Get_WorldMatrix()
{
	return &m_matWorld;
}


void CTransform::Set_Matrix(const _matrix * pMatWorld)
{
	m_matWorld = *pMatWorld;
}

void CTransform::Set_Matrix(const _matrix & rMatWorld)
{
	m_matWorld = rMatWorld;
}

void CTransform::Set_Info(INFO eType, _vec3 * pInfo)
{
	memcpy(&m_vInfo[eType], pInfo, sizeof(_vec3));
}

//Set
void CTransform::Set_Pos(const _vec3* pPos)
{
	m_vInfo[INFO_POS] = *pPos;
}

void CTransform::Set_Scale(const _vec3* pScale)
{
	m_vScale = *pScale;
}

void CTransform::Set_Angle(const _vec3* pAngle)
{
	m_vAngle = *pAngle;
}

void CTransform::Set_Angle(ROTATION eType, const _float & fAngle)
{
	*(((_float*)&m_vAngle) + eType) = fAngle;
}


void CTransform::Set_Dir(_vec3 * pDir)
{
	memcpy(&m_vInfo[INFO_LOOK], pDir, sizeof(_vec3));
}


//Other
void CTransform::Move_Pos(const _vec3* const pDir, const _float& fTimeDelta, const _float& fSpeed)
{
	m_vInfo[INFO_POS] += *pDir * fTimeDelta * fSpeed;
}

void CTransform::Rotation(ROTATION eType, const _float& fAngle)
{
	*(((_float*)&m_vAngle) + eType) += fAngle;
}

void CTransform::Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix		matScale, matRot, matTrans;

	//matRot = *Compute_LookAtTarget(pTargetPos);

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	//m_matWorld = matScale * matRot * matTrans;
	m_matWorld = matScale * matTrans;
}

void CTransform::RotationAxis(_vec3 * vAxis, _float fAngle) // 0401_미정
{
	_matrix matScale, matRotAxis, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationAxis(&matRotAxis, vAxis, fAngle);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRotAxis * matTrans;
}

void CTransform::RotationX(_float fAngle) // 0401_미정
{
	_matrix matScale, matRotAxis, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationX(&matRotAxis, fAngle);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRotAxis * matTrans;
}

//General
HRESULT Engine::CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

_int Engine::CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	_matrix		matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	return 0;
}

//Private Func
const Engine::_matrix* CTransform::Compute_LookAtTarget(const _vec3* pTargetPos)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	_vec3		vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir);

	_matrix		matRot;
	_vec3		vUp;

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]);

	_float fAngle = acosf(D3DXVec3Dot(&vDir, &vUp));

	return D3DXMatrixRotationAxis(&matRot, &vAxis, fAngle);
}

Engine::CTransform* Engine::CTransform::Create()
{
	CTransform*		pInstance = new CTransform;

	if (FAILED(pInstance->Ready_Transform()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CTransform::Free()
{
}

