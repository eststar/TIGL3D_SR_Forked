#ifndef Component_h__
#define Component_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent : public CBase 
{
protected:
	explicit CComponent();
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

public:
	virtual _int	Update_Component(const _float& fTimeDelta);
	virtual void	LateUpdate_Component(const _float& fTimeDelta);

protected:
	virtual void	Free() PURE;

};

END
#endif // Component_h__