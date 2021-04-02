#include "FrameMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrameMgr)

Engine::CFrameMgr::CFrameMgr()
{

}

Engine::CFrameMgr::~CFrameMgr()
{
	Free();
}

Engine::_bool Engine::CFrameMgr::IsPermit_Call(FRAMEID eFrameID, const _float& fTimeDelta)
{
	CFrame*	pFrame = Find_Frame(eFrameID);

	if (nullptr == pFrame)
		return false;

	return pFrame->IsPermit_Call(fTimeDelta);
}

HRESULT Engine::CFrameMgr::Ready_Frame(FRAMEID eFrameID, const _float& fCallLimit)
{
	CFrame*	pFrame = Find_Frame(eFrameID);

	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fCallLimit);
	NULL_CHECK_RETURN(pFrame, E_FAIL);

	m_mapFrames.emplace(eFrameID, pFrame);

	return S_OK;
}

Engine::CFrame* Engine::CFrameMgr::Find_Frame(FRAMEID eFrameID)
{
	auto	iter = m_mapFrames.find(eFrameID);

	if (iter == m_mapFrames.end())
		return nullptr;
	
	return iter->second;
}

void Engine::CFrameMgr::Free()
{
	for_each(m_mapFrames.begin(), m_mapFrames.end(), CDeleteMap());
	m_mapFrames.clear();
}