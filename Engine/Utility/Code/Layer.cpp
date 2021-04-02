#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer()
	: m_iVecSize(0)
{
}

Engine::CLayer::~CLayer()
{
}

CComponent * CLayer::Get_Component(const _int & iIndex, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eType)
{
	if (m_vecObjectList[iIndex].empty())
		return nullptr;

	auto	iter = find_if(m_vecObjectList[iIndex].begin(), m_vecObjectList[iIndex].end(), CObjTagFinder(pObjTag));

	if (iter == m_vecObjectList[iIndex].end())
		return nullptr;

	return (*iter)->Get_Component(pComponentTag, eType);
}

CGameObject * CLayer::Get_FirstGameObject(const _int & iIndex)
{
	if (m_vecObjectList[iIndex].empty())
		return nullptr;

	return m_vecObjectList[iIndex].front();
}

list<CGameObject*> CLayer::Get_List(const _int & iIndex)
{
	return m_vecObjectList[iIndex];
}

HRESULT CLayer::Add_GameObject(const _int & iIndex, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_vecObjectList[iIndex].emplace_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Ready_Layer(_int iVecSize)
{
	m_iVecSize = iVecSize;
	m_vecObjectList.resize(m_iVecSize);

	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	_int iExit = 0;
	
	for (int i = 0; i < m_iVecSize; ++i)
	{
		for (auto& iter = m_vecObjectList[i].begin(); iter != m_vecObjectList[i].end(); )
		{
			int iEvent = (*iter)->Update_Object(fTimeDelta);

			if (OBJ_DEAD == iEvent)
			{
				Safe_Release(*iter);
				iter = m_vecObjectList[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	return 0;
}

void CLayer::LateUpdate_Layer(const _float & fTimeDelta)
{
	for (int i = 0; i < m_iVecSize; ++i)
	{
		for (auto& pObject : m_vecObjectList[i])
		{
			pObject->LateUpdate_Object(fTimeDelta);

			if (m_vecObjectList[i].empty())
				break;
		}
	}
}



void CLayer::Free()
{
	for (int i = 0; i < m_iVecSize; ++i)
	{
		for_each(m_vecObjectList[i].begin(), m_vecObjectList[i].end(), CDeleteObj());
		m_vecObjectList[i].clear();
	}
	m_vecObjectList.clear();
}
