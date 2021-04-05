#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Engine_Define.h"
#include "Base.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "HTerrainTex.h"
#include "CubeTex.h"
#include "TetraTex.h"

#include "Texture.h"

BEGIN(Engine)

class ENGINE_DLL CResourcesMgr : public CBase
{
	DECLARE_SINGLETON(CResourcesMgr)

private:
	explicit CResourcesMgr();
	virtual ~CResourcesMgr();

public:
	CResources*		Clone(const _ushort& wContainerIdx, const _tchar* pResourceTag);

public:
	HRESULT		Reserve_ContainerSize(const _ushort& wSize);
	HRESULT		Ready_Buffers(LPDIRECT3DDEVICE9& pGraphicDev, const _ushort& wContainerIdx, 
								const _tchar* pBufferTag, BUFFERID eID,
								const _ulong& dwCntX = 129,
								const _ulong& dwCntZ = 129,
								const _ulong& dwVtxItv = 1);
	void		Render_Buffers(const _ushort& wContainerIdx, const _tchar* pBufferTag);

	HRESULT		Ready_Textures(LPDIRECT3DDEVICE9& pGraphicDev, 
								const _ushort& wContainerIdx, 
								const _tchar* pTextureTag, 
								TEXTUREID eID, 
								const _tchar* pPath, 
								const _uint& iCnt = 1);

public:
	//0406_새로운 버퍼 추가용
	HRESULT			ADD_Buffer(LPDIRECT3DDEVICE9& pGraphicDev, const _ushort & wContainerIdx
		, const _tchar * pBufferTag, BUFFERID eID, const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwVtxItv);

private:
	CResources*		Find_Resources(const _ushort& wContainerIdx, const _tchar* pResourceTag);

private:
	_ulong										m_dwContainerSize;
	map<const _tchar*, CResources*>*			m_pmapResources;
	typedef	map<const _tchar*, CResources*>		MAPRESOURCES;
	
public:
	virtual void	Free();
};

END
#endif // ResourcesMgr_h__
