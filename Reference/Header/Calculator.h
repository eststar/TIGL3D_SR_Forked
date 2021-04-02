#ifndef Calculator_h__
#define Calculator_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCalculator : public CComponent
{
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator();

public:
	HRESULT			Ready_Calculator();
	_float			Compute_HeightOnTerrain(const _vec3* pPos, 
											const _vec3* pTerrainVtx, 
											const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;

public:
	static CCalculator*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone();

private:
	virtual void Free();		
};

END
#endif // Calculator_h__