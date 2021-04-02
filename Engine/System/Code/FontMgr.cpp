#include "FontMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFontMgr)

Engine::CFontMgr::CFontMgr()
{

}

Engine::CFontMgr::~CFontMgr()
{
	Free();
}

HRESULT Engine::CFontMgr::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, FONTID eFontID, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	CFont*		pFont = nullptr;

	pFont = Find_Font(eFontID);
	if (nullptr != pFont)
		return E_FAIL;

	pFont = CFont::Create(pGraphicDev, pFontType, iWidth, iHeight, iWeight);
	NULL_CHECK_RETURN(pFont, E_FAIL);

	m_mapFont.emplace(eFontID, pFont);

	return S_OK;
}

void Engine::CFontMgr::Render_Font(FONTID eFontID, const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color)
{
	CFont*	pFont = Find_Font(eFontID);
	NULL_CHECK(pFont);

	pFont->Render_Font(pString, pPos, Color);

}

CFont* Engine::CFontMgr::Find_Font(FONTID eFontID)
{
	auto	iter = m_mapFont.find(eFontID);
	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void Engine::CFontMgr::Free()
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}