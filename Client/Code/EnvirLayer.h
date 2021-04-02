#ifndef EnvirLayer_h__
#define EnvirLayer_h__

#include "Define.h"
#include "Layer.h"

BEGIN(Engine)

END

class CEnvirLayer : public Engine::CLayer
{
private:
	explicit CEnvirLayer();
	virtual ~CEnvirLayer();

public:
	virtual HRESULT			Ready_Layer(_int iVecSize)					override;
	virtual  _int			Update_Layer(const _float& fTimeDelta)		override;
	virtual  void			LateUpdate_Layer(const _float& fTimeDelta)	override;

public:
	static CEnvirLayer*		Create(_int iVecSize);
public:
	void					Free() override;

};


#endif // LogicLayer_h__
