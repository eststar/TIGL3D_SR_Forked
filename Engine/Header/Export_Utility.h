#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "Calculator.h"
#include "Transform.h"
#include "Renderer.h"
#include "Creature.h"
#include "Camera.h"
#include "Scene.h"
BEGIN(Engine)

// Management
// Get
inline CComponent*		Get_Component(LAYERID eLayerID, const _int& iIndex, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eType);
inline CScene*				Get_Scene(); //0406_ManagementÂü°í
// Set
inline HRESULT			Add_GameObject(LAYERID eLayerID, const _int& iIndex, CGameObject* pGameObject);
// General
inline HRESULT			Ready_Management(CManagement** ppManagement);
inline HRESULT			SetUp_Scene(CScene* pScene);
inline _int				Update_Scene(const _float& fTimeDelta);
inline void				LateUpdate_Scene(const _float& fTimeDelta);
inline void				Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev);


// Renderer
// Get
inline CRenderer*		Get_Renderer();
// Set
// General
inline void			Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject);
inline void			Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void			Clear_RenderGroup();


// Release Utility
inline void				Release_Utility();

#include "Export_Utility.inl"

END
#endif // Export_Utility_h__