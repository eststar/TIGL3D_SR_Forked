#ifndef Terrain_h__
#define Terrain_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Engine)

class CTerrainTex;
class CHTerrainTex;
class CTexture;
class CRenderer;
class CTransform;

//0406_지형 로드용
class CResources;
class CComponent;
END
class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain();

public:
	HRESULT Ready_Object()							 override;
	_int Update_Object(const _float& fTimeDelta)	 override;
	void LateUpdate_Object(const _float& fTimeDelta) override;
	void Render_Object()							 override;
public:
	//0406 지형 로드시 컴포넌트 변경용 함수들
	void	Set_Component(COMPONENTID eCOMID, const _tchar* strCOMTag, CComponent* pCom);
	void	Set_Resource(COMPONENTID eCOMID, const _tchar* strCOMTag, CResources* pResource);

	//0406_지형 로드시 옵션 적용 함수
	void Set_Option(_byte byOption) { m_byOption = byOption; }
	void Set_Type(_byte byType) { m_byType = byType; }
	void Set_Index(_ulong dwIndex) { m_dwIndex = dwIndex; }
	//0406 지형 옵션 외부에서 사용시
	const _byte Get_Option() { return m_byOption; }
	const _byte Get_Type() { return m_byType; }
	const _ulong Get_Index() { return m_dwIndex; }

private:
	HRESULT		Add_Component();

private:
	Engine::CTerrainTex*	m_pBufferCom	 = nullptr;
	Engine::CTexture*		m_pTextureCom	 = nullptr;
	Engine::CRenderer*		m_pRendererCom	 = nullptr;
	Engine::CTransform*		m_pTransformCom	 = nullptr;

	//0406_지형 옵션
	_byte								m_byOption = OPTION_BLOCKED;
	_byte								m_byType = TYPE_FLOOR;
	_ulong							m_dwIndex = 0;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void					Free() override;

};


#endif // Terrain_h__
