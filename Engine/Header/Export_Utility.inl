// Management
// Get
CComponent*		Get_Component(LAYERID eLayerID, const _int& iIndex, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eType)
{
	return CManagement::GetInstance()->Get_Component(eLayerID, iIndex, pObjTag, pComponentTag, eType);
}
// Set
HRESULT			 Add_GameObject(LAYERID eLayerID, const _int& iIndex, CGameObject* pGameObject)
{
	return CManagement::GetInstance()->Add_GameObject(eLayerID, iIndex, pGameObject);
}
// General
HRESULT			Ready_Management(CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();

	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppManagement = pManagement;

	return S_OK;
}

HRESULT			SetUp_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->SetUp_Scene(pScene);
 }
_int			Update_Scene(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
 }
void			LateUpdate_Scene(const _float& fTimeDelta)
{
	CManagement::GetInstance()->LateUpdate_Scene(fTimeDelta);
}
void			Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(pGraphicDev);
 }

// Renderer
// Get
CRenderer*		Get_Renderer()
{
	return CRenderer::GetInstance();
}

// Set
// General
void			Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eGroup, pGameObject);
}
void			Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void			Clear_RenderGroup()
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}


// Release Utility
void				Release_Utility()
{
	CRenderer::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
 }