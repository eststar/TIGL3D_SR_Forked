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
	//// ������ �� �ﰢ��
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

//0406 �÷��̾� ��ǥ ������� �������� CTerrain�߿� ��ſ� ���ִ��� �ε��� ������� TERRAININFO���� ã��
Engine::_ulong Engine::CCalculator::Compute_IndexOnTerrain(const _vec3* pPos, const vector<TERRAININFO*>& vecTerrainInfo)
{
	CGameObject* pObject = nullptr;
	for (auto& iter : vecTerrainInfo)
	{
		//�������� vPos��ŭ ��������, ��ü ����ũ��� X��  dwVtxCNX*dwVtxItv, Z��  dwVtxCNZ*dwVtxItv �̹Ƿ� �������� ���� �»����. 
		if (pPos->x >= iter->vPos.x && pPos->x <= iter->vPos.x + (iter->dwVtxCNX)*(iter->dwVtxItv))
		{
			if (pPos->z >= iter->vPos.z && pPos->z <= iter->vPos.z + (iter->dwVtxCNZ)*(iter->dwVtxItv))
			{
				return iter->dwIndex;
			}
		}

		//�ܾʵ���
		//�ٸ� RECT �� POINT �̿��ؼ� �Ҽ����κ��� ���ư��� ������ �������κп��� ������ ���� �� ���� �ʿ��ϴٸ� ���� AABB�浹? �簢�� �浹 �ʿ�
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

