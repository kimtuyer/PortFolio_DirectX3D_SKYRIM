#include "stdafx.h"
#include "MonsterBar.h"

#include "Export_Function.h"

CMonsterBar::CMonsterBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CMonsterBar::CMonsterBar(const CMonsterBar& rhs)
	: CGameObject(rhs)
{

}

CMonsterBar::~CMonsterBar(void)
{

}

HRESULT CMonsterBar::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = 400.f;
	m_fY = 100.f;

	m_fSizeX = 800.f;
	m_fSizeY = 100.f;

	m_iHP = 1;

	return S_OK;
}

Engine::_int CMonsterBar::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	//Add_RenderGroup(RENDER_MonsterBar, this);
	Add_RenderGroup(RENDER_NONALPHA, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	return 0;
}

void CMonsterBar::Render_Object(void)
{

	if (m_bRenderOn)
	{
		if(m_ObjectID==OBJECT_GuardMan)
		Render_Font(L"Font_Skyrim", L"Guard", &_vec2(370, 120), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //충돌 NPC이름 가져와 출력
		else
			Render_Font(L"Font_Skyrim", L"Dragon", &_vec2(370, 120), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //충돌 NPC이름 가져와 출력


		_matrix		matWorld, matView, matOldView, matOldProj;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matView);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

		matWorld._11 = m_fSizeX * 0.1f;
		matWorld._22 = m_fSizeY * 0.1f;
		matWorld._33 = 1.f;
		matWorld._41 = m_fX - WINCX * 0.5f;
		matWorld._42 = -m_fY + WINCY * 0.5f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);



		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);

		m_pTextureCom->Render_Texture();
		m_pBufferCom->Render_Buffer();


		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	}
}

HRESULT CMonsterBar::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_MonsterBar"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;

}

CMonsterBar* CMonsterBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonsterBar*	pInstance = new CMonsterBar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMonsterBar::Free(void)
{
	CGameObject::Free();
}

