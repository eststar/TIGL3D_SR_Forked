#ifndef Terrain_h__
#define Terrain_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Engine)

class CTerrainTex;
class CRcTex;

class CTexture;
//class CRenderer;
class CTransform;
class CResources;
class CComponent;
END
class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	//explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain(void);

public:
	HRESULT Ready_Object(void) override;
	_int Update_Object(const _float& fTimeDelta) override;
	void Render_Object(void) override;
	
private:
	HRESULT		Add_Component(void);

public:
	Engine::CTerrainTex*	m_pBufferCom		= nullptr;
	//Engine::CRcTex*	m_pBufferCom = nullptr;

	Engine::CTexture*			m_pTextureCom		= nullptr;
	//Engine::CRenderer*		m_pRendererCom	= nullptr;
	Engine::CTransform*		m_pTransformCom	= nullptr;

	//list<_tchar*>			m_lstTagNames;
	_bool								m_bShowTexture	= false;
	_ulong							m_dwFillmode		= D3DFILL_WIREFRAME;

	_ulong							m_dwIndex			= 0;
	_byte								m_byOption			= OPTION_BLOCKED;
	_byte								m_byType				= TYPE_FLOOR;
public:
	void	Set_Component(COMPONENTID eCOMID, const _tchar* strCOMTag, CComponent* pCom);
	void	Set_Resource(COMPONENTID eCOMID, const _tchar* strCOMTag, CResources* pResource);

	void Set_Option(_byte byOption) { m_byOption = byOption; }
	void Set_Type(_byte byType) { m_byType = byType; }
	void Set_Index(_ulong dwIndex) { m_dwIndex = dwIndex; }

	const _byte Get_Option() { return m_byOption; }
	const _byte Get_Type() { return m_byType; }
	const _ulong Get_Index() { return m_dwIndex; }

	//map<const _tchar*, CComponent*>* 	Get_mapCOM(COMPONENTID eCOMID);
public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void Free(void) override;

};


#endif // Terrain_h__
