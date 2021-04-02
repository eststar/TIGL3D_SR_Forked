#ifndef UILayer_h__
#define UILayer_h__

#include "Define.h"
#include "Layer.h"

BEGIN(Engine)

END

class CUILayer : public Engine::CLayer
{
private:
	explicit CUILayer();
	virtual ~CUILayer();

public:
	virtual HRESULT			Ready_Layer(_int iVecSize)					override;
	virtual  _int			Update_Layer(const _float& fTimeDelta)		override;
	virtual  void			LateUpdate_Layer(const _float& fTimeDelta)	override;

public:
	static CUILayer*		Create(_int iVecSize);
public:
	void					Free() override;

};


#endif // UILayer_h__
