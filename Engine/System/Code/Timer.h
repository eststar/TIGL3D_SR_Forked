#ifndef Timer_h__
#define Timer_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer();

public:
	_float			Get_TimeDelta() { return m_fTimeDelta; }
	void			SetUp_TimeDelta();

public:
	HRESULT			Ready_Timer();

private:
	_float				m_fTimeDelta;

	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;


public:
	static CTimer*		Create();
	virtual void		Free();
};

END
#endif // Timer_h__
