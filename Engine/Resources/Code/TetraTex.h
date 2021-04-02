#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class CTetraTex :
	public CVIBuffer
{
public:
	explicit CTetraTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTetraTex(const CTetraTex& rhs);
	virtual ~CTetraTex();

public:
	virtual HRESULT	Ready_Buffer();

public:
	static CTetraTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResources*	Clone();
	virtual void		Free();

};
END
