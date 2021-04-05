#ifndef MainApp_Tool_h__
#define MainApp_Tool_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"


BEGIN(Engine)

class CGraphicDev;
class CGameObject;
END
class CDynamicCamera;
class CTerrain;

class CMainApp_Tool  : public CBase
{
	DECLARE_SINGLETON(CMainApp_Tool)
	// 생성자, 소멸자
private:
	explicit CMainApp_Tool(void);
	virtual ~CMainApp_Tool(void);

	// public 함수, 변수
public:
	HRESULT		Ready_MainApp_Tool(void);
	_int				Update_MainApp_Tool(const _float& fTimeDelta);
	_int				Late_Update_MainApp_Tool(const _float& fTimeDelta);

	void				Render_MainApp_Tool(void);
public:
	//HRESULT		ReadImagePath();

	// protected 함수, 변수
	// private 함수, 변수
private:
	HRESULT				SetUp_Setting(LPDIRECT3DDEVICE9* ppGraphicDev);
	//HRESULT				Ready_Resources();

public:
	Engine::CGraphicDev*				m_pDeviceClass = nullptr;
	LPDIRECT3DDEVICE9				m_pGraphicDev = nullptr;
	CDynamicCamera*						m_pDynamicCamera = nullptr;
	vector<CGameObject*>*					m_pvecTerrain;
	vector<TERRAININFO*>					m_vecTile;

	_bool											m_bShowTex;
	_ulong										m_dwFillmode = D3DFILL_WIREFRAME;
public:
	LPDIRECT3DDEVICE9		Get_Device() { return m_pGraphicDev; }
	HRESULT							Ready_Resource(/*LPDIRECT3DDEVICE9& pGraphicDev*/);


	//HRESULT							Ready_Buffer(RESOURCEID eResourceID, const _tchar* pBufferTag
	//	, Engine::BUFFERID eBufferID, DWORD dwVTXCNTX, DWORD dwVTXCNTZ, DWORD dwVTXITV);
	HRESULT							ADD_Buffer(RESOURCEID eResourceID, const _tchar* pBufferTag
		, Engine::BUFFERID eBufferID, DWORD dwVTXCNTX, DWORD dwVTXCNTZ, DWORD dwVTXITV);



	//HRESULT							Ready_Texture(RESOURCEID eResourceID, const _tchar* pTextureTag
	//	, Engine::TEXTUREID eTextureID, const _tchar* pPath, _uint iCnt);

	// 생성 함수, 소멸 함수
public:
	virtual void					Free(void) override;

};




#endif // MainApp_Tool_h__
