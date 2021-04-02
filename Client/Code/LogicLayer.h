#ifndef LogicLayer_h__
#define LogicLayer_h__

#include "Define.h"
#include "Layer.h"

BEGIN(Engine)

END

class CLogicLayer : public Engine::CLayer
{
private:
	explicit CLogicLayer();
	virtual ~CLogicLayer();

public:
	virtual HRESULT			Ready_Layer(_int iVecSize)					override;
	virtual  _int			Update_Layer(const _float& fTimeDelta)		override;
	virtual  void			LateUpdate_Layer(const _float& fTimeDelta)	override;

public:
	static CLogicLayer*		Create(_int iVecSize);
public:
	void					Free() override;

};


#endif // LogicLayer_h__
