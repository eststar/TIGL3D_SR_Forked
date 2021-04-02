
#include "TimerMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimerMgr)

CTimerMgr::CTimerMgr()
{

}

CTimerMgr::~CTimerMgr()
{
	Free();
}

_float CTimerMgr::Get_TimeDelta(TIMERID eTimerID)
{
	CTimer*	pTimer = Find_Timer(eTimerID);

	if (nullptr == pTimer)
		return 0.f;
	
	return pTimer->Get_TimeDelta();
}

void CTimerMgr::SetUp_TimeDelta(TIMERID eTimerID)
{
	CTimer*	pTimer = Find_Timer(eTimerID);

	if (nullptr == pTimer)
		return;

	pTimer->SetUp_TimeDelta();
}

HRESULT CTimerMgr::Ready_Timer(TIMERID eTimerID)
{
	CTimer*		pTimer = Find_Timer(eTimerID);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	NULL_CHECK_RETURN(pTimer, E_FAIL);

	m_mapTimers.emplace(eTimerID, pTimer);


	return S_OK;
}

Engine::CTimer* CTimerMgr::Find_Timer(TIMERID eTimerID)
{
	auto	iter = m_mapTimers.find(eTimerID);

	if (iter == m_mapTimers.end())
		return nullptr;

	return iter->second;
}

void CTimerMgr::Free()
{
	for_each(m_mapTimers.begin(), m_mapTimers.end(), CDeleteMap());
	m_mapTimers.clear();
}