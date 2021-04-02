#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex : public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex();

public:
	HRESULT Ready_Buffers()			override;
	void	Render_Buffers()		override;

public:
	const _vec3*  Get_VtxPos() const { return m_arrVecPos; }
		
public:
	static CRcTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResources* Clone();
	void				Free()		override;

private:
	_vec3				m_arrVecPos[4];

};

END
#endif // RcTex_h__
