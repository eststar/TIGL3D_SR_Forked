#ifndef Creature_h__
#define Creature_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCreature : public CGameObject
{
protected:
	explicit CCreature(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCreature();

public:
	const _int&			Get_HP()  { return m_iHP; }
	const _int&			Get_MaxHP() { return m_iMaxHP; }
	const _int&			Get_Att() { return m_iAtt; }
	const _int&			Get_Def() { return m_iDef; }

public:
	void				Set_HP(const _int& iHP)					{ m_iHP += iHP; }
	void				Set_Att(const _int& iAtt)				{ m_iAtt += iAtt; }
	void				Set_Def(const _int& iDef)				{ m_iDef += iDef; }

	void				Set_IsDead(const _bool& bIsDead = true)	{ m_bIsDead = bIsDead; }

public:
	virtual HRESULT		Ready_Object()								override;
	virtual _int		Update_Object(const _float& fTimeDelta)		override;
	virtual void		LateUpdate_Object(const _float& fTimeDelta) override;

protected:
	_int m_iHP;
	_int m_iMaxHP;

	_int m_iAtt;
	_int m_iDef;

	_bool m_bIsDead;

public:
	virtual void Free();
};

END
#endif // Creature_h__
