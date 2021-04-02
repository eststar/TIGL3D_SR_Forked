#ifndef FrameMgr_h__
#define FrameMgr_h__

#include "Base.h"
#include "Frame.h"

BEGIN(Engine)

class ENGINE_DLL CFrameMgr : public CBase
{
	DECLARE_SINGLETON(CFrameMgr)

private:
	explicit CFrameMgr();
	virtual ~CFrameMgr();

public:
	_bool		IsPermit_Call(FRAMEID eFrameID, const _float& fTimeDelta);

public:
	HRESULT		Ready_Frame(FRAMEID eFrameID, const _float& fCallLimit);

private:
	CFrame*		Find_Frame(FRAMEID eFrameID);

private:
	map<FRAMEID, CFrame*>				m_mapFrames;

public:
	virtual void	Free();

};

END
#endif // FrameMgr_h__
