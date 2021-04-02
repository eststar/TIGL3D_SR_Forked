#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CResources
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	virtual HRESULT			Ready_Buffers();
	virtual void			Render_Buffers();

public:
	_pVtxBuffer				GetVtxBuffer() { return m_pVB; }

protected:
	_pVtxBuffer						m_pVB;
	_pIdxBuffer						m_pIB;

	_ulong							m_dwVtxSize;
	_ulong							m_dwVtxCnt;
	_ulong							m_dwFVF;
	_ulong							m_dwTriCnt;

	_ulong							m_dwIdxSize;

	D3DFORMAT						m_dwIdxFmt;

public:
	virtual void			Free();

};

END
#endif // VIBuffer_h__
