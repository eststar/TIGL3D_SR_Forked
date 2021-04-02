#ifndef TimerMgr_h__
#define TimerMgr_h__

#include "Base.h"
#include "Timer.h"

BEGIN(Engine)

class ENGINE_DLL CTimerMgr : public  CBase
{
	DECLARE_SINGLETON(CTimerMgr)

private:
	explicit CTimerMgr();
	virtual ~CTimerMgr();

public:
	_float			Get_TimeDelta(TIMERID eTimerID);
	void			SetUp_TimeDelta(TIMERID eTimerID);

public:
	HRESULT			Ready_Timer(TIMERID eTimerID);

private:
	CTimer*			Find_Timer(TIMERID eTimerID);

private:
	map<TIMERID, CTimer*>				m_mapTimers;
	
public:
	virtual void	Free();

};

END
#endif // TimerMgr_h__