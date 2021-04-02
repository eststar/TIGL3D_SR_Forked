#ifndef HTerrainTex_h__
#define HTerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CHTerrainTex : public CVIBuffer
{
private:
	explicit CHTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHTerrainTex(const CHTerrainTex& rhs);
	virtual ~CHTerrainTex();

public:
	const _vec3*		Get_VtxPos() const { return m_pPos; }

public:
	HRESULT Ready_Buffers(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	void	Render_Buffers() override;
		
private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

	_vec3*				m_pPos;
	_bool				m_bClone;

public:
	static CHTerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv = 1);
	virtual CResources*			Clone();
	void						Free()		override;

};

END
#endif // HTerrainTex_h__
