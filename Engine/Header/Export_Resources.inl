// ResourcesMgr
// Get
CResources*		Clone(const _ushort& wContainerIdx, const _tchar* pResourceTag)
{
	return CResourcesMgr::GetInstance()->Clone(wContainerIdx, pResourceTag);
}
// Set
inline HRESULT			ADD_Buffer(LPDIRECT3DDEVICE9& pGraphicDev, const _ushort & wContainerIdx
	, const _tchar * pBufferTag, BUFFERID eID, const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwVtxItv);

// General
HRESULT		Reserve_ContainerSize(const _ushort& wSize)
{
	return CResourcesMgr::GetInstance()->Reserve_ContainerSize(wSize);
 }
HRESULT		Ready_Buffers(LPDIRECT3DDEVICE9& pGraphicDev, const _ushort& wContainerIdx,
							const _tchar* pBufferTag, BUFFERID eID,
							const _ulong& dwCntX/* = 129 */,
							const _ulong& dwCntZ/* = 129 */,
							const _ulong& dwVtxItv/* = 1 */)
{
	return CResourcesMgr::GetInstance()->Ready_Buffers(pGraphicDev, wContainerIdx, pBufferTag, eID, dwCntX, dwCntZ, dwVtxItv);
 }
HRESULT		Ready_Textures(LPDIRECT3DDEVICE9& pGraphicDev, 
							const _ushort& wContainerIdx, 
							const _tchar* pTextureTag, 
							TEXTUREID eID, 
							const _tchar* pPath, 
							const _uint& iCnt/* = 1 */)
{
	return CResourcesMgr::GetInstance()->Ready_Textures(pGraphicDev, wContainerIdx, pTextureTag, eID, pPath, iCnt);
}

void		Render_Buffers(const _ushort& wContainerIdx, const _tchar* pBufferTag)
{
	CResourcesMgr::GetInstance()->Render_Buffers(wContainerIdx, pBufferTag);
 }

//Release Resources
void			Release_Resources()
{
	CResourcesMgr::GetInstance()->DestroyInstance();
 }