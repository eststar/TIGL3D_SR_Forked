#ifndef Export_Resources_h__
#define Export_Resources_h__

#include "ResourcesMgr.h"

BEGIN(Engine)

// ResourcesMgr
// Get
inline CResources*		Clone(const _ushort& wContainerIdx, const _tchar* pResourceTag);
// Set

HRESULT			ADD_Buffer(LPDIRECT3DDEVICE9& pGraphicDev, const _ushort & wContainerIdx
	, const _tchar * pBufferTag, BUFFERID eID, const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwVtxItv)
{
	return CResourcesMgr::GetInstance()->ADD_Buffer(pGraphicDev, wContainerIdx, pBufferTag, eID, dwCntX, dwCntZ, dwVtxItv);
}

// General
inline HRESULT		Reserve_ContainerSize(const _ushort& wSize);
inline HRESULT		Ready_Buffers(LPDIRECT3DDEVICE9& pGraphicDev, const _ushort& wContainerIdx,
									const _tchar* pBufferTag, BUFFERID eID,
									const _ulong& dwCntX = 129,
									const _ulong& dwCntZ = 129,
									const _ulong& dwVtxItv = 1);
inline HRESULT		Ready_Textures(LPDIRECT3DDEVICE9& pGraphicDev, 
									const _ushort& wContainerIdx, 
									const _tchar* pTextureTag, 
									TEXTUREID eID, 
									const _tchar* pPath, 
									const _uint& iCnt = 1);
inline void			Render_Buffers(const _ushort& wContainerIdx, const _tchar* pBufferTag);

//Release Resources
inline void			Release_Resources();


#include "Export_Resources.inl"
END

#endif // Export_Resources_h__
