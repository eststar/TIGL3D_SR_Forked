#include "Calculator.h"
#include "GameObject.h"

USING(Engine)

Engine::CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CCalculator::CCalculator(const CCalculator& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CCalculator::~CCalculator()
{
}

HRESULT Engine::CCalculator::Ready_Calculator()
{
	return S_OK;
}

Engine::_float Engine::CCalculator::Compute_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVtx, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	_ulong	dwIndex = _ulong(pPos->z / dwVtxItv) * dwCntX + _ulong(pPos->x / dwVtxItv);

	if (dwIndex > dwCntX * dwCntZ)
		dwIndex = 0;

	_float	fRatioX = (pPos->x - pTerrainVtx[dwIndex + dwCntX].x) / dwVtxItv;
	_float	fRatioZ = (pTerrainVtx[dwIndex + dwCntX].z - pPos->z) / dwVtxItv;

	//D3DXPLANE		Plane;
	//// 오른쪽 위 삼각형
	//if (fRatioX > fRatioZ)
	//{
	//	D3DXPlaneFromPoints(&Plane, 
	//		&pTerrainVtx[dwIndex + dwCntX],
	//		&pTerrainVtx[dwIndex + dwCntX + 1],
	//		&pTerrainVtx[dwIndex + 1]);
	//}
	//
	//else
	//{
	//	D3DXPlaneFromPoints(&Plane,
	//		&pTerrainVtx[dwIndex + dwCntX],
	//		&pTerrainVtx[dwIndex + 1],
	//		&pTerrainVtx[dwIndex]);
	//}
	//// ax + by + cz + d 
	//// by = -ax - cz - d
	//// y = (-ax - cz - d) / b
	//
	//return (-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b;

	_float	fHeight[4] = {

		pTerrainVtx[dwIndex + dwCntX].y,
		pTerrainVtx[dwIndex + dwCntX + 1].y,
		pTerrainVtx[dwIndex + 1].y,
		pTerrainVtx[dwIndex].y

	};

	if (fRatioX > fRatioZ)
	{
		return fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}

	else
	{
		return fHeight[0] + (fHeight[2] - fHeight[3]) * fRatioX + (fHeight[3] - fHeight[0]) * fRatioZ;
	}
}

//0406 플레이어 좌표 기반으로 여러개의 CTerrain중에 어떤거에 서있는지 인덱스 얻기위해 TERRAININFO에서 찾음
Engine::_ulong Engine::CCalculator::Compute_IndexOnTerrain(const _vec3* pPos, const vector<TERRAININFO*>& vecTerrainInfo)
{
	CGameObject* pObject = nullptr;
	for (auto& iter : vecTerrainInfo)
	{
		//원점기준 vPos만큼 움직였고, 전체 지형크기는 X축  dwVtxCNX*dwVtxItv, Z축  dwVtxCNZ*dwVtxItv 이므로 원점기준 각각 좌상우하. 
		if (pPos->x >= iter->vPos.x && pPos->x <= iter->vPos.x + (iter->dwVtxCNX)*(iter->dwVtxItv))
		{
			if (pPos->z >= iter->vPos.z && pPos->z <= iter->vPos.z + (iter->dwVtxCNZ)*(iter->dwVtxItv))
			{
				return iter->dwIndex;
			}
		}

		//외않되지
		//다만 RECT 및 POINT 이용해서 소수점부분이 날아가기 때문에 지형끝부분에서 문제가 있을 수 있음 필요하다면 예전 AABB충돌? 사각형 충돌 필요
		//RECT rc = { _int(iter->vPos.x), (_int((iter->vPos.z) + ((iter->dwVtxCNZ)*(iter->dwVtxItv))))
		//				,  _int((iter->vPos.x) + ((iter->dwVtxCNX)*(iter->dwVtxItv))), _int(iter->vPos.z) };
		//POINT pt = { _int(pPos->x), _int(pPos->z)};
		//
		//_uint iTest = PtInRect(&rc, pt);
		//if (PtInRect(&rc, pt))
		//{
		//	return iter->dwIndex;
		//}
	}
	return -1;
}

Engine::CCalculator* Engine::CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator*	pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CCalculator::Clone()
{
	return new CCalculator(*this);
}

void Engine::CCalculator::Free()
{
	Safe_Release(m_pGraphicDev);
}

