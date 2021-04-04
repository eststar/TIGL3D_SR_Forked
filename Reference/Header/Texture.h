#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CResources
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	const _uint&	Get_TextureSize() { return m_vecTexture.size(); }
	void			Set_Texture(const _uint& iIndex = 0);

public:
	HRESULT			Ready_Texture(const _tchar* pPath, TEXTUREID eType, const _uint& iCnt);

private:
	vector<IDirect3DBaseTexture9*>				m_vecTexture;
	typedef vector<IDirect3DBaseTexture9*>		VECTEXTURE;

public:
	virtual CResources*		Clone();
	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTUREID eType, const _uint& iCnt = 1);
	virtual void			Free();

};

END
#endif // Texture_h__
